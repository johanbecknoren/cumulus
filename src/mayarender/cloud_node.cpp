//
// Copyright (C) 
// 
// File: buildMayaNode.cpp
//
// Dependency Graph Node: buildMaya
//
// Author: Maya Plug-in Wizard 2.0
//

#include "cloud_node.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MDistance.h>
#include <maya/MGlobal.h>

// You MUST change this to a unique value!!!  The id is a 32bit value used
// to identify this type of node in the binary file format.  
//
MTypeId CloudNode::id( 0x81736 );
MObject CloudNode::size;
MString CloudNode::drawDbClassification("drawdb/geometry/cloud");
MString CloudNode::drawRegistrantId("CloudNodePlugin");
// Example attributes
// 
MObject     CloudNode::input;        
MObject     CloudNode::output;       

CloudNode::CloudNode() {}
CloudNode::~CloudNode() {}

MStatus CloudNode::compute( const MPlug& plug, MDataBlock& data ) {
	return MS::kUnknownParameter;
}

// Standard viewport implementation
void CloudNode::draw( M3dView & view, const MDagPath &path,
	M3dView::DisplayStyle style,
	M3dView::DisplayStatus status ) 
{
	MObject thisNode = thisMObject();
	MPlug plug( thisNode, size );
	MDistance sizeVal;
	plug.getValue( sizeVal );

	float multiplier = (float) sizeVal.asCentimeters();

	view.beginGL();


	if ( ( style == M3dView::kFlatShaded ) ||
		( style == M3dView::kGouraudShaded ) )
	{
		// Push the color settings
		//
		glPushAttrib( GL_CURRENT_BIT );

		if ( status == M3dView::kActive ) {
			view.setDrawColor( 13, M3dView::kActiveColors );
		} else {
			view.setDrawColor( 13, M3dView::kDormantColors );
		}

		glBegin(GL_QUADS);
		glVertex3f(-0.5, 0.0, -0.5);
		glVertex3f(0.5, 0.0, -0.5);
		glVertex3f(0.5, 0.0, 0.5);
		glVertex3f(-0.5, 0.0, 0.5);
		glEnd();
		

		glPopAttrib();
	}

	// Draw the outline of the foot
	//
	glBegin( GL_LINES );

	glEnd();


	view.endGL();
	
}

bool CloudNode::isBounded() const {
	return true;
}

MBoundingBox CloudNode::boundingBox() const {
	MObject thisNode = thisMObject();
	MPlug plug( thisNode, size );
	MDistance sizeVal;
	plug.getValue( sizeVal );

	double multiplier = sizeVal.asCentimeters();

	MPoint corner1( -0.17, 0.0, -0.7 );
	MPoint corner2( 0.17, 0.0, 0.3 );

	corner1 = corner1 * multiplier;
	corner2 = corner2 * multiplier;

	return MBoundingBox( corner1, corner2 );
}

void* CloudNode::creator() {
	return new CloudNode();
}

MStatus CloudNode::initialize() {
	// This sample creates a single input float attribute and a single
	// output float attribute.
	//
	MFnNumericAttribute nAttr;
	MStatus				stat;

	input = nAttr.create( "input", "in", MFnNumericData::kFloat, 0.0 );
	// Attribute will be written to files when this type of node is stored
	nAttr.setStorable(true);
	// Attribute is keyable and will show up in the channel box
	nAttr.setKeyable(true);

	output = nAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
	// Attribute is read-only because it is an output attribute
	nAttr.setWritable(false);
	// Attribute will not be written to files when this type of node is stored
	nAttr.setStorable(false);

	// Add the attributes we have created to the node
	//
	stat = addAttribute( input );
	if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( output );
	if (!stat) { stat.perror("addAttribute"); return stat;}

	// Set up a dependency between the input and the output.  This will cause
	// the output to be marked dirty when the input changes.  The output will
	// then be recomputed the next time the value of the output is requested.
	//
	stat = attributeAffects( input, output );
	if (!stat) { stat.perror("attributeAffects"); return stat;}

	return MS::kSuccess;

}

