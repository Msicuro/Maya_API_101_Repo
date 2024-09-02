#pragma once

#include "stressMap.h"

#include <maya/MDagPath.h>
#include <maya/MDrawContext.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnMesh.h>
#include <maya/MFrameContext.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MItMeshVertex.h>
#include <maya/MPointArray.h>
#include <maya/MUserData.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MGlobal.h>

#include "stressMapOverride.h"

MString StressMapOverride::drawDbClassification = "drawdb/geometry/StressMapOverride";
MTypeId StressMapOverride::typeID(0x90010);

MHWRender::MPxDrawOverride* StressMapOverride::creator(const MObject& obj) { return new StressMapOverride(obj); }

StressMapOverride::StressMapOverride(const MObject& obj) : MHWRender::MPxDrawOverride(obj, StressMapOverride::draw) {}

MHWRender::DrawAPI StressMapOverride::supportedDrawAPIs() const { return MHWRender::kAllDevices; }

void draw(const MHWRender::MDrawContext& context, const MUserData* data) {}

// This is called before the draw, here you can store data inside the user daya points so that it can be used inside the draw function
MUserData* StressMapOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* data)
{
	// Get the user data for the draw if available. This will be returned by this function and passed by Maya to the various draw functions
	auto* boundsData = dynamic_cast<StressMapUserData*>(data);
	
	if (!boundsData) { boundsData = new StressMapUserData(); }

	// Use the node to access the plugs
	MObject obj = objPath.node();
	MPlug stressPlug(obj, StressMap::output);
	// Conver the MObject of the plug into a usable array
	MFnDoubleArrayData doubleFn;
	MStatus status;
	status = doubleFn.setObject(stressPlug.asMObject()); CHECK_MSTATUS(status);
	boundsData->stressData = doubleFn.array(&status); CHECK_MSTATUS(status);

	MPlug meshPlug(obj, StressMap::inputMesh);
	MFnMesh meshFn;
	status = meshFn.setObject(meshPlug.asMObject()); CHECK_MSTATUS(status);
	meshFn.getPoints(boundsData->meshPoints);

	boundsData->fPath = objPath;

	// Get the object bounding box
	MFnDagNode dagNodeFn(objPath);
	boundsData->fBounds = dagNodeFn.boundingBox();

	return boundsData;
}

// This function allows for rendering simple UI features, and provides access to the drawManager which can render simple geometry like points & lines
void StressMapOverride::addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data)
{
	// Get the user data that will contain the bounds
	const auto* boundsData = dynamic_cast<const StressMapUserData*>(data);
	if (!boundsData) { return; }

	const MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(objPath);

	// Extract variables from the arrays
	const MDoubleArray& stress = boundsData->stressData;
	const MPointArray& points = boundsData->meshPoints;
	const unsigned int stressSize = stress.length();
	const unsigned int pointSize = points.length();
	
	// Make sure the size is the same 
	if (stressSize != pointSize) { MGlobal::displayError("Point and Stress arrays length do not match"); return; }

	drawManager.beginDrawable();
	if (frameContext.getDisplayStyle() & (MHWRender::MFrameContext::kFlatShaded | MHWRender::MFrameContext::kGouraudShaded | MHWRender::MFrameContext::kTextured))
	{
		const unsigned int length = points.length();
		MColor green = MColor(0.0, 1.0f, 0.0, 1.0f);
		MColor red = MColor(1.0, 0.0f, 0.0, 1.0f);
		for (unsigned int i = 0; i < pointSize; i++)
		{
			MColor color = stress[i] > 0.0f ? stress[i] * green : (-stress[i]) * (red);
			drawManager.setColor(color);
			drawManager.sphere(points[i], 0.2f, 5, 5, true);
		}
	}

	drawManager.endDrawable();
}