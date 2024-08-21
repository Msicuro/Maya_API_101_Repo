#pragma once
#include <maya/MPxDeformerNode.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>


class SnapDeformer : public MPxDeformerNode
{
	// Methods
public:
	// General Node setup
	static void* creator();
	static MStatus initialize();

	// Constructor
	SnapDeformer();

	// deform replaces the "compute" function in other nodes
	virtual MStatus deform(MDataBlock& data, MItGeometry& iter, const MMatrix& matrix, unsigned int mIndex);
	virtual MStatus shouldSave(const MPlug& plug, bool& result);

private:
	void initData(MObject& driverMesh, MPointArray& deformedPoints, MIntArray& bindArray, MObject& attribute);
	void ensureIndexes(MObject& attribute, int indexSize);

	// Variables
public:
	// General Node setup
	static MTypeId typeId;

	// Node attributes
	static MObject referenceMesh;
	static MObject rebind;
	static MObject driverMesh;
	static MObject idAssociation;

private:
	unsigned int elemCount;
	MIntArray bindArray;
	bool initialized;

};