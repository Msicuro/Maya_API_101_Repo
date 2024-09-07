#pragma once

#include <maya/MTypeId.h>
#include <maya/MPxDeformerNode.h>
#include <maya/MMatrix.h>
#include <maya/MDataBlock.h>
#include <maya/MItGeometry.h>


class RelaxNode : MPxDeformerNode
{
	// Methods
public:
	RelaxNode();
	static void* creator();
	static MStatus initialize();
	virtual MStatus deform(MDataBlock& data, MItGeometry& iter, const MMatrix& matrix, unsigned int mIndex);

private:

	// Variables
public:
	static MTypeId typeId;
	static MObject amount;
	static MObject inputMesh;
};