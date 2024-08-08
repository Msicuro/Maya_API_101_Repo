#pragma once

#include <maya/MPxLocatorNode.h>
#include <maya/MFnNumericAttribute.h>

class LocatorNodeVP1 : public MPxLocatorNode
{
	static MObject drawIt;

public:
	// General Node Setup
	static MTypeId typeID;
	static void* creator();
	static MStatus initialize();

	bool isBounded() const override;
	MBoundingBox boundingBox() const override;
	void draw(M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView::DisplayStatus status) override;

};