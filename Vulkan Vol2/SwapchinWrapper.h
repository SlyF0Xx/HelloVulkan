#pragma once
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif // _WIN32

#include <vulkan\vulkan.h>
#include <vulkan\vk_platform.h>
#include <vector>
#include "Logger.h"
#include <string>

#pragma comment (lib, "vulkan-1.lib")

using namespace std;

class SwapchinWrapper
{
private:
	VkSwapchainKHR Swapchain;
	vector<VkImage> Images;
	Logger* logger;
public:
	SwapchinWrapper(VkSwapchainKHR swapchain, vector<VkImage> images, Logger* logger);
	vector<VkImage> &GetImages();
	VkSwapchainKHR GetSwapchain();
	~SwapchinWrapper();
};

