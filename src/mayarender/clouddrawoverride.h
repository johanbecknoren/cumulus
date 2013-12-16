#ifndef _CLOUD_DRAW_OVERRIDE_
#define _CLOUD_DRAW_OVERRIDE_

// Viewport 2.0 class
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPxLocatorNode.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MDrawContext.h>
#include <maya/MFrameContext.h>
#include <maya/MDrawRegistry.h>

#include <coregl.h>

class CloudData : public MUserData {
public:
	CloudData() : MUserData(false), cloudcore(NULL) 
	{} // False - dont del after draw
	virtual ~CloudData() {}

	MBoundingBox fCurrentBoundingBox;
	float fMultiplier;
	float fColor[3];
	core::CoreGL *cloudcore;

};

class CloudDrawOverride : public MHWRender::MPxDrawOverride {
public:
	static MHWRender::MPxDrawOverride *Creator(const MObject &obj) {
		return new CloudDrawOverride(obj);
	}
	virtual ~CloudDrawOverride();

	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	virtual bool isBounded(
		const MDagPath& objPath,
		const MDagPath& cameraPath) const;

	virtual MBoundingBox boundingBox(
		const MDagPath& objPath,
		const MDagPath& cameraPath) const;

	virtual MUserData* prepareForDraw(
		const MDagPath& objPath,
		const MDagPath& cameraPath,
		const MHWRender::MFrameContext& frameContext,
		MUserData* oldData);

	virtual bool hasUIDrawables() const { return true; }

	virtual void addUIDrawables(
		const MDagPath& objPath,
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext,
		const MUserData* data);

	static void draw(const MHWRender::MDrawContext& context, const MUserData* data);

protected:
	MBoundingBox mCurrentBoundingBox;

private:
	CloudDrawOverride(const MObject &obj);
	float getMultiplier(const MDagPath &objPath) const;
};

#endif