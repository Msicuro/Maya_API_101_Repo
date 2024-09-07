#pragma once

#include "relaxNode.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MGlobal.h>

MTypeId RelaxNode::typeId(90012);

MObject amount;
MObject inputMesh;

// Define constants
#define SMALL (float)1e-6
#define BIG_DIST 99999

RelaxNode::RelaxNode() {}

void* RelaxNode::creator() { return new RelaxNode(); }

MStatus RelaxNode::initialize()
{
	MStatus status;
	MFnNumericAttribute numFn;
	MFnTypedAttribute typeFn;

	inputMesh = typeFn.create("inputMesh", "inputMesh", MFnData::kMesh, MObject::kNullObj, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inputMesh); CHECK_MSTATUS_AND_RETURN_IT(status);

	amount = numFn.create("amount", "amount", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(amount); CHECK_MSTATUS_AND_RETURN_IT(status);

	MGlobal::executeCommand("makePaintable -attrType multiFloat -sm deformer pushNode weights");

	return MS::kSuccess;
}

MStatus RelaxNode::deform(MDataBlock& data, MItGeometry& iter, const MMatrix& matrix, unsigned int mIndex)
{
	// Get the needed data
	double envelopeValue = data.inputValue(envelope).asFloat();
	double amountValue = data.inputValue(amount).asDouble();

	// Check if the mesh is connected
	MPlug inputMeshP(thisMObject(), inputMesh);
	if (!inputMeshP.isConnected()) { return MS::kNotImplemented; }

	// Gather data
	MObject driverMeshV = data.inputValue(inputMesh).asMesh();

	// If envelope is zero, do not compute
	MDataHandle envelopeHandle = data.inputValue(envelope);
	double envelopeV = envelopeHandle.asFloat();
	
	if (envelopeV < SMALL)
	{
		return MS::kSuccess;
	}


}