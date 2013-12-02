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

class Cloud : public MPxNode 
{
public:
	Cloud();
    virtual ~Cloud();

	virtual MStatus compute( const MPlug&, MDataBlock& );
virtual void    postConstructor();

	static  void *  creator();
	static  MStatus initialize();

	//  Id tag for use with binary file format
	static  MTypeId id;
};

// Static data
MTypeId Cloud::id(0x81016);

Cloud::Cloud() { };

MStatus compute(const MPlug &plug, MDataBlock &block) { 
	return MS::kSuccess;
};

