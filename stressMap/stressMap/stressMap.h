#pragma once

#include <maya/MDoubleArray.h>
#include <maya/mpointArray.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MTypeId.h>
#include <vector>

struct StressPoint
{
	MIntArray neighbors;
	MDoubleArray neighborsStress;
	int sizes;
};

class StressMap final : public MPxLocatorNode
{
	// Methods
public:

	// General Node Setup
	static void* creator();
	static MStatus initialize();
	MStatus compute(const MPlug& plug, MDataBlock& data) override;

	StressMap();
		
	void draw(M3dView&, const MDagPath&, M3dView::DisplayStyle, M3dView::DisplayStatus) override;
	bool isBounded() const override;

	void buildConnectionTree(std::vector<StressPoint>& pointTree, MDoubleArray& stressMapValues, MObject& referenceMesg);

	// Variables
public:
	static MTypeId typeId;
	static MObject fakeOut;
	static MObject drawIt;
	static MObject inputMesh;
	static MObject referenceMesh;
	std::vector<StressPoint> pointStoredTree;

	MDoubleArray stressMapValues;
	static const MString kDrawDbClassification;
	static MObject output;
	static MObject multiplier;
	static MObject clampMax;
	static MObject normalize;
	static MObject squashColor;
	static MObject stretchColor;
	static MObject intensity;

	int firstRun;
	MPointArray referencePos;
	MPointArray inputPos;
};