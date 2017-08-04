#define VK_USE_PLATFORM_WIN32_KHR

#include <Windows.h>
#include <stdlib.h>
#include<time.h>
#include <string>
#include "Logger.h"
#include "Foundation.h"
#include <vulkan\vk_platform.h>
#include <vulkan\vulkan.h>

#include "Model.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PrimitiveModel.h"
#include "PrimitivePipelineWrapper.h"
#include "PrimitiveIndexedModel.h"
#include "ColorModel.h"
#include "ColorPipelineWrapper.h"
#include "PrimitiveCamera.h"
#include "AbstractWorldModel.hpp"

#pragma comment (lib, "vulkan-1.lib")

using namespace std;


const int ScreenX = 900;
const int ScreenY = 600;
const float zoom = -2.5f;
glm::vec3 rotation = glm::vec3();

#ifdef _WIN32
WNDCLASSEX windowsclass;
MSG msg;
HWND hwnd;
HINSTANCE HInstance;
#endif


Logger logger;
AbstractCamera* ControlCamera;
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN:
	{
		ControlCamera->keyPressed(wparam);
		break;
	}
	}


	return (DefWindowProc(hwnd, msg, wparam, lparam));
}


VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t object,
	size_t location,
	int32_t messageCode,
	const char *pLayerPrefix,
	const char *pMessage,
	void *pUserData)
{
	logger.LogMessage(pMessage);
	return VK_FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	windowsclass.cbSize = sizeof(windowsclass);
	windowsclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	windowsclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowsclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowsclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	windowsclass.hInstance = hInstance;
	windowsclass.lpfnWndProc = WinProc;
	windowsclass.lpszClassName = L"windowsclass";
	windowsclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowsclass);

	hwnd = CreateWindow(
		L"windowsclass",
		L"VulkanApp",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100,
		ScreenX,
		ScreenY,
		NULL,
		NULL,
		hInstance,
		NULL);
	HInstance = hInstance;

	logger.LogMessage("Start");

	try {
		Foundation foundation = Foundation(&logger);
		
#ifdef _DEBUG
		PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = NULL;
		PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = NULL;
		fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)
			vkGetInstanceProcAddr(foundation.GetInstance(), "vkCreateDebugReportCallbackEXT");
		fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)
			vkGetInstanceProcAddr(foundation.GetInstance(), "vkDestroyDebugReportCallbackEXT");

		VkDebugReportCallbackCreateInfoEXT debug_report_callback_info;
		memset(&debug_report_callback_info, 0, sizeof(debug_report_callback_info));
		debug_report_callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debug_report_callback_info.flags = VK_DEBUG_REPORT_DEBUG_BIT_EXT |
			VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
		debug_report_callback_info.pfnCallback = DebugReportCallback;

		VkDebugReportCallbackEXT debug_report_callback = VK_NULL_HANDLE;
		logger.checkResults(fvkCreateDebugReportCallbackEXT(foundation.GetInstance(), &debug_report_callback_info, nullptr, &debug_report_callback));
