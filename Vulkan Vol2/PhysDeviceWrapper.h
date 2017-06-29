#pragma once
#include "LogicDeviceWrapper.h"

using namespace std;

class PhysDeviceWrapper
{
private:
	Logger* logger;
	VkPhysicalDevice PhysDevice;

	vector<LogicDeviceWrapper> Logic;
public:
	PhysDeviceWrapper(VkPhysicalDevice physDevice, Logger* logger);
	void addLogicDevice(int numOfQueues);
	vector<LogicDeviceWrapper> &GetLogicDevices();
	VkPhysicalDevice GetPhysDevice();
	~PhysDeviceWrapper();
};

