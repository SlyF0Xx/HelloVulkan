#pragma once
#include "AbstractControlCamera.h"
#include "PrimitivePipelineWrapper.h"
#include "ColorPipelineWrapper.h"
#include "MatrixBuffer.h"

class PrimitiveCamera :
	public AbstractControlCamera
{
private:
	MatrixBuffer MainMatrixes;
	Logger * logger;
	VkSurfaceFormatKHR SurfaceFormat;
	LogicDeviceWrapper Device;
public:
	PrimitiveCamera(Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR surfaceFormat, int ScreenX, int ScreenY, float zoom);
	void keyPressed(uint32_t keyCode);
	void Draw(VkCommandBuffer CmdBuffer, VkImage image);
	
	~PrimitiveCamera();
};

