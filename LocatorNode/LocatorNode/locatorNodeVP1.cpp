#pragma once
#include "locatorNodeVP1.h"

MTypeId LocatorNodeVP1::typeID(0x90006);
MObject LocatorNodeVP1::drawIt;

void* LocatorNodeVP1::creator(){ return new LocatorNodeVP1; }

bool LocatorNodeVP1::isBounded() const { return true; }

MBoundingBox LocatorNodeVP1::boundingBox() const
{
	return MBoundingBox(MPoint(-2.0, -1.5, 0.0), MPoint(3.0, 1.5, 0.0));
}

MStatus LocatorNodeVP1::initialize()
{
	MStatus status;

	MFnNumericAttribute numericFn;
	drawIt = numericFn.create("drawIt", "drawIt", MFnNumericData::kBoolean, 1.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
	status = addAttribute(drawIt); CHECK_MSTATUS_AND_RETURN_IT(status);

	return status;
}

void LocatorNodeVP1::draw(M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
	MPlug drawItP(thisMObject(), drawIt);
	bool drawItV;
	drawItP.getValue(drawItV);
	if (!drawItV) { return; }

	view.beginGL();

	glPushAttrib(GL_ALL_ATTRIB_BITS); // Back up the current global settings

	glEnable(GL_BLEND); // Allow transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Enable transparency

	// Change the color if this is the most recently selected object
	if (status == M3dView::kLead) { glColor4d(0.0, 1.0, 0.0, 0.3); }
	else { glColor4d(1.0, 1.0, 0.0, 0.3); }

	glBegin(GL_TRIANGLES); // Draw triangles
	glVertex3d(-2.0, 1.0, 0.0); // First triangle
	glVertex3d(-2.0, -1.0, 0.0);
	glVertex3d(2.0, -1.0, 0.0);

	glVertex3d(2.0, 1.0, 0.0); // Second triangle
	glVertex3d(-2.0, -1.0, 0.0);
	glVertex3d(2.0, -1.0, 0.0);
	
	glVertex3d(2.0, -1.5, 0.0); // Third triangle
	glVertex3d(2.0, 1.5, 0.0);
	glVertex3d(3.0, 0.0, 0.0);
	glEnd();

	glPopAttrib(); // Restore all global settings to the last backup

	view.endGL();
}