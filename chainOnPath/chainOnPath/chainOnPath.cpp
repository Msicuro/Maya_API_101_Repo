#pragma once

#include "chainOnPath.h"

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnData.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>

#include <string>

const double toDeg = 57.295779513082320876798154814105;

MTypeId ChainOnPath::typeId(0x90013);

MObject ChainOnPath::inCurve;
MObject ChainOnPath::inSurface;

MObject ChainOnPath::numOfOutputs;
MObject ChainOnPath::paramValues;
MObject ChainOnPath::outputTranslateX;
MObject ChainOnPath::outputTranslateY;
MObject ChainOnPath::outputTranslateZ;
MObject ChainOnPath::outputTranslate;
MObject ChainOnPath::outputRotateX;
MObject ChainOnPath::outputRotateY;
MObject ChainOnPath::outputRotateZ;
MObject ChainOnPath::outputRotate;
MObject ChainOnPath::paramAs;
MObject ChainOnPath::stretchToFitLength;
MObject ChainOnPath::defaultCurveLength;
MObject ChainOnPath::reverseRoot;

ChainOnPath::ChainOnPath()
{
	initParams = 0;
	initCurve = 0;
	initSurface = 0;
	paramsV = 0;
	curveLen = 0;
	rotOrder = MTransformationMatrix::kXYZ;
	uVal = 0.0;
	inParam = 0;
	inParamStretched = 0;
	inParamReversed = 0;
	MPoint outP;
	reversed = 0.0;
	finalParam = 0.0;
}

void* ChainOnPath::creator() { return new ChainOnPath(); }

MStatus ChainOnPath::initialize()
{
	MStatus status;
	MFnTypedAttribute typeFn;
	MFnCompoundAttribute compoundFn;
	MFnNumericAttribute numFn;
	MFnMatrixAttribute matrixFn;
	MFnUnitAttribute unitFn;
	MFnEnumAttribute enumFn;

	// Inputs
	inCurve = typeFn.create("inCurve", "inCurve", MFnData::kNurbsCurve, MObject::kNullObj, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inCurve); CHECK_MSTATUS_AND_RETURN_IT(status);

	inSurface = typeFn.create("inSurface", "inSurface", MFnData::kNurbsSurface, MObject::kNullObj, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inSurface);

	numOfOutputs = numFn.create("numberOfOutputs", "numberOfOutputs", MFnNumericData::kInt, 1, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setMin(0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(numOfOutputs); CHECK_MSTATUS_AND_RETURN_IT(status);

	paramValues = numFn.create("parameterValues", "parameterValues", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setArray(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(paramValues); CHECK_MSTATUS_AND_RETURN_IT(status);

	defaultCurveLength = numFn.create("defaultCurveLength", "defaultCurveLength", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(defaultCurveLength); CHECK_MSTATUS_AND_RETURN_IT(status);

	stretchToFitLength = numFn.create("stretchToFitLength", "stretchToFitLength", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setMin(0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setMax(1); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(stretchToFitLength); CHECK_MSTATUS_AND_RETURN_IT(status);

	reverseRoot = numFn.create("reverseRoot", "reverseRoot", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setMin(0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setMax(1); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(reverseRoot); CHECK_MSTATUS_AND_RETURN_IT(status);

	paramAs = enumFn.create("paramAs", "paramAs", 0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = enumFn.addField("U", 0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = enumFn.addField("Len", 1); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = enumFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = enumFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(paramAs); CHECK_MSTATUS_AND_RETURN_IT(status);

	//Outputs
	outputTranslateX = numFn.create("outputTranslateX", "outputTranslateX", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setWritable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputTranslateX); CHECK_MSTATUS_AND_RETURN_IT(status);

	outputTranslateY = numFn.create("outputTranslateY", "outputTranslateY", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setWritable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputTranslateY); CHECK_MSTATUS_AND_RETURN_IT(status);

	outputTranslateZ = numFn.create("outputTranslateZ", "outputTranslateZ", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setWritable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputTranslateZ); CHECK_MSTATUS_AND_RETURN_IT(status);

	outputTranslate = compoundFn.create("outputTranslate", "outputTranslate", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.addChild(outputTranslateX); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.addChild(outputTranslateY); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.addChild(outputTranslateZ); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.setStorable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.setKeyable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.setWritable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.setArray(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputTranslate); CHECK_MSTATUS_AND_RETURN_IT(status);
	
	outputRotateX = unitFn.create("outputRotateX", "outputRotateX", MFnUnitAttribute::kAngle, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setKeyable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setStorable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setWritable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputRotateX); CHECK_MSTATUS_AND_RETURN_IT(status);

	outputRotateY = unitFn.create("outputRotateY", "outputRotateY", MFnUnitAttribute::kAngle, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setKeyable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setStorable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setWritable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputRotateY); CHECK_MSTATUS_AND_RETURN_IT(status);

	outputRotateZ = unitFn.create("outputRotateZ", "outputRotateZ", MFnUnitAttribute::kAngle, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setKeyable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setStorable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = unitFn.setWritable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputRotateZ); CHECK_MSTATUS_AND_RETURN_IT(status);

	outputRotate = compoundFn.create("outputRotate", "outputRotate", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.addChild(outputRotateX); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.addChild(outputRotateY); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.addChild(outputRotateZ); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.setStorable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.setKeyable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.setWritable(false); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = compoundFn.setArray(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(outputRotate); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(inCurve, outputTranslate); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inCurve, outputRotate); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(inSurface, outputTranslate); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(inSurface, outputRotate); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(numOfOutputs, outputTranslate); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(numOfOutputs, outputRotate); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(paramValues, outputTranslate); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(paramValues, outputRotate); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(defaultCurveLength, outputTranslate); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(defaultCurveLength, outputRotate); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(stretchToFitLength, outputTranslate); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(stretchToFitLength, outputRotate); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(reverseRoot, outputTranslate); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(reverseRoot, outputRotate); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(paramAs, outputTranslate); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(paramAs, outputRotate); CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}