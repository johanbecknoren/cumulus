#ifndef _buildMayaNode
#define _buildMayaNode

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPxLocatorNode.h>
#include <maya/MPxDrawOverride.h>

class CloudNode : public MPxLocatorNode
{
public:
						CloudNode();
	virtual				~CloudNode(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	virtual void draw(M3dView & view, const MDagPath &path,
                             M3dView::DisplayStyle style,
                             M3dView::DisplayStatus status);

	virtual bool isBounded() const;
	virtual MBoundingBox boundingBox() const;

	static  void*		creator();
	static  MStatus		initialize();

	// The typeid is a unique 32bit indentifier that describes this node.
	// It is used to save and retrieve nodes of this type from the binary
	// file format.  If it is not unique, it will cause file IO problems.
	//
	static	MTypeId id;
	static  MObject size;
	static  MString drawDbClassification;
    static  MString drawRegistrantId;

	// There needs to be a MObject handle declared for each attribute that
	// the node will have.  These handles are needed for getting and setting
	// the values later.
	//
	static  MObject input;		// Example input attribute
	static  MObject output;		// Example output attribute
};



#endif
