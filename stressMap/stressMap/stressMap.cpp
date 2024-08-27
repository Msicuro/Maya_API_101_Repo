#pragma once

#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MIntArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MPlug.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>

#include "stressMap.h"

MTypeId StressMap::typeId(0x90000);
const MString StressMap::kDrawDbClassification("drawdb/geometry/stressMapDO");

// Needed attributes
MObject StressMap::fakeOut;
MObject StressMap::drawIt;
MObject StressMap::inputMesh;
MObject StressMap::referenceMesh;
MObject StressMap::output;
MObject StressMap::multiplier;
MObject StressMap::clampMax;
MObject StressMap::normalize;
MObject StressMap::squashColor;
MObject StressMap::stretchColor;
MObject StressMap::intensity;

// NOTE: Read more about member initializer lists (using the colon with a constructor definition)
StressMap::StressMap() : firstRun(0) {}

void* StressMap::creator() { return new StressMap(); }

MStatus StressMap::initialize()
{
	MStatus status;
	
	// Declare the needed attribute function sets
	MFnEnumAttribute enumFn;
	MFnMatrixAttribute matrixFn;
	MFnNumericAttribute numFn;
	MFnCompoundAttribute compA;
	MFnTypedAttribute typeFn;

	// NOTE: Double check if having kNullObj in the create function in order to include the MStatus is okay
	inputMesh = typeFn.create("inputMesh", "inputMesh", MFnData::kMesh, MObject::kNullObj, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(inputMesh); CHECK_MSTATUS_AND_RETURN_IT(status);

	referenceMesh = typeFn.create("referenceMesh", "referenceMesh", MFnData::kMesh, MObject::kNullObj, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = typeFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(referenceMesh); CHECK_MSTATUS_AND_RETURN_IT(status);

	drawIt = numFn.create("drawIt", "drawIt", MFnNumericData::kBoolean, 1.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(drawIt); CHECK_MSTATUS_AND_RETURN_IT(status);

	clampMax = numFn.create("clampMax", "clampMax", MFnNumericData::kDouble, 1.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setMin(0); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setMax(10); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(clampMax); CHECK_MSTATUS_AND_RETURN_IT(status);

	multiplier = numFn.create("multiplier", "multiplier", MFnNumericData::kDouble, 1.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(multiplier); CHECK_MSTATUS_AND_RETURN_IT(status);

	normalize = numFn.create("normalize", "normalize", MFnNumericData::kBoolean, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(normalize); CHECK_MSTATUS_AND_RETURN_IT(status);

	squashColor = numFn.create("squashColor", "squashColor", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setDefault(0.0f, 1.0f, 0.0f); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setReadable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setWritable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(squashColor); CHECK_MSTATUS_AND_RETURN_IT(status);

	stretchColor = numFn.create("stretchColor", "stretchColor", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setDefault(1.0f, 0.0f, 0.0f); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setReadable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setWritable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(stretchColor); CHECK_MSTATUS_AND_RETURN_IT(status);

	intensity = numFn.create("intensity", "intensity", MFnNumericData::kDouble, 0.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(intensity); CHECK_MSTATUS_AND_RETURN_IT(status);

	fakeOut = numFn.create("fakeOutput", "fakeOutput", MFnNumericData::kBoolean, 1, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);

	output = numFn.create("output", "output", MFnData::kDoubleArray);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setWritable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(output); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(inputMesh, output); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(referenceMesh, output); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(clampMax, output); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(multiplier, output); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(squashColor, output); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(stretchColor, output); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(intensity, output); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(normalize, output); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = attributeAffects(inputMesh, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(referenceMesh, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(clampMax, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(multiplier, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(squashColor, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(stretchColor, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(intensity, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = attributeAffects(normalize, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);

	// Attribute Editor template
	MString stresTemplateNode(
		MString() + "global proc AEstressMapTemplate(string &nodeName) \n" +
		"{editorTemplate -beginScrollLayout; \n" +
		"editorTemplate -beginLayout \"Setting Attributes\" -collapse 0; \n" +
		"editorTemplate -addControl \"normalize\"; \n" +
		"editorTemplate -addControl \"clampMax\"; \n" +
		"editorTemplate -addControl \"multiplier\"; \n" +
		"editorTemplate -endLayout; \n" +

		"editorTemplate -beginLayout \"Drawing Attributes\" -collapse 0; \n" +
		"editorTemplate -addControl \"drawIt\"; \n" +
		"editorTemplate -addControl \"intensity\"; \n" +
		"editorTemplate -addControl \"squashColor\"; \n" +
		"editorTemplate -addControl \"stretchColor\"; \n" +
		"editorTemplate -endLayout; \n" +

		"editorTemplate -addExtraControls; \n" +
		"editorTemplate -endScrollLayout; \n");

	MGlobal::executeCommand(stresTemplateNode);

	return MS::kSuccess;
}