#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "OpenMayaRender.lib")
#pragma comment(lib, "OpenMayaUI.lib")
#pragma comment(lib, "OpenMayaAnim.lib")

#include "relaxNode.h"
#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>

const MString pluginRegistrantId("relaxNode");

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin fnPlugin(obj, "Mattia Sicuro", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("relaxNode", RelaxNode::typeId, RelaxNode::creator, RelaxNode::initialize, MPxNode::kDeformerNode));
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin fnPlugin(obj);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(RelaxNode::typeId));
	return MS::kSuccess;
}