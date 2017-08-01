#pragma once
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif // _WIN32

#include <vulkan\vulkan.h>
#include <vulkan\vk_platform.h>
#include <vector>
#include "Logger.h"
#include <string>
#include "Foundation.h"
#include "AbstractModel.hpp"

#pragma comment (lib, "vulkan-1.lib")

using namespace std;

class AbstractBasePipelineWrapper
{
protected:
	virtual void _Draw(VkCommandBuffer CmdBuffer, vector<VkImageView> ImageViews) = 0;
public:
	AbstractBasePipelineWrapper();
	virtual ~AbstractBasePipelineWrapper();
};

