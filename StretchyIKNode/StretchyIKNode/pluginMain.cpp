#include <maya/MFnPlugin.h>
#include "StretchyIK.h"

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin fnPlugin(obj, "Mattia Sicuro", "1.0", "Any", &stat); CHECK_MSTATUS_AND_RETURN_IT(stat);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("Stretchy IK", StretchyIK::typeID, StretchyIK::creator, StretchyIK::initialize));
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin fnPlugin(obj);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(StretchyIK::typeID));
	return MS::kSuccess;
}