#endif // DEBUG

		foundation.CreateDevices();
		logger.LogMessage("Generate Devacie");
		
		
		foundation.addSurface(HInstance, hwnd);

		logger.LogMessage("Generate Surface");
		logger.LogMessage(foundation.GetSurfaces()[0]);

		for (auto &i : foundation.GetPhysDevices())
		{
			i.addLogicDevice(1);
			logger.LogMessage("Generate Logic Devacie");
			logger.LogMessage(i.GetLogicDevices()[0].GetLogicDevice());

			for (auto &j : i.GetLogicDevices())
			{
				foundation.addSwapchain(i.GetPhysDevice(), foundation.GetSurfaces()[0], &j);
				logger.LogMessage("Generate Swapchain");
				for (auto k : j.GetQueuesFamilies())
				{
					j.addCommandPool(k.Index);
					logger.LogMessage("Generate Queue Family");
				}
				for (CommandPoolWrapper& k : j.GetCommandPools())
				{
					j.addCommandBuffers(&k, true, j.GetSwapchins()[0].GetImages().size()*2);					
					logger.LogMessage("Generate Command Pool");
				}
			}
		}


		ZeroMemory(&msg, sizeof(msg));

		ShowWindow(hwnd, nShowCmd);
		UpdateWindow(hwnd);
		ZeroMemory(&msg, sizeof(msg));


		logger.LogMessage("Generate");

		PrimitiveModel* mode = new PrimitiveModel(foundation.GetPhysDevices()[0].GetLogicDevices()[0], glm::vec3());
		//PrimitiveIndexedModel* Imode = new PrimitiveIndexedModel(foundation.GetPhysDevices()[0].GetLogicDevices()[0]);
		//PrimitiveModel* mode = PrimitiveModel::create(foundation.GetPhysDevices()[0].GetLogicDevices()[0]);
		//PrimitiveIndexedModel* Imode = PrimitiveIndexedModel::create(foundation.GetPhysDevices()[0].GetLogicDevices()[0]);
		ColorModel * model = new ColorModel(foundation.GetPhysDevices()[0].GetLogicDevices()[0], glm::vec3());


		//MatrixesBufer MatrixesMainBufer = MatrixesBufer(foundation.GetPhysDevices()[0].GetLogicDevices()[0]);

		PrimitiveCamera camera = PrimitiveCamera(&logger, foundation.GetPhysDevices()[0].GetLogicDevices()[0], foundation.GetSurfaceFormat(), ScreenX, ScreenY, zoom);
		ControlCamera = &camera;
		static_cast<PrimitivePipelineWrapper*>(camera.Pipelines[0])->Models.push_back((AbstractWorldModel<PrimitiveVertex>*)mode);
		static_cast<ColorPipelineWrapper*>(camera.Pipelines[1])->Models.push_back((AbstractWorldModel<ColorVertex>*)model);


		logger.LogMessage("Done");


		VkSemaphoreCreateInfo SemaphoreInfo;
		SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		SemaphoreInfo.pNext = NULL;
		SemaphoreInfo.flags = 0;

		VkSemaphore ImageReady;
		VkSemaphore CalculationReady;
		logger.checkResults(vkCreateSemaphore(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &SemaphoreInfo, nullptr, &ImageReady));
		logger.checkResults(vkCreateSemaphore(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &SemaphoreInfo, nullptr, &CalculationReady));

		logger.LogMessage("Prepare");
		for (int i(0); i < foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0].GetImages().size(); i++)
		{
			camera.Draw(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0].GetImages()[i]);
		}

		VkPipelineStageFlags stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

		unsigned int ImageIndex;

		logger.LogMessage("All prepare for draw");

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			else
			{
				logger.LogMessage("Start draw");
				logger.checkResults(vkAcquireNextImageKHR(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(),
					foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0].GetSwapchain(), UINT64_MAX, ImageReady, NULL, &ImageIndex));
				
				VkSubmitInfo SubmitInfo;
				SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				SubmitInfo.pNext = NULL;
				SubmitInfo.waitSemaphoreCount = 1;
				SubmitInfo.pWaitSemaphores = &ImageReady;
				SubmitInfo.pWaitDstStageMask = &stage;
				SubmitInfo.commandBufferCount = 1;
				SubmitInfo.pCommandBuffers = &foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[ImageIndex];
				SubmitInfo.signalSemaphoreCount = 1;
				SubmitInfo.pSignalSemaphores = &CalculationReady;



				logger.checkResults(vkQueueSubmit(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Queues[0], 1, &SubmitInfo ,NULL));

				VkSwapchainKHR swap = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0].GetSwapchain();
				VkResult* Results = new VkResult[1];

				VkPresentInfoKHR PresentInfo;
				PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
				PresentInfo.pNext = NULL;
				PresentInfo.waitSemaphoreCount = 1;
				PresentInfo.pWaitSemaphores = &CalculationReady;
				PresentInfo.swapchainCount = 1;
				PresentInfo.pSwapchains = &swap;
				PresentInfo.pImageIndices = &ImageIndex;
				PresentInfo.pResults = Results;

				
				logger.checkResults(vkQueuePresentKHR(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Queues[0], &PresentInfo));
				logger.checkResults(Results[0]);
			}
		}

	}	
	catch (exception e)
	{
		logger.LogMessage((string) e.what());
	}
}