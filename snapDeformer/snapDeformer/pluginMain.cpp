#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>

#include "snapDeformer.h"

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin fnPlugin(obj, "Mattia", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.registerNode("snapDeformer", SnapDeformer::typeId, SnapDeformer::creator, SnapDeformer::initialize, MPxNode::kDeformerNode));

	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin fnPlugin;
	CHECK_MSTATUS_AND_RETURN_IT(fnPlugin.deregisterNode(SnapDeformer::typeId));

	return MS::kSuccess;
}