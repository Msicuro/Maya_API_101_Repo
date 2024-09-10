#include <maya/MFnPlugin.h>
#include "aimNode.h"

MStatus initializePlugin(MObject plugin)
{
	MStatus status;
	MFnPlugin fnPlugin(plugin, "Mattia Sicuro", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("AimNode", AimNode::typeID, AimNode::creator, AimNode::initialize));
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject plugin)
{
	MFnPlugin fnPlugin(plugin);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(AimNode::typeID));
	return MS::kSuccess;
}