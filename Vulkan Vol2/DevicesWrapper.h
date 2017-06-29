#pragma once
#include <vulkan\vulkan.h>
#include <vector>
#include "Logger.h"
#include <string>

#pragma comment (lib, "vulkan-1.lib")

using namespace std;


class DevicesWrapper
{
private:
	vector<VkPhysicalDevice> PhysicalDevices;
	vector<VkDevice> LogicDevices;

public:
	void addDevice(VkPhysicalDevice* linkedDevice, vector<VkDeviceQueueCreateInfo> queueInfos);
	void addPhysDevice(VkPhysicalDevice* physDevice);


	DevicesWrapper();
	DevicesWrapper(vector<VkPhysicalDevice>* physicalDevices);
	~DevicesWrapper();
};

