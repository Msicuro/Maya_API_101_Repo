#pragma once

#include "chainOnPath.h"
#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>


MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin fnPlugin(obj, "Mattia", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("chainOnPath", ChainOnPath::typeId, ChainOnPath::creator, ChainOnPath::initialize));

	MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin fnPlugin;
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(ChainOnPath::typeId));

	return MS::kSuccess;
}