#pragma once

#include <maya/MGlobal.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MPlug.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MDataHandle.h>
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MDistance.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>

class AimNode : public MPxNode
{
public:
	static MTypeId typeID;
	static void* creator();
	static MStatus initialize();
	// Use override as sanity check when overriding Maya's native compute method
	MStatus compute(const MPlug& plug, MDataBlock& block) override;

};