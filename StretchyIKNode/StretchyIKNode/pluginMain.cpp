#include <maya/MFnPlugin.h>
#include "StretchyIK.h"

MStatus initializePlugin(MObject plugin)
{
	MStatus stat;
	MFnPlugin fnPlugin(plugin, "Mattia Sicuro", "1.0", "Any", &stat); CHECK_MSTATUS_AND_RETURN_IT(stat);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("StretchyIK", StretchyIK::typeID, StretchyIK::creator, StretchyIK::initialize));
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject plugin)
{
	MFnPlugin fnPlugin(plugin);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(StretchyIK::typeID));
	return MS::kSuccess;
}