#pragma once

#include <maya/MBoundingBox.h>
#include <maya/MPxDrawOverride.h>

class StressMapOverride final : public ::MPxDrawOverride
{
public:
	static MTypeId typeID;
	static MString drawDbClassification;

private:
	
	// Class used to store the necessary data for the render
	class StressMapUserData final : public  MUserData
	{
	public:
		StressMapUserData() : MUserData() { }
		virtual ~StressMapUserData() = default;

		MDoubleArray stressData;
		MPointArray meshPoints;
		MBoundingBox fBounds;
		MDagPath fPath;
	};

public:

	static MHWRender::MPxDrawOverride* creator(const MObject& obj);

	virtual ~StressMapOverride() = default;

	virtual MHWRender::DrawAPI supportedDrawAPIs() const override;
	
	MUserData* prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* data) override;
	
	bool hasUIDrawables() const override { return true; }

	void addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data) override;

	//static void draw(const MHWRender::MDrawContext& context, const MUserData* data);

private:

	// Constuctor should only be called from the creator function
	StressMapOverride(const MObject& obj);
};