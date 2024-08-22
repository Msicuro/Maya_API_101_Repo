#pragma once

#include "snapDeformer.h"
#include <maya/MItGeometry.h>
#include <maya/MItMeshVertex.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MPlug.h>

// Assumed these weren't necessary since they're in the Header file but added for debugging
#include <maya/MPxDeformerNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MPointArray.h>
#include <maya/MMatrix.h>

// Define constants
#define SMALL (float)1e-6
#define BIG_DIST 99999

MTypeId SnapDeformer::typeId(0x90008);

// The "In" attributes
MObject SnapDeformer::rebind;
MObject SnapDeformer::driverMesh;
MObject SnapDeformer::referenceMesh;
MObject SnapDeformer::idAssociation;

// Constructor function
SnapDeformer::SnapDeformer()
{
	// Identifies if the node is running for the first time
	initialized = 0;
	// Keeps the count of vertices
	elemCount = 0;
	// Tracks the Ids of the matching vertices
	bindArray = MIntArray();

}

void* SnapDeformer::creator()
{
	return new SnapDeformer();
}

MStatus SnapDeformer::initialize()
{
	MStatus status;
	MFnNumericAttribute numericAttr;
	MFnTypedAttribute typeAttr;

	rebind = numericAttr.create("rebind", "rebind", MFnNumericData::kBoolean, 0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericAttr.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericAttr.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(rebind); CHECK_MSTATUS_AND_RETURN_IT(status);

	idAssociation = numericAttr.create("idAssociation", "idAssociation", MFnNumericData::kInt, 0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericAttr.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericAttr.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericAttr.setArray(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(idAssociation); CHECK_MSTATUS_AND_RETURN_IT(status);

	driverMesh = typeAttr.create("driverMesh", "driverMesh", MFnData::kMesh, MObject::kNullObj, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = typeAttr.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = typeAttr.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(driverMesh); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(rebind, outputGeom); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(driverMesh, outputGeom); CHECK_MSTATUS_AND_RETURN_IT(status);

	// Make the deformer weights paintable
	MGlobal::executeCommand("makePaintable -attrType multiFloat -sm deformer snapDeformer weights");

	return MStatus::kSuccess;

}

MStatus SnapDeformer::deform(MDataBlock& data, MItGeometry& iter, const MMatrix& matrix, unsigned int mIndex)
{
	// Check if the driver mesh is connected
	MPlug driverMeshPlug(thisMObject(), driverMesh);

	if (not driverMeshPlug.isConnected())
	{
		MGlobal::displayError("Error: Driver Mesh not connected");
		return MS::kNotImplemented;
	}

	MObject driverMeshV = data.inputValue(driverMesh).asMesh();
	
	MArrayDataHandle idAssociationV = data.inputArrayValue(idAssociation);
	idAssociationV.jumpToArrayElement(0);

	MDataHandle envelopeHandle = data.inputValue(envelope);
	double envelopeV = envelopeHandle.asFloat();
	bool rebindV = data.inputValue(rebind).asBool();

	// If envelope is zero, do not compute
	if (envelopeV < SMALL)
	{
		return MS::kSuccess;
	}

	// Get the driver point
	MPointArray driverPoint;
	MFnMesh driverGeoFn(driverMeshV);
	driverGeoFn.getPoints(driverPoint, MSpace::kWorld);

	// Get the input point
	MPointArray pos;
	iter.allPositions(pos, MSpace::kWorld);

	// Check if rebind is needed
	if (rebindV)
	{
		initData(driverMeshV, pos, bindArray, idAssociation);
	}

	if (elemCount == 0)
	{
		elemCount = iter.exactCount();
	}

	// Check if the bind array is empty or incorrect and if so, read the data from the attributes
	int arrayLength = bindArray.length();

	if (elemCount != arrayLength || initialized == 0 || arrayLength == 0)
	{
		// Read from the attribute
		ensureIndexes(idAssociation, iter.exactCount());

		// Loop through the attributes and read the values
		MArrayDataHandle idsHandle = data.inputArrayValue(idAssociation);
		idsHandle.jumpToArrayElement(0);

		int count = iter.exactCount();
		bindArray.setLength(count);

		for (int i = 0; i < count; i++, idsHandle.next())
		{
			bindArray[i] = idsHandle.inputValue().asInt();
		}

		// Set value in controller variables
		elemCount = count;
		initialized = 1;
	}

	if (elemCount != iter.exactCount())
	{
		MGlobal::displayError("Mismatch between saved bind index and current mesh vertex, please rebind");
		return MS::kSuccess;
	}

	// Loop through the size
	MVector delta, current, target;

	for (int i = 0; i < elemCount; i++)
	{
		// Compute the delta from the input position and final position
		delta = driverPoint[bindArray[i]] - pos[i];
		// Scale the delta by the envelope
		pos[i] = pos[i] + (delta * envelopeV);
	}

	iter.setAllPositions(pos);
	
	return MS::kSuccess;
}

void SnapDeformer::initData(MObject& driverMesh, MPointArray& deformedPoints, MIntArray& bindArray, MObject& attribute)
{
	// Get the amount of points on the mesh and set the array size
	int count = deformedPoints.length();
	bindArray.setLength(count);

	// Declare the function sets and store the points in driverPoints
	MFnMesh meshFn(driverMesh);
	MItMeshPolygon faceIter(driverMesh);
	MPointArray driverPoints;
	meshFn.getPoints(driverPoints, MSpace::kWorld);

	// Declare variables ahead of the for loop
	MPlug attrPlug(thisMObject(), attribute);
	MDataHandle handle;

	MPoint closest;
	int closestFace, oldIndex, minId;
	unsigned int v;
	MIntArray vertices;
	double minDistance, distance;
	MVector base, end, vector;

	// Iterate through the points on the deformer mesh
	for (int i = 0; i < count; i++)
	{
		// Get the closest face
		meshFn.getClosestPoint(deformedPoints[i], closest, MSpace::kWorld, &closestFace);

		// Find the closest vertex
		faceIter.setIndex(closestFace, oldIndex);
		vertices.setLength(0);
		faceIter.getVertices(vertices);

		// Convert the MPoint to an MVector
		base = MVector(closest);
		minDistance = BIG_DIST;

		// Loop through all the faces
		for (v = 0; v < vertices.length(); v++)
		{
			// Get the end of the vector
			end = MVector(driverPoints[vertices[v]]);
			// Build the vector
			vector = end - base;
			// Get the vector length
			distance = vector.length();
			
			// Check if the distance is the shortest
			if (distance < minDistance)
			{
				minDistance = distance;
				minId = vertices[v];
			}
		}

		// Store the results both in the array and in the attribute
		bindArray[i] = int(minId);

		// Ensure we have the attribute indexes
		attrPlug.selectAncestorLogicalIndex(i, attribute);
		attrPlug.getValue(handle);
		attrPlug.setValue(minId);
	}

	// Set controller variable
	initialized = 1;
	elemCount = count;
}

MStatus SnapDeformer::shouldSave(const MPlug& plug, bool& result)
{
	MStatus status = MS::kSuccess;
	result = true;
	return status;
}

void SnapDeformer::ensureIndexes(MObject& attribute, int indexSize)
{
	MPlug attrPlug(thisMObject(), attribute);
	MDataHandle handle;

	for (int i = 0; i < indexSize; i++)
	{
		attrPlug.selectAncestorLogicalIndex(i, attribute);
		attrPlug.getValue(handle);
	}

}
