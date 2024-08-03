#pragma once

#include "aimNode.h"

MTypeId AimNode::typeID(0x90005);

// Attributes
MObject AimNode::inputTranslateX;
MObject AimNode::inputTranslateY;
MObject AimNode::inputTranslateZ;
MObject AimNode::inputTranslates;
MObject AimNode::outputRotateX;
MObject AimNode::outputRotateY;
MObject AimNode::outputRotateZ;
MObject AimNode::outputRotates;
MObject AimNode::inputDriverMatrix;
MObject AimNode::inputUpVectorMatrix;
// Enum Attributes
MObject AimNode::aimVector;
MObject AimNode::aimVectorX;
MObject AimNode::aimVectorY;
MObject AimNode::aimVectorZ;

void* AimNode::creator() { return new AimNode; }

MStatus AimNode::initialize()
{
	MStatus status;
	MFnMatrixAttribute matFn;
	MFnUnitAttribute unitFn;
	MFnNumericAttribute numericFn;
	MFnTypedAttribute typedFn;
	MFnCompoundAttribute compFn;
	MFnEnumAttribute enumFn;

	// Create the driver matrix attribute
	inputDriverMatrix = matFn.create("driverMatrix", "driverMatrix", MFnMatrixAttribute::kDouble, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inputDriverMatrix); CHECK_MSTATUS_AND_RETURN_IT(status);
	
	// Create the up vector matrix attribute
	inputUpVectorMatrix = matFn.create("upVectorMatrix", "upVectorMatrix", MFnMatrixAttribute::kDouble, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inputUpVectorMatrix); CHECK_MSTATUS_AND_RETURN_IT(status);
	
	// Create the driver translate attributes
	inputTranslateX = numericFn.create("translateX", "translateX", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setWritable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inputTranslateX); CHECK_MSTATUS_AND_RETURN_IT(status);

	inputTranslateY = numericFn.create("translateY", "translateY", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setWritable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inputTranslateY); CHECK_MSTATUS_AND_RETURN_IT(status);

	inputTranslateZ = numericFn.create("translateZ", "translateZ", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setWritable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inputTranslateZ); CHECK_MSTATUS_AND_RETURN_IT(status);

	inputTranslates = compFn.create("inputTranslates", "inputTranslates", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compFn.addChild(inputTranslateX); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compFn.addChild(inputTranslateY); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compFn.addChild(inputTranslateZ); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compFn.setWritable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inputTranslates); CHECK_MSTATUS_AND_RETURN_IT(status);

	// Enum attribute to determine final aim vector
	aimVector = enumFn.create("aimVector", "aimVector", 0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = enumFn.addField("X", 0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = enumFn.addField("Y", 1); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = enumFn.addField("Z", 2); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(aimVector); CHECK_MSTATUS_AND_RETURN_IT(status);

	// Create the driver output rotation attributes
	outputRotateX = unitFn.create("rotateX", "rotateX", MFnUnitAttribute::kAngle, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputRotateX); CHECK_MSTATUS_AND_RETURN_IT(status);

	outputRotateY = unitFn.create("rotateY", "rotateY", MFnUnitAttribute::kAngle, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputRotateY); CHECK_MSTATUS_AND_RETURN_IT(status);

	outputRotateZ = unitFn.create("rotateZ", "rotateZ", MFnUnitAttribute::kAngle, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputRotateZ); CHECK_MSTATUS_AND_RETURN_IT(status);

	outputRotates = compFn.create("outputRotates", "outputRotates", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compFn.addChild(outputRotateX); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compFn.addChild(outputRotateY); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compFn.addChild(outputRotateZ); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputRotates); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(inputDriverMatrix, outputRotates); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inputUpVectorMatrix, outputRotates); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inputTranslates, outputRotates); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(aimVector, outputRotates); CHECK_MSTATUS_AND_RETURN_IT(status);

	return status;
}


MStatus AimNode::compute(const MPlug& plug, MDataBlock& data)
{
	if (plug != outputRotates && plug != outputRotateX
		&& plug != outputRotateY && plug != outputRotateZ)
	{
		return MS::kInvalidParameter;
	}

	// Extract input data
	MMatrix driverMatrix = data.inputValue(inputDriverMatrix).asMatrix();
	MMatrix upVectorMatrix = data.inputValue(inputUpVectorMatrix).asMatrix();
	MVector translateVector = data.inputValue(inputTranslates).asVector();

	// Extract the positions from the matrices
	MVector driverMatrixPos = { driverMatrix[3][0], driverMatrix[3][1], driverMatrix[3][2] };
	MVector upVectorMatrixPos = { upVectorMatrix[3][0], upVectorMatrix[3][1], upVectorMatrix[3][2] };

	// Compute the vectors from the driven object
	MVector upVector = upVectorMatrixPos - translateVector;
	MVector driverVector = driverMatrixPos - translateVector;

	// Normalize the vectors
	MVector upVectorNorm = upVector.normal();
	MVector driverVectorNorm = driverVector.normal();

	// Compute a perpendicular vector to the upVector and Driver positions
	MVector cross = driverVectorNorm ^ upVectorNorm;
	MVector finalUpVector = cross ^ driverVectorNorm;

	// Create a rotational Matrix
	double newMatrix[4][4] = {
		{driverVectorNorm.x, driverVectorNorm.y, driverVectorNorm.z, 0.0},
		{finalUpVector.x, finalUpVector.y, finalUpVector.z, 0.0},
		{cross.x, cross.y, cross.z, 0.0},
		{translateVector.x, translateVector.y, translateVector.z, 0.0}
	};

	// Extract Euler rotations
	MMatrix rotMatrix(newMatrix);
	MTransformationMatrix matrixFn(rotMatrix);
	MEulerRotation euler = matrixFn.eulerRotation();

	// Set and clean output data
	data.outputValue(outputRotates).set(euler.x, euler.y, euler.z);
	data.outputValue(outputRotates).setClean();

	return MS::kSuccess;
}
