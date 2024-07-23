#pragma once

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MDistance.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>

class StretchyIK : public MPxNode
{
public:
	// IK Chain Info
	static MObject inRootMatrixAttr;
	static MObject inPoleVectorMatrixAttr;
	static MObject inControlMatrixAttr;
	static MObject inUpperLengthAttr;
	static MObject inLowerLengthAttr;
	static MObject inGlobalScaleAttr;

	// Features
	static MObject inSlideAttr;
	static MObject inStretchAttr;
	static MObject inPoleVectorLockAttr;

	// Outputs
	static MObject outUpperLengthAttr;
	static MObject outLowerLengthAttr;

	static MTypeId typeID;
	static void* creator();
	static MStatus initialize();
	MStatus compute(const MPlug& plug, MDataBlock& block) override;
};