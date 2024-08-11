#pragma once

#pragma comment(lib, "OpenMayaUI.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "OpenMayaRender.lib")

// _OPENMAYA_DEPRECATION_DISABLE_WARNING defined in preprocessor

#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>
#include "locatorNodeVP1.h"
#include "locatorNodeVP12.h"

MStatus initializePlugin(MObject pluginObject)
{
	MStatus status;
	MFnPlugin fnPlugin(pluginObject, "Mattia Sicuro", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("locatorNodeVP1", LocatorNodeVP1::typeID, LocatorNodeVP1::creator, LocatorNodeVP1::initialize, MPxNode::kLocatorNode));
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("locatorNodeVP12", LocatorNodeVP12::typeID, LocatorNodeVP12::creator, LocatorNodeVP12::initialize, MPxNode::kLocatorNode, &LocatorNodeVP12::drawDbClassification));
	CHECK_MSTATUS_AND_RETURN_IT(MHWRender::MDrawRegistry::registerDrawOverrideCreator(LocatorNodeVP12::drawDbClassification, "locatorNodeVP12", LocatorNodeVP12Override::creator));
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject pluginObject)
{
	MFnPlugin fnPlugin(pluginObject);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(LocatorNodeVP1::typeID));
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(LocatorNodeVP12::typeID));
	CHECK_MSTATUS_AND_RETURN_IT(MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(LocatorNodeVP12::drawDbClassification), "LocatorNodeVP12");
	return MS::kSuccess;
}