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

void* AimNode::creator() { return new AimNode; }

MStatus AimNode::initialize()
{
	MStatus status;
	MFnMatrixAttribute matFn;
	MFnUnitAttribute unitFn;
	MFnNumericAttribute numericFn;
	MFnTypedAttribute typedFn;
	MFnCompoundAttribute compFn;

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

	return status;
}


MStatus AimNode::compute(const MPlug& plug, MDataBlock& block)
{
	if (plug != outputRotates && plug != outputRotateX
		&& plug != outputRotateY && plug != outputRotateZ)
	{
		return MS::kInvalidParameter;
	}


	
	return MS::kSuccess;
}
