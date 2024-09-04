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

	squashColor = numFn.createColor("squashColor", "squashColor", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setDefault(0.0f, 1.0f, 0.0f); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setReadable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setWritable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(squashColor); CHECK_MSTATUS_AND_RETURN_IT(status);

	stretchColor = numFn.createColor("stretchColor", "stretchColor", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
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

	fakeOut = numFn.create("fakeOutput", "fakeOutput", MFnNumericData::kBoolean, 1.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setKeyable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = numFn.setStorable(true); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);

	output = typeFn.create("output", "output", MFnData::kDoubleArray);
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

	//status = attributeAffects(inputMesh, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	//status = attributeAffects(referenceMesh, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	//status = attributeAffects(clampMax, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	//status = attributeAffects(multiplier, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	//status = attributeAffects(squashColor, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	//status = attributeAffects(stretchColor, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	//status = attributeAffects(intensity, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);
	//status = attributeAffects(normalize, fakeOut); CHECK_MSTATUS_AND_RETURN_IT(status);

	// Attribute Editor template
	MString stresTemplateNode(
		MString() + "global proc AEstressMapTemplate(string $nodeName) \n" +
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
		"editorTemplate -endScrollLayout;} \n");

	MGlobal::executeCommand(stresTemplateNode);

	return MS::kSuccess;
}

bool StressMap::isBounded() const { return false; }

MStatus StressMap::compute(const MPlug& plug, MDataBlock& data)
{
	// Check if the meshes are connected
	MPlug inputMeshP(thisMObject(), inputMesh);
	if (!inputMeshP.isConnected()) { return MS::kNotImplemented; }

	MPlug referenceMeshP(thisMObject(), referenceMesh);
	if (!referenceMeshP.isConnected()) { return MS::kNotImplemented; }

	// Gather data
	MObject referenceMeshV = data.inputValue(referenceMesh).asMesh();
	MObject inputMeshV = data.inputValue(inputMesh).asMesh();
	const double multiplierV = data.inputValue(multiplier).asDouble();
	const double clampItMaxV = data.inputValue(clampMax).asDouble();
	const bool normalizeV = data.inputValue(normalize).asBool();

	// Build a table for the neighbors of a vertex and cache the mesh topology
	if ((firstRun == 0) || (pointStoredTree.empty() == 1) || (stressMapValues.length() == 0))
	{
		buildConnectionTree(pointStoredTree, stressMapValues, referenceMeshV);
	}

	// Get input points
	MFnMesh inMeshFn(inputMeshV);
	inMeshFn.getPoints(inputPos, MSpace::kObject);

	// Get reference points
	MFnMesh refMeshFn(referenceMeshV);
	refMeshFn.getPoints(referencePos, MSpace::kObject);

	const unsigned int intLength = inputPos.length();

	// Check the amount of points on the input mesh
	if (intLength != referencePos.length())
	{
		MGlobal::displayError("Unequal number of points between input mesh and reference mesh");
		return MS::kSuccess;
	}

	// Check if the amount of stored points matches the input points
	if (pointStoredTree.size() != intLength)
	{
		MGlobal::displayError("Unequal number of points between reference and main data, try to rebuild");
		return MS::kSuccess;
	}

	double value = 0;
	MVector storedLen, currentLen;

	// Loop through every vertex to calculate the stress data
	for (int v = 0; v < intLength; v++)
	{
		value = 0;

		// Loop through all the vertices connected to the current vertex
		for (int n = 0; n < pointStoredTree[v].sizes; n++)
		{
			// Alias for the neighbor points for readability
			int connIndex = pointStoredTree[v].neighbors[n];
			// Get the vectors length
			storedLen = MVector(referencePos[connIndex] - referencePos[v]);
			currentLen = MVector(inputPos[connIndex] - inputPos[v]);

			value += (currentLen.length() / storedLen.length());
		}

		// Average the full value by the number of edges
		value = value / static_cast<double>(pointStoredTree[v].sizes);
		// Remap the value from 0-2 to -1-1
		value -= 1;
		
		// Multiply the value by the multiplier
		value *= multiplierV;

		// Clamp the value
		if (normalizeV == 1 && value > clampItMaxV) { value = clampItMaxV; }

		stressMapValues[v] = value;
	}

	// Set the output data
	MStatus status;
	MFnDoubleArrayData outputDataFn;
	MObject outputData = outputDataFn.create(stressMapValues, &status);
	data.outputValue(output).setMObject(outputData);
	data.outputValue(output).setClean();

	data.outputValue(fakeOut).set(0);
	data.outputValue(fakeOut).setClean();

	return MS::kSuccess;
}

void StressMap::buildConnectionTree(std::vector<StressPoint>& pointTree, MDoubleArray& stressMapValues, MObject& referenceMesh)
{
	// Clear the array of stress points and free memory
	pointTree.clear();
	// Clear the stress values
	stressMapValues.clear();

	// Initialize mesh functions 
	MItMeshVertex iter(referenceMesh);
	MFnMesh meshFn(referenceMesh);
	MPointArray points;
	meshFn.getPoints(points);

	// Resize the stressmap and point tree based on the number of vertices on the mesh
	int size = points.length();
	pointTree.resize(size);
	stressMapValues.setLength(size);

	// Needed variables
	int oldIndex;

	// Loop through all the points
	for (unsigned int i = 0; i < points.length(); i++)
	{
		StressPoint pnt;

		iter.setIndex(i, oldIndex);
		MIntArray vertices;
		iter.getConnectedVertices(vertices);
		pnt.neighbors = vertices;
		pnt.sizes = vertices.length();
		pointTree[i] = pnt;
		stressMapValues[i] = 0;
	}
}

inline void stressLine(MPoint& point, float stress, const float* squashColor, const float* stretchColor, const float mult)
{
	// Check if the stress1 is greater than 0, if so this means it is stretched
	const float* color = stress > 0 ? stretchColor : squashColor;

	// Clamp the stretch
	stress = stress > 1.0f ? 1.0f : stress;
	stress = stress <= 0.95f ? 0.95f : stress;

	// Check whether we got a compression or stretch
	// If compressed, the value will be negative and will be reversed
	stress = stress < 0.0f ? -stress : stress;

	// Setting the color and adding the vertex
	glColor4f(color[0] * stress * mult, color[1] * stress * mult, color[2] * stress * mult, 1.0);
	glVertex3d(point.x, point.y, point.z);
}

void StressMap::draw(M3dView& view, const MDagPath& path, M3dView::DisplayStyle dispStyle, M3dView::DisplayStatus status)
{
	// Get and use the value of the drawIt attribute
	MPlug drawItP(thisMObject(), drawIt);
	bool drawItV;
	drawItP.getValue(drawItV);

	if (drawItV == 0) { return; }

	// Force the output to evaluate
	MPlug fakeP(thisMObject(), fakeOut);
	bool fakeV;
	fakeP.getValue(fakeV);
	
	MPlug outP(thisMObject(), output);
	MObject outV;
	outP.getValue(outV);

	// Get the input mesh
	MPlug inputMeshP(thisMObject(), inputMesh);
	MObject inputMeshV;
	inputMeshP.getValue(inputMeshV);

	MFnMesh meshFn(inputMeshV);
	MPointArray inPoint;
	meshFn.getPoints(inPoint);
	MItMeshPolygon faceIt(inputMeshV);

	// Get the colors
	MPlug squashPlug(thisMObject(), squashColor);
	MObject squashObj;
	squashPlug.getValue(squashObj);
	MFnNumericData numericFn(squashObj);

	float squashColorV[] = { 0, 0, 0, 1 };
	float stretchColorV[] = { 0, 0, 0, 1 };
	
	numericFn.getData(squashColorV[0], squashColorV[1], squashColorV[2]);

	MPlug stretchPlug(thisMObject(), stretchColor);
	// MObject stretchObj;
	// stretchPlug.getValue(stretchObj);
	numericFn.setObject(stretchPlug.asMObject());
	numericFn.getData(stretchColorV[0], stretchColorV[1], stretchColorV[2]);

	// Color multiplier
	MPlug intensityP(thisMObject(), intensity);
	const float intensityVf = intensityP.asFloat();

	if (stressMapValues.length() != inPoint.length()) { return; }

	// Initialize openGL and draw //
	
	view.beginGL();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(2);
	if (status == M3dView::kLead)
	{
		glColor4f(0.0, 1.0, 0.0, 1.0f);
	}
	else
	{
		glColor4f(1.0, 1.0, 0.0, 1.0f);
	}

	faceIt.reset();

	std::vector<int> edgesDone;
	int size = meshFn.numEdges();
	edgesDone.resize(size);

	// Loop through each edge
	for (int i = 0; i < size; i++)
	{
		edgesDone[i] = 0;
	}

	MIntArray facePoint;
	MIntArray edges;

	// Start drawing the line
	glBegin(GL_LINES);

	for (; faceIt.isDone() != 1; faceIt.next())
	{
		// Draw the edges and get the vertices
		faceIt.getEdges(edges);
		faceIt.getVertices(facePoint);

		// Loop through the edges
		int length = edges.length();

		for (int e = 0; e < length; e++)
		{
			// Check if the edge has been done already
			int edgeId = edges[e];
			if (edgesDone[edgeId] == 0)
			{
				// Edges should be in the same order as the points
				// The fist edge will have points with index [0] and [1]

				int vtx1 = facePoint[e];
				int vtx2 = e != (length - 1) ? facePoint[e + 1] : facePoint[0];

				// Draw the points
				stressLine(inPoint[vtx1], stressMapValues[vtx1], squashColorV, stretchColorV, intensityVf);
				stressLine(inPoint[vtx2], stressMapValues[vtx2], squashColorV, stretchColorV, intensityVf);

				// Mark the edge as drawn
				edgesDone[edges[e]] = 1;
			}
		}
	}
	glEnd();
	glDisable(GL_BLEND);
	glPopAttrib();
}