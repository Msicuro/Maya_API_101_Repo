#pragma once

#pragma comment(lib, "OpenMayaUI.lib")
#pragma comment(lib, "opengl32.lib")

// _OPENMAYA_DEPRECATION_DISABLE_WARNING defined in preprocessor

#include <maya/MFnPlugin.h>
#include "locatorNodeVP1.h"

MStatus initializePlugin(MObject pluginObject)
{
	MStatus status;
	MFnPlugin fnPlugin(pluginObject, "Mattia Sicuro", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("locatorNode", LocatorNodeVP1::typeID, LocatorNodeVP1::creator, LocatorNodeVP1::initialize));
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject pluginObject)
{
	MFnPlugin fnPlugin(pluginObject);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(LocatorNodeVP1::typeID));
	return MS::kSuccess;
}