#pragma once
#include "PhysDeviceWrapper.h"

using namespace std;



class Foundation
{
private:
	Logger* logger;
	VkInstance Instance = VK_NULL_HANDLE;
	vector<PhysDeviceWrapper> AvaliablePhysDevices;
	vector<VkSurfaceKHR> Surfaces;
	VkSurfaceFormatKHR SurfaceFormat;
public:
	VkSurfaceFormatKHR GetSurfaceFormat();
	vector<PhysDeviceWrapper> &GetPhysDevices();
#ifdef _WIN32
	void addSurface(HINSTANCE &hinstance, HWND &hwnd);
#endif
	Foundation(Logger* logger);
	~Foundation();
	void CreateDevices();
	VkInstance GetInstance();
	vector<VkSurfaceKHR> &GetSurfaces();
	void addSwapchain(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, LogicDeviceWrapper *logicDevice);
};

