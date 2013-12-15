//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#include <math.h>
#include <stdlib.h>

#include <maya/MPxNode.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h> 
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatPoint.h>
#include <maya/MFnPlugin.h>

#define PI                  3.14159265358979323846

#ifdef FLOOR
#undef FLOOR
#endif
#define FLOOR(x)            ((int)floorf(x))

#define TABLELEN            512
#define TLD2                256    // TABLELEN 

// Local variables
static int                  Phi[TABLELEN];
static char                 fPhi[TABLELEN];
static float                G[TABLELEN][3];


class Flame3D : public MPxNode
{
	public:
                    Flame3D();
    virtual         ~Flame3D();

    virtual MStatus compute( const MPlug&, MDataBlock& );
	virtual void    postConstructor();

    static  void *  creator();
    static  MStatus initialize();

	//  Id tag for use with binary file format
    static  MTypeId id;

	private:

	// Input attributes
    static MObject  aColorBase;
    static MObject  aColorFlame;
    static MObject  aRiseSpeed;
    static MObject  aFlickerSpeed;
    static MObject  aFlickerDeform;
    static MObject  aFlamePow;
    static MObject  aFlameFrame;
    static MObject  aRiseAxis;
    static MObject  aPlaceMat;
    static MObject  aPointWorld;

	// Output attributes
    static MObject  aOutAlpha;
    static MObject  aOutColor;
};

// Static data
MTypeId Flame3D::id(0x81016);

// Attributes
MObject  Flame3D::aColorBase;
MObject  Flame3D::aColorFlame;
MObject  Flame3D::aRiseSpeed;
MObject  Flame3D::aFlickerSpeed;
MObject  Flame3D::aFlickerDeform;
MObject  Flame3D::aFlamePow;
MObject  Flame3D::aFlameFrame;
MObject  Flame3D::aRiseAxis;
MObject  Flame3D::aPointWorld;
MObject  Flame3D::aPlaceMat;

MObject  Flame3D::aOutAlpha;
MObject  Flame3D::aOutColor;

#define MAKE_INPUT(attr)	\
    CHECK_MSTATUS(attr.setKeyable(true));  		\
	CHECK_MSTATUS(attr.setStorable(true));		\
    CHECK_MSTATUS(attr.setReadable(true)); 		\
	CHECK_MSTATUS(attr.setWritable(true));

#define MAKE_OUTPUT(attr)	\
    CHECK_MSTATUS(attr.setKeyable(false)); 		\
	CHECK_MSTATUS(attr.setStorable(false));		\
    CHECK_MSTATUS(attr.setReadable(true)); 		\
	CHECK_MSTATUS(attr.setWritable(false));

