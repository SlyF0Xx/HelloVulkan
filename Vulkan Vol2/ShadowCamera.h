#pragma once
#include "AbstractControlCamera.h"
#include "MatrixBuffer.h"
#include "ShadowPrimitivePipeline.h"
#include "ShadowColorPipeline.h"
#include "TransformationBuffer.h"
#include "MaterialPipelineWrapper.h"

class ShadowCamera :
	public AbstractControlCamera
{
private:

	static const VkFormat DepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
	Logger * logger;
	VkSurfaceFormatKHR SurfaceFormat;
	LogicDeviceWrapper Device;
	VkImage DepthTexture;
	VkImageView DethView;
	MatrixBuffer MainMatrixes;

public:
	ShadowCamera(Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR surfaceFormat,
		int ScreenX, int ScreenY, float zoom, MatrixBuffer _MainMatrixes, VkImage depthTexture, VkImageView texture);
	
	void keyPressed(uint32_t keyCode);
	void Draw(VkCommandBuffer CmdBuffer, VkImage image);
	void loadMatModel(string name, string path, glm::vec3 translation);
	void SetTexture(VkCommandBuffer CmdBuffer, VkImage depthTexture);
	TransformationBuffer TransformationMat;
	~ShadowCamera();
};

