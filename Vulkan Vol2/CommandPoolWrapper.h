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

class CommandPoolWrapper
{
private:
	VkCommandPool CommandPool;
	vector<VkCommandBuffer> CommandBuffers;
	Logger* logger;
public:
	vector<VkCommandBuffer> &GetCommandBuffers();
	VkCommandPool GetCommandPool();
	void AddCommandBuffer(vector<VkCommandBuffer> Buffers);
	void AddCommandBuffer(VkCommandBuffer Buffer);
	CommandPoolWrapper(VkCommandPool CommandPool, Logger* logger);
	~CommandPoolWrapper();
};

