#include "clouddrawoverride.h"
#include "cloud_node.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MStateManager.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MDistance.h>

CloudDrawOverride::~CloudDrawOverride() {}

CloudDrawOverride::CloudDrawOverride(const MObject &obj) : MHWRender::MPxDrawOverride(obj, CloudDrawOverride::draw) {

}

MHWRender::DrawAPI CloudDrawOverride::supportedDrawAPIs() const
{
	// this plugin supports only GL
	return (MHWRender::kOpenGL);
	//return (MHWRender::kOpenGL | MHWRender::kDirectX11);
}

MBoundingBox CloudDrawOverride::boundingBox(
	const MDagPath& objPath,
	const MDagPath& cameraPath) const
{
	MPoint corner1( -0.17, 0.0, -0.7 );
	MPoint corner2( 0.17, 0.0, 0.3 );

	float multiplier = getMultiplier(objPath);
	corner1 = corner1 * multiplier;
	corner2 = corner2 * multiplier;

	CloudDrawOverride *nonConstThis = (CloudDrawOverride *) this;
	nonConstThis->mCurrentBoundingBox.clear();
	nonConstThis->mCurrentBoundingBox.expand( corner1 );
	nonConstThis->mCurrentBoundingBox.expand( corner2 );

	return mCurrentBoundingBox;
}

bool CloudDrawOverride::isBounded(
	const MDagPath& objPath,
	const MDagPath& cameraPath) const {
		// What is this?
		return true;
}

float CloudDrawOverride::getMultiplier(const MDagPath& objPath) const
{
	// Retrieve value of the size attribute from the node
	MStatus status;
	MObject cloudNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(cloudNode, CloudNode::size);
		if (!plug.isNull())
		{
			MDistance sizeVal;
			if (plug.getValue(sizeVal))
			{
				return (float)sizeVal.asCentimeters();
			}
		}
	}

	return 1.0f;
}

void CloudDrawOverride::addUIDrawables(
	const MDagPath& objPath,
	MHWRender::MUIDrawManager& drawManager,
	const MHWRender::MFrameContext& frameContext,
	const MUserData* data)
{
	// Draw a text "Foot"
	MPoint pos( 0.0, 0.0, 0.0 ); // Position of the text
	MColor textColor( 0.1f, 0.8f, 0.8f, 1.0f ); // Text color

	drawManager.beginDrawable();

	drawManager.setColor( textColor );
	drawManager.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );
	drawManager.text( pos,  MString("Clouds"), MHWRender::MUIDrawManager::kCenter );

	drawManager.endDrawable();
}

MUserData* CloudDrawOverride::prepareForDraw(
	const MDagPath& objPath,
	const MDagPath& cameraPath,
	const MHWRender::MFrameContext& frameContext,
	MUserData* oldData)
{
	// Retrieve data cache (create if does not exist)
	CloudData* data = dynamic_cast<CloudData*>(oldData);
	if (!data)
	{
		data = new CloudData();
	}

	// compute data and cache it
	data->fMultiplier = getMultiplier(objPath);
	MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
	data->fColor[0] = color.r;
	data->fColor[1] = color.g;
	data->fColor[2] = color.b;

	if(!data->cloudcore)
		data->cloudcore = core::CoreGL::creator("box.obj");

	return data;
}

void CloudDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data) {
	// get renderer
	MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
	if (!theRenderer) return;

	// get state data
	MStatus status;
	const MMatrix transform =
		context.getMatrix(MHWRender::MDrawContext::kWorldViewMtx, &status);
	if (status != MStatus::kSuccess) return;
	const MMatrix projection =
		context.getMatrix(MHWRender::MDrawContext::kProjectionMtx, &status);
	if (status != MStatus::kSuccess) return;

	const int displayStyle = context.getDisplayStyle();
	const CloudData* cloudData = dynamic_cast<const CloudData*>(data);
	if (!cloudData)
		return;

	// If we don't want to draw the bounds within this plugin
	// manaully, then skip drawing altogether in bounding box mode
	// since the bounds draw is handled by the renderer and
	// doesn't need to be drawn here.
	//
	if (displayStyle & MHWRender::MDrawContext::kBoundingBox)
	{
		return;
	}

	// Check to see if we are drawing in a shadow pass.
	// If so then we keep the shading simple which in this
	// example means to disable any extra blending state changes
	//
	const MHWRender::MPassContext & passCtx = context.getPassContext();
	const MStringArray & passSem = passCtx.passSemantics();
	bool castingShadows = false;
	for (unsigned int i=0; i<passSem.length(); i++)
	{
		if (passSem[i] == MHWRender::MPassContext::kShadowPassSemantic)
			castingShadows = true;
	}
	bool debugPassInformation = false;
	if (debugPassInformation)
	{
		const MString & passId = passCtx.passIdentifier();
		printf("footprint node drawing in pass[%s], semantic[", passId.asChar());
		for (unsigned int i=0; i<passSem.length(); i++)
			printf(" %s", passSem[i].asChar());
		printf("\n");
	}

	// get cached data
	float multiplier = cloudData->fMultiplier;
	float color[4] = {
		cloudData->fColor[0],
		cloudData->fColor[1],
		cloudData->fColor[2],
		1.0f
	};

	bool requireBlending = false;

	// If we're not casting shadows then do extra work
	// for display styles
	if (!castingShadows)
	{

		// Use some monotone version of color to show "default material mode"
		//
		if (displayStyle & MHWRender::MDrawContext::kDefaultMaterial)
		{
			color[0] = color[1] = color[2] = (color[0] + color[1] + color[2]) / 3.0f;
		}
		// Do some alpha blending if in x-ray mode
		//
		else if (displayStyle & MHWRender::MDrawContext::kXray)
		{
			requireBlending = true;
			color[3] = 0.3f;
		}
	}

	// Set blend and raster state
	//
	MHWRender::MStateManager* stateMgr = context.getStateManager();
	const MHWRender::MBlendState* pOldBlendState = NULL;
	const MHWRender::MRasterizerState* pOldRasterState = NULL;
	bool rasterStateModified = false;

	if(stateMgr && (displayStyle & MHWRender::MDrawContext::kGouraudShaded))
	{
		// draw filled, and with blending if required
		if (stateMgr && requireBlending)
		{
			static const MHWRender::MBlendState* blendState = NULL;
			if (!blendState)
			{
				MHWRender::MBlendStateDesc desc;
				desc.targetBlends[0].blendEnable = true;
				desc.targetBlends[0].destinationBlend = MHWRender::MBlendState::kInvSourceAlpha;
				desc.targetBlends[0].alphaDestinationBlend = MHWRender::MBlendState::kInvSourceAlpha;
				blendState = stateMgr->acquireBlendState(desc);
			}

			if (blendState)
			{
				pOldBlendState = stateMgr->getBlendState();
				stateMgr->setBlendState(blendState);
			}
		}

		// Override culling mode since we always want double-sided
		//
		pOldRasterState = stateMgr ? stateMgr->getRasterizerState() : NULL;
		if (pOldRasterState)
		{
			MHWRender::MRasterizerStateDesc desc( pOldRasterState->desc() );
			// It's also possible to change this to kCullFront or kCullBack if we
			// wanted to set it to that.
			MHWRender::MRasterizerState::CullMode cullMode = MHWRender::MRasterizerState::kCullNone;
			if (desc.cullMode != cullMode)
			{
				static const MHWRender::MRasterizerState *rasterState = NULL;
				if (!rasterState)
				{
					// Just override the cullmode
					desc.cullMode = cullMode;
					rasterState = stateMgr->acquireRasterizerState(desc);
				}
				if (rasterState)
				{
					rasterStateModified = true;
					stateMgr->setRasterizerState(rasterState);
				}
			}
		}
	}

	// GL Draw
	//if (theRenderer->drawAPIIsOpenGL())
	{

		if (cloudData->cloudcore != NULL) 
		{	
			cout << "trans:";
			glm::mat4 tproj = core::carrToGlmM4(projection.matrix);
			cout << endl << "Proj";
			glm::mat4 ttrans = core::carrToGlmM4(transform.matrix);
			cout << endl;
			cloudData->cloudcore->render(ttrans, tproj);
		}
		else {
			cout << "NULL" << endl;
		}
		// set colour
		glColor4fv(color);
		// set world matrix
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixd(transform.matrix[0]);

		// set projection matrix
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadMatrixd(projection.matrix[0]);

		glColor3f(1, 0, 0);
 
		//glBegin(GL_QUADS);
		//glVertex3f(-0.5, 0.0, -0.5);
		//glVertex3f(0.5, 0.0, -0.5);
		//glVertex3f(0.5, 0.0, 0.5);
		//glVertex3f(-0.5, 0.0, 0.5);
		//glEnd();

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	// Restore old blend state and old raster state
	if(stateMgr && (displayStyle & MHWRender::MDrawContext::kGouraudShaded))
	{
		if (stateMgr && pOldBlendState)
		{
			stateMgr->setBlendState(pOldBlendState);
		}
		if (rasterStateModified && pOldRasterState)
		{
			stateMgr->setRasterizerState(pOldRasterState);
		}
	}
}