#pragma once

#include "pushNode.h"
#include <maya/MItGeometry.h>
#include <maya/MItMeshVertex.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MPlug.h>
#include <set>

#include <maya/MPxDeformerNode.h>


#define SMALL (float)1e-6
#define BIG_DIST 99999

MTypeId PushNode::typeId(0x90011);

MObject PushNode::stressMap;
MObject PushNode::useStress;
MObject PushNode::amount;

PushNode::PushNode() { }

void* PushNode::creator() { return new PushNode(); }

MStatus PushNode::initialize()
{
	MStatus status;
	MFnNumericAttribute numericAttr;
	MFnTypedAttribute typedFn;

	useStress = numericAttr.create("useStress", "useStress", MFnNumericData::kBoolean, 0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericAttr.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericAttr.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(useStress); CHECK_MSTATUS_AND_RETURN_IT(status);

	amount = numericAttr.create("amount", "amount", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericAttr.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericAttr.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(amount); CHECK_MSTATUS_AND_RETURN_IT(status);

	stressMap = typedFn.create("stressMap", "stressMap", MFnData::kDoubleArray, MObject::kNullObj, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = typedFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = typedFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = typedFn.setWritable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(stressMap); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(stressMap, outputGeom); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(amount, outputGeom); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(useStress, outputGeom); CHECK_MSTATUS_AND_RETURN_IT(status);

	MGlobal::executeCommand("makePaintable -attrType multiFloat -sm deformer pushNode weights");

	return MS::kSuccess;
}

MStatus PushNode::deform(MDataBlock& data, MItGeometry& iter, const MMatrix& matrix, unsigned int mIndex)
{
	// Get the needed data
	double envelopeValue = data.inputValue(envelope).asFloat();
	bool useStressValue = data.inputValue(useStress).asBool();
	double amountValue = data.inputValue(amount).asDouble();

	// If the UseStress attr is on, pull the data from the stress map
	MDoubleArray stressValue;
	if (useStressValue)
	{
		// Pull the raw data as an MObject
		MObject stressData = data.inputValue(stressMap).data();
		// Convert the data to a double array
		MFnDoubleArrayData stressDataFn(stressData);
		stressValue = stressDataFn.array();
	}

	// If envelope is zero do not compute
	if (envelopeValue < SMALL) { return MS::kSuccess; }

	// Pull out all the points
	MPointArray pos;
	iter.allPositions(pos, MSpace::kWorld);

	// Access the input mesh and create a meshFn instance
	MArrayDataHandle meshHandle = data.inputArrayValue(input);
	meshHandle.jumpToArrayElement(0);
	MObject mesh = meshHandle.inputValue().child(inputGeom).asMesh();
	MFnMesh meshFn(mesh);

	// Retrieve all of the Normals
	MFloatVectorArray normals;
	meshFn.getNormals(normals, MSpace::kWorld);

	MPoint temp;
	for (int i = 0; i < iter.exactCount(); i++)
	{
		if (useStressValue)
		{
			pos[i] += (MVector(normals[i]) * envelopeValue * amountValue * stressValue[i]);
		}
		else
		{
			pos[i] += (MVector(normals[i]) * envelopeValue * amountValue);
		}
	}

	// Set all the positions
	iter.setAllPositions(pos);

	return MStatus::kSuccess;
}

