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

#pragma comment (lib, "vulkan-1.lib")

using namespace std;

class AbstractIndexBufferWrapper
{
protected:
	vector<uint32_t> Indexes;
	VkBuffer Buffer;
	VkDeviceMemory memory;
	LogicDeviceWrapper Device;
public:
	virtual void WriteIndexes() =0;
	VkBuffer GetBuffer();
	vector<uint32_t> GetIndexes();
	AbstractIndexBufferWrapper(LogicDeviceWrapper device, vector<uint32_t> indexes);
	virtual ~AbstractIndexBufferWrapper();

	AbstractIndexBufferWrapper() {};
};