#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "OpenMayaRender.lib")
#pragma comment(lib, "OpenMayaUI.lib")

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
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("stressMap", StressMap::typeId, StressMap::creator, StressMap::initialize, MPxNode::kLocatorNode, &StressMap::kDrawDbClassification));
	CHECK_MSTATUS_AND_RETURN_IT(MHWRender::MDrawRegistry::registerDrawOverrideCreator(StressMap::kDrawDbClassification, "stressMap", StressMapOverride::creator));
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin fnPlugin(obj);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(StressMap::typeId));
	CHECK_MSTATUS_AND_RETURN_IT(MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(StressMap::kDrawDbClassification, "StressMap"));
	return MS::kSuccess;
}