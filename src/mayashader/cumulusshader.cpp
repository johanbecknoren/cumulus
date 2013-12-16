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
#include <coregl.h>
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
    static MObject  aColorFlame;
    static MObject  aPlaceMat;
    static MObject  aPointWorld;

	// Output attributes
    static MObject  aOutAlpha;
    static MObject  aOutColor;
	//core::CoreGL *vol;
};

// Static data
MTypeId Flame3D::id(0x81016);

// Attributes
MObject  Flame3D::aColorFlame;

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
    CHECK_MSTATUS(addAttribute(aColorFlame));
    CHECK_MSTATUS(addAttribute(aPointWorld));
    CHECK_MSTATUS(addAttribute(aPlaceMat));

    CHECK_MSTATUS(addAttribute(aOutAlpha));
    CHECK_MSTATUS(addAttribute(aOutColor));

    // All input affect the output color and alpha

    CHECK_MSTATUS(attributeAffects (aColorFlame, aOutColor));
    CHECK_MSTATUS(attributeAffects(aColorFlame, aOutAlpha));


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
    const MFloatVector& cFlame=block.inputValue(aColorFlame).asFloatVector();
    
	MFloatPoint q(worldPos[0], worldPos[1], worldPos[2]);
	q *= mat;								// Convert into solid space

	q.x = (1.f + q.x) * 0.5f;
	q.y = (1.f + q.y) * 0.5f;
	q.z = (1.f + q.z) * 0.5f;
	// Offset texture coord along the RiseAxis

	float d = core::CoreGL::getDensityAtWorld(q.x,q.y,q.z,256,256,256);

    MFloatVector resultColor;

	if (cFlame.x > 0.5f)
		resultColor = MFloatVector(1.f - d, 1.f - d, 1.f - d); //cFlame;
	else
		resultColor = MFloatVector(d, d, d); //cFlame;
	if (d > 0.f && d < 1.f) {
		cout << "C - q:x" << q.x << " y" << q.y << " z" << q.z << " d" << d << flush << endl;
	}
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
