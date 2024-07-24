#pragma once

#include "StretchyIK.h"

double lerp(double a, double b, double t)
{
	/*
	* Linear Interpolate:
	* Blend value a towards value b using t, which is a value between 0.0 and 1.0
	* If t is 0, then it is 0% blended and will return a
	* If t is 1, we are 100% blended and will return b
	*/
	return a + (b - a) * t;
}

MTypeId StretchyIK::typeID = 0x80005;

// IK Chain Info
MObject StretchyIK::inRootMatrixAttr;
MObject StretchyIK::inPoleVectorMatrixAttr;
MObject StretchyIK::inControlMatrixAttr;
MObject StretchyIK::inUpperLengthAttr;
MObject StretchyIK::inLowerLengthAttr;
MObject StretchyIK::inGlobalScaleAttr;

// Features
MObject StretchyIK::inSlideAttr;
MObject StretchyIK::inStretchAttr;
MObject StretchyIK::inPoleVectorLockAttr;

// Outputs
MObject StretchyIK::outUpperLengthAttr;
MObject StretchyIK::outLowerLengthAttr;

void* StretchyIK::creator() { return new StretchyIK; }


MStatus StretchyIK::initialize()
{
	MStatus status;
	MFnMatrixAttribute matFn;
	MFnUnitAttribute unitFn;
	MFnNumericAttribute numericFn;

	// IK Chain Info
	inRootMatrixAttr = matFn.create("inRoot", "inRoot", MFnMatrixAttribute::kDouble, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inRootMatrixAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	inPoleVectorLockAttr = matFn.create("inPoleVector", "inPoleVector", MFnMatrixAttribute::kDouble, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inPoleVectorLockAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	inControlMatrixAttr = matFn.create("inControl", "inControl", MFnMatrixAttribute::kDouble, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inControlMatrixAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	inUpperLengthAttr = unitFn.create("inUpperLength", "inUpperLength", MFnUnitAttribute::kDistance, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setMin(0.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	inLowerLengthAttr = unitFn.create("inLowerLength", "inLowerLength", MFnUnitAttribute::kDistance, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setMin(0.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	inGlobalScaleAttr = numericFn.create("inGlobalScale", "inGlobalScale", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setMin(0.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inGlobalScaleAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	inSlideAttr = numericFn.create("inSlide", "inSlide", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setMin(-1.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setMax(1.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inSlideAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	inStretchAttr = numericFn.create("inStretch", "inStretch", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setMin(0.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setMax(1.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inStretchAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	inPoleVectorLockAttr = numericFn.create("inPoleVectorLock", "inPoleVectorLock", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setMin(0.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setMax(1.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inPoleVectorLockAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	outUpperLengthAttr = numericFn.create("outUpperLength", "outUpperLength", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	outLowerLengthAttr = numericFn.create("outLowerLength", "outLowerLength", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
}