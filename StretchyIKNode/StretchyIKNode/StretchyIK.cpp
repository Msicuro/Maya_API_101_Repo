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

MTypeId StretchyIK::typeID(0x80005);

// IK Chain Info
MObject StretchyIK::inRootMatrixAttr;
MObject StretchyIK::inPoleVectorMatrixAttr;
MObject StretchyIK::inControlMatrixAttr;
MObject StretchyIK::inUpperLengthAttr;
MObject StretchyIK::inLowerLengthAttr;
MObject StretchyIK::inGlobalScaleAttr;
// Homework
MObject StretchyIK::inUpperScaleAttr;
MObject StretchyIK::inLowerScaleAttr;

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

	inPoleVectorMatrixAttr = matFn.create("inPoleVector", "inPoleVector", MFnMatrixAttribute::kDouble, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inPoleVectorMatrixAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

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

	// Homework
	inUpperScaleAttr = numericFn.create("inUpperScale", "inUpperScale", MFnNumericData::kDouble, 1.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setMin(0.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inUpperScaleAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	inLowerScaleAttr = numericFn.create("inLowerScale", "inLowerScale", MFnNumericData::kDouble, 1.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numericFn.setMin(0.0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inLowerScaleAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	// Outputs
	outUpperLengthAttr = numericFn.create("outUpperLength", "outUpperLength", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	outLowerLengthAttr = numericFn.create("outLowerLength", "outLowerLength", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	// IK Chain Info
	status = attributeAffects(inRootMatrixAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inRootMatrixAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inPoleVectorMatrixAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inPoleVectorMatrixAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inControlMatrixAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inControlMatrixAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inUpperLengthAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inUpperLengthAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inLowerLengthAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inLowerLengthAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inGlobalScaleAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inGlobalScaleAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	// Homework
	status = attributeAffects(inUpperScaleAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inUpperScaleAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inLowerScaleAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inLowerScaleAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	// Features
	status = attributeAffects(inSlideAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inSlideAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inStretchAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inStretchAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inPoleVectorLockAttr, outUpperLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inPoleVectorLockAttr, outLowerLengthAttr); CHECK_MSTATUS_AND_RETURN_IT(status);

	return status;
}


MStatus StretchyIK::compute(const MPlug& plug, MDataBlock& block)
{
	// Check if the plug is either of the two output attributes
	if (plug != outUpperLengthAttr && plug != outLowerLengthAttr)
	{
		return MS::kInvalidParameter;
	}

	// Get the chain length
	double upperLength = block.inputValue(inUpperLengthAttr).asDistance().asCentimeters();
	double lowerLength = block.inputValue(inLowerLengthAttr).asDistance().asCentimeters();

	// HOMEWORK - UPPER AND LOWER SCALE
	// Reset the initial length with the upper and lower scale attributes
	double upperScaleFactor = block.inputValue(inUpperScaleAttr).asDouble();
	double lowerScaleFactor = block.inputValue(inLowerScaleAttr).asDouble();
	// Prevent 0.0 length bones
	if (upperScaleFactor < 0.0001) { upperScaleFactor = 0.0001; }
	if (lowerScaleFactor < 0.0001) { lowerScaleFactor = 0.0001; }
	// Multiply by the new scale factors
	upperLength *= upperScaleFactor;
	lowerLength *= lowerScaleFactor;

	// Compute the full chain length
	double chainLength = upperLength + lowerLength;

	/// INPUT READING ///
	// Get the positions of the incoming matrices
	// Get the matrix
	MMatrix rootM = block.inputValue(inRootMatrixAttr).asMatrix();
	MMatrix poleVectorM = block.inputValue(inPoleVectorMatrixAttr).asMatrix();
	MMatrix controlM = block.inputValue(inControlMatrixAttr).asMatrix();

	// Get the world space translations from the matrix
	MVector root(rootM[3]);
	MVector poleVector(poleVectorM[3]);
	MVector control(controlM[3]);
	
	// Factor in the Global Scale so all the math is in "pre-scale" space
	// Make sure the scale is non 0 to avoid divide by 0 errors
	double globalScale = block.inputValue(inGlobalScaleAttr).asDouble();
	if (globalScale < 0.00001) { globalScale = 0.00001; }
	
	root /= globalScale;
	poleVector /= globalScale;
	control /= globalScale;

	/// Slide ///
	// Slide is implemented before the stretch to ensure the chain length does not change

	// Make one bone closer to the full chain length, and the other bone closer to 0.0 using the lerp function
	double slide = block.inputValue(inSlideAttr).asDouble();

	// Limit the sliding values to avoid flipping
	if (slide < -0.999) { slide = -0.999; }
	if (slide > 0.999) { slide = 0.999; }

	// Slide is between 1- and 1, if slide is less than 0, blend the upper bone towards 0
	if (slide < 0.0)
	{
		upperLength = lerp(upperLength, 0.0, -slide);
		lowerLength = lerp(lowerLength, chainLength, -slide);
	}
	// Slide is between 1- and 1, if slide is greater than 0, blend the lower bone towards 0
	else
	{
		upperLength = lerp(upperLength, chainLength, slide);
		lowerLength = lerp(lowerLength, 0.0, slide);
	}


	/// STRETCH
	// If the ik handle passes a certain point, use the distance from the ik handle as the new chain length to stretch
	MVector ikhDelta = control - root;
	double desiredChainLength = ikhDelta.length();
	double stretch = block.inputValue(inStretchAttr).asDouble();
	double scaleFactor = 1.0;

	if (desiredChainLength > chainLength)
	{
		scaleFactor = desiredChainLength / chainLength; // This value is greater than 1
		// Interpolate from a scale of 1
		scaleFactor = lerp(1.0, scaleFactor, stretch);
		// Will be overridden if soft ik is triggered
		upperLength *= scaleFactor;
		lowerLength *= scaleFactor;
	}


	// POLE VECTOR LOCK
	// Blend the joint lengths so the middle joint is exactly at the pole vector
	double poleVectorLock = block.inputValue(inPoleVectorLockAttr).asDouble();

	if (poleVectorLock > 0.0)
	{
		// The desired lengths are the distances between the points
		double desiredUpperLength = (poleVector - root).length();
		double desiredLowerLength = (control - poleVector).length();

		// Use the pole vector lock value to blend from the original length to the new length
		upperLength = lerp(upperLength, desiredUpperLength, poleVectorLock);
		lowerLength = lerp(lowerLength, desiredLowerLength, poleVectorLock);
	}


	// SET OUTPUTS
	block.outputValue(outUpperLengthAttr).setDouble(upperLength);
	block.outputValue(outLowerLengthAttr).setDouble(lowerLength);
	block.setClean(outUpperLengthAttr);
	block.setClean(outLowerLengthAttr);

	return MS::kSuccess;
}