#pragma once

#include <maya/MPxNode.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MMatrix.h>
#include <maya/MTypeId.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MVector.h>

class ChainOnPath : public MPxNode
{
	// Methods
public:
	ChainOnPath();
	static void* creator();
	static MStatus initialize();
	MStatus compute(const MPlug& plug, MDataBlock& block) override;
	MStatus shouldSave(const MPlug& plug, bool& result) override;
	MStatus setDependentsDirty(const MPlug& plug, MPlugArray& plugArray) override;

	//Variables
public:
	static MTypeId typeId;

	static MObject inCurve;
	static MObject inSurface;
	static MObject numOfOutputs;
	static MObject paramValues;

	static MObject outputTranslateX;
	static MObject outputTranslateY;
	static MObject outputTranslateZ;
	static MObject outputTranslate;
	static MObject outputRotateX;
	static MObject outputRotateY;
	static MObject outputRotateZ;
	static MObject outputRotate;

	static MObject paramAs;
	static MObject defaultCurveLength;
	static MObject stretchToFitLength;
	static MObject reverseRoot;

private:
	bool initParams;
	bool initCurve;
	bool initSurface;
	MDoubleArray inParams;
	MFnNurbsCurve curveFn;
	MFnNurbsSurface surfaceFn;
	double curveLen;
	int paramsV;
	double uMin, uMax, vMin, vMax;
	MTransformationMatrix::RotationOrder rotOrder;
	MMatrix rotMatrix;
	MTransformationMatrix matrixFn;
	double outR[3];
	MVector tan;
	MVector normal;
	MVector cross;
	double ratio;
	double defaultCurveLengthV;
	double uVal;
	double inParam;
	double inParamStretched;
	double inParamReversed;
	MPoint outP;
	double reversed;
	double finalParam;
	MObject inCurveV;
	MObject inSurfaceV;
};