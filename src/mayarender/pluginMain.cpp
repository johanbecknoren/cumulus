//
// Copyright (C) 
// 
// File: pluginMain.cpp
//
// Author: Maya Plug-in Wizard 2.0
//

#include "cloud_node.h"
#include "clouddrawoverride.h"

#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>

MStatus initializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is loaded into Maya.  It 
//		registers all of the services that this plug-in provides with 
//		Maya.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "", "0.0.1", "Any");

	status = plugin.registerNode("sfxCloud", CloudNode::id, CloudNode::creator,
								  CloudNode::initialize,
								  MPxNode::kLocatorNode,
                                &CloudNode::drawDbClassification);
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		CloudNode::drawDbClassification,
		CloudNode::drawRegistrantId,
		CloudDrawOverride::Creator
		);
	if (!status) {
        status.perror("registerDrawOverrideCreator");
        return status;
    }
	return status;
}

MStatus uninitializePlugin( MObject obj)
//
//	Description:
//		this method is called when the plug-in is unloaded from Maya. It 
//		deregisters all of the services that it was providing.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterNode( CloudNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
