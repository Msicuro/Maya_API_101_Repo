#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "OpenMayaRender.lib")

#include <maya/MDrawRegistry.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include "stressMap.h"
#include "stressMapOverride.h"

const MString pluginRegistrantId("stressMap");

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin fnPlugin(obj, "Mattia Sicuro", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("locatorNodeVP12", LocatorNodeVP12::typeID, LocatorNodeVP12::creator, LocatorNodeVP12::initialize, MPxNode::kLocatorNode, &LocatorNodeVP12::drawDbClassification));
	CHECK_MSTATUS_AND_RETURN_IT(MHWRender::MDrawRegistry::registerDrawOverrideCreator(LocatorNodeVP12::drawDbClassification, "locatorNodeVP12", LocatorNodeVP12Override::creator));
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin fnPlugin(obj);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(LocatorNodeVP12::typeID));
	CHECK_MSTATUS_AND_RETURN_IT(MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(LocatorNodeVP12::drawDbClassification, "LocatorNodeVP12"));
	return MS::kSuccess;
}