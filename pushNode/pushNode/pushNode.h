#pragma once

#include <maya/MPxDeformerNode.h>
#include <maya/MMatrix.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MEventMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>

#include <vector>
#include <set>

class PushNode : MPxDeformerNode
{
	// Methods
public:
	PushNode();
	static void* creator();
	static MStatus initialize();
	virtual MStatus deform(MDataBlock& data, MItGeometry& iter, const MMatrix& matrix, unsigned int mIndex);

	// Variables
public:
	static MTypeId typeId;
	static MObject stressMap;
	static MObject useStress;
	static MObject amount;
};