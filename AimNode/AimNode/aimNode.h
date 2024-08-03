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
#include <maya/MFnEnumAttribute.h>

class AimNode : public MPxNode
{
public:
	// General Node Setup
	static MTypeId typeID;
	static void* creator();
	static MStatus initialize();
	// Use override as sanity check when overriding Maya's native compute method
	MStatus compute(const MPlug& plug, MDataBlock& block) override;

	// Input Translates for driven object
	static MObject inputTranslateX;
	static MObject inputTranslateY;
	static MObject inputTranslateZ;
	static MObject inputTranslates;

	// Output Rotates for driven object
	static MObject outputRotateX;
	static MObject outputRotateY;
	static MObject outputRotateZ;
	static MObject outputRotates;

	// Input matrix for the driver of the constraint
	static MObject inputDriverMatrix;

	// Input matrix for the up vector of the constraint
	static MObject inputUpVectorMatrix;

	// Driven object aim vector to be chosen from an enum
	static MObject aimVector;
	static MObject aimVectorX;
	static MObject aimVectorY;
	static MObject aimVectorZ;

	// Function to change order of Matrix array
	static void changeOrder(double array[], int a, int b, int c);
};