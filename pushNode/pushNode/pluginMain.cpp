#pragma once

#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>

#include "pushNode.h"

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin fnPlugin(obj, "Mattia", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("pushNode", PushNode::typeId, PushNode::creator, PushNode::initialize, MPxNode::kDeformerNode));

	MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin fnPlugin;
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(PushNode::typeId));

	return MS::kSuccess;
}