//
// DESCRIPTION:
///////////////////////////////////////////////////////
void Flame3D::postConstructor( )
{
	setMPSafe(true);
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
Flame3D::Flame3D()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
Flame3D::~Flame3D()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
void * Flame3D::creator()
{
    return new Flame3D();
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus Flame3D::initialize()
{
    MFnMatrixAttribute mAttr;
    MFnNumericAttribute nAttr; 

	// Create input attributes
    aRiseSpeed = nAttr.create( "Rise", "rs", MFnNumericData::kFloat);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS(nAttr.setDefault(0.1f));
    CHECK_MSTATUS(nAttr.setMin(0.0f));
    CHECK_MSTATUS(nAttr.setMax(1.0f));

    aFlickerSpeed = nAttr.create( "Speed", "s", MFnNumericData::kFloat);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS(nAttr.setDefault(0.1f));
    CHECK_MSTATUS(nAttr.setMin(0.0f));
    CHECK_MSTATUS(nAttr.setMax(1.0f));

    aFlickerDeform = nAttr.create( "Flicker", "f", MFnNumericData::kFloat);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS(nAttr.setDefault(0.5f));
    CHECK_MSTATUS(nAttr.setMin(0.0f));
    CHECK_MSTATUS(nAttr.setMax(1.0f));

    aFlamePow = nAttr.create( "Power", "pow", MFnNumericData::kFloat);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS(nAttr.setDefault(1.0f));
    CHECK_MSTATUS(nAttr.setMin(0.0f));
    CHECK_MSTATUS(nAttr.setMax(1.0f));

    aFlameFrame = nAttr.create( "Frame", "fr", MFnNumericData::kFloat);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS(nAttr.setDefault(1.0f));
    CHECK_MSTATUS(nAttr.setMin(0.0f));
    CHECK_MSTATUS(nAttr.setMax(1000.0f));

    aRiseAxis = nAttr.createPoint( "Axis", "a");
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS(nAttr.setDefault(0., 1., 0.));

    aColorBase = nAttr.createColor("ColorBase", "cg");
	MAKE_INPUT(nAttr);

    aColorFlame = nAttr.createColor("ColorFlame", "cb");
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS(nAttr.setDefault(1., 1., 1.));
    
    aPlaceMat = mAttr.create("placementMatrix", "pm",
							 MFnMatrixAttribute::kFloat);
	MAKE_INPUT(mAttr);

	// Internal shading attribute, implicitely connected.
    aPointWorld = nAttr.createPoint("pointWorld", "pw");
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS(nAttr.setHidden(true));

    // Create output attributes
    aOutColor  = nAttr.createColor("outColor",  "oc");
	MAKE_OUTPUT(nAttr);

    aOutAlpha = nAttr.create( "outAlpha", "oa", MFnNumericData::kFloat);
	MAKE_OUTPUT(nAttr);

	// Add the attributes here
    CHECK_MSTATUS(addAttribute(aColorBase));
    CHECK_MSTATUS(addAttribute(aColorFlame));
    CHECK_MSTATUS(addAttribute(aRiseSpeed));
    CHECK_MSTATUS(addAttribute(aFlickerSpeed));
    CHECK_MSTATUS(addAttribute(aFlickerDeform));
    CHECK_MSTATUS(addAttribute(aFlamePow));
    CHECK_MSTATUS(addAttribute(aFlameFrame));
    CHECK_MSTATUS(addAttribute(aRiseAxis));
    CHECK_MSTATUS(addAttribute(aPointWorld));
    CHECK_MSTATUS(addAttribute(aPlaceMat));

    CHECK_MSTATUS(addAttribute(aOutAlpha));
    CHECK_MSTATUS(addAttribute(aOutColor));

    // All input affect the output color and alpha
    CHECK_MSTATUS(attributeAffects (aColorBase, aOutColor));
    CHECK_MSTATUS(attributeAffects(aColorBase, aOutAlpha));

    CHECK_MSTATUS(attributeAffects (aColorFlame, aOutColor));
    CHECK_MSTATUS(attributeAffects(aColorFlame, aOutAlpha));

    CHECK_MSTATUS(attributeAffects(aRiseSpeed, aOutColor));
    CHECK_MSTATUS(attributeAffects(aRiseSpeed, aOutAlpha));

    CHECK_MSTATUS(attributeAffects(aFlickerSpeed, aOutColor));
    CHECK_MSTATUS(attributeAffects(aFlickerSpeed, aOutAlpha));

    CHECK_MSTATUS(attributeAffects(aFlickerDeform, aOutColor));
    CHECK_MSTATUS(attributeAffects(aFlickerDeform, aOutAlpha));

    CHECK_MSTATUS(attributeAffects(aFlamePow, aOutColor));
    CHECK_MSTATUS(attributeAffects(aFlamePow, aOutAlpha));

    CHECK_MSTATUS(attributeAffects(aFlameFrame, aOutColor));
    CHECK_MSTATUS(attributeAffects(aFlameFrame, aOutAlpha));

    CHECK_MSTATUS(attributeAffects(aRiseAxis, aOutColor));
    CHECK_MSTATUS(attributeAffects(aRiseAxis, aOutAlpha));

    CHECK_MSTATUS(attributeAffects(aPointWorld, aOutColor));
    CHECK_MSTATUS(attributeAffects (aPointWorld, aOutAlpha));

    CHECK_MSTATUS(attributeAffects(aPlaceMat, aOutColor));
    CHECK_MSTATUS(attributeAffects(aPlaceMat, aOutAlpha));

    return MS::kSuccess;
}


///////////////////////////////////////////////////////
// DESCRIPTION:
// This function gets called by Maya to evaluate the texture.
//
///////////////////////////////////////////////////////

MStatus Flame3D::compute(const MPlug& plug, MDataBlock& block) 
{ 
	// outColor or individial R, G, B channel, or alpha
    if((plug != aOutColor) && (plug.parent() != aOutColor) && 
	   (plug != aOutAlpha))
       return MS::kUnknownParameter;

	float3 & worldPos = block.inputValue( aPointWorld).asFloat3();
    const MFloatMatrix& mat = block.inputValue(aPlaceMat).asFloatMatrix();
    const MFloatVector& cBase = block.inputValue(aColorBase).asFloatVector();
    const MFloatVector& cFlame=block.inputValue(aColorFlame).asFloatVector();
    const MFloatVector& axis = block.inputValue( aRiseAxis ).asFloatVector();
    const float rise_speed = block.inputValue( aRiseSpeed ).asFloat();
    const float flicker_speed = block.inputValue( aFlickerSpeed ).asFloat();
    const float dscale = block.inputValue( aFlickerDeform ).asFloat();
    const float frame = block.inputValue( aFlameFrame ).asFloat();
    const float power = block.inputValue( aFlamePow ).asFloat();
    
	MFloatPoint q(worldPos[0], worldPos[1], worldPos[2]);
	q *= mat;								// Convert into solid space

	// Offset texture coord along the RiseAxis
    

    MFloatVector resultColor;

	resultColor = cFlame;


    MDataHandle outHandle = block.outputValue( aOutColor );
    MFloatVector & outColor = outHandle.asFloatVector();
    outColor = resultColor;
    outHandle.setClean();

    outHandle = block.outputValue(aOutAlpha);
    outHandle.asFloat() = 1;
    outHandle.setClean();

    return MS::kSuccess;
}

//////////////////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
    const MString UserClassify( "texture/3d" );

    MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any" );
    CHECK_MSTATUS( plugin.registerNode( "flamemynode", Flame3D::id, 
	    Flame3D::creator, Flame3D::initialize,
	    MPxNode::kDependNode, &UserClassify) );
    
    return MS::kSuccess;
}

// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus uninitializePlugin( MObject obj )
{
    MFnPlugin plugin( obj );
    CHECK_MSTATUS( plugin.deregisterNode( Flame3D::id ) );

    return MS::kSuccess;
}
