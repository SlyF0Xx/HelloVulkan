#pragma once

#include "AbstractScene.h"
#include "ShadowCamera.h"
#include "LightCamera.h"
#include "SwapchainWrapper.h"
#include "CommandPoolWrapper.h"
#include <map>

class PrimitiveScene :
	public AbstractScene
{
protected:
	void Draw();
	LogicDeviceWrapper LogicDevice;
	Logger* logger;
	SwapchainWrapper Swapchain;
	CommandPoolWrapper ComPool;



	VkSemaphore _ImageReady; VkSemaphore _CalculationReady; VkQueue _Queue;




public:

	map<VkImage, VkImage> ColorViews;
	VkImage CreateDepthImage();
	void Draw(VkSemaphore ImageReady, VkSemaphore CalculationReady, VkQueue Queue);
	void PreDraw(VkCommandBuffer CmdBuffer, VkImage Color);
	void Update();
	PrimitiveScene(Logger* logger, LogicDeviceWrapper logicDevice, int ScreenX, int ScreenY, int zoom, VkSurfaceFormatKHR surfaceFormat, SwapchainWrapper swapchain, CommandPoolWrapper comPool, VkQueue Queue);
	~PrimitiveScene();
};

