#pragma once

#include "FirstWorldMaterialModel.h"
#include "AbstractControlCamera.h"
#include "MatrixBuffer.h"
#include "LightPrimitivePipeline.h"
#include "LightColorPipeline.h"
#include "LightMaterialPipeline.h"

class LightCamera :
	public AbstractControlCamera
{
private:
	Logger * logger;
	LogicDeviceWrapper Device;
	static const VkFormat DepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
public:
	LightCamera(Logger * logger, LogicDeviceWrapper device, int ScreenX, int ScreenY);
	void keyPressed(uint32_t keyCode);
	void Draw(VkCommandBuffer CmdBuffer, VkImage image);
	void loadMatModel(string name, string path, glm::vec3 translation);
	MatrixBuffer MainMatrixes;
	~LightCamera();
};

