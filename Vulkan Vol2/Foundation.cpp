#include "Foundation.h"


VkSurfaceFormatKHR Foundation::GetSurfaceFormat()
{
	return SurfaceFormat;
}

vector<PhysDeviceWrapper> &Foundation::GetPhysDevices()
{
	return AvaliablePhysDevices;
}


#ifdef _WIN32
void Foundation::addSurface(HINSTANCE &hinstance, HWND &hwnd)
{
	VkWin32SurfaceCreateInfoKHR SurfaceInfo;
	SurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	SurfaceInfo.pNext = NULL;
	SurfaceInfo.flags = 0;
	SurfaceInfo.hinstance = hinstance;
	SurfaceInfo.hwnd = hwnd;

	Surfaces.resize(Surfaces.size()+1);
	
	//VkSurfaceKHR temp;
	//logger->checkResults(vkCreateWin32SurfaceKHR(Instance, &SurfaceInfo, nullptr, &temp]));
	//Surfaces.push_back(temp);
	
	logger->checkResults(vkCreateWin32SurfaceKHR(Instance, &SurfaceInfo, nullptr, &Surfaces[Surfaces.size()-1]));
}
#endif // _WIN32

Foundation::Foundation(Logger* logger)
{
	this->logger = logger;

	VkApplicationInfo ApplicationInfo;
	ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ApplicationInfo.pNext = NULL;
	ApplicationInfo.pApplicationName = "Vulkan Vol2";
	ApplicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	ApplicationInfo.pEngineName = "VulkanEngine";
	ApplicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	ApplicationInfo.apiVersion = 0;
	
	//------------------------------------------------

	uint32_t LayCount;
	logger->checkResults(vkEnumerateInstanceLayerProperties(&LayCount, NULL));
	vector<VkLayerProperties> Layers(LayCount);
	logger->checkResults(vkEnumerateInstanceLayerProperties(&LayCount, Layers.data()));

	uint32_t CountExt;
	logger->checkResults(vkEnumerateInstanceExtensionProperties(nullptr, &CountExt, NULL));
	vector<VkExtensionProperties> Ext(CountExt);
	logger->checkResults(vkEnumerateInstanceExtensionProperties(nullptr, &CountExt, Ext.data()));

	vector<char *> Extensions;

	Extensions.push_back("VK_KHR_surface");
	Extensions.push_back("VK_EXT_debug_report");
#ifdef _WIN32
	Extensions.push_back("VK_KHR_win32_surface");
	logger->LogMessage("Win surface");
#endif // WIN32

	vector<char *> Layouts;
	
	Layouts.push_back("VK_LAYER_LUNARG_standard_validation");
	//-------------------------------------------------------

	VkInstanceCreateInfo InstanceInfo;
	InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceInfo.pNext = NULL;
	InstanceInfo.flags = 0;
	InstanceInfo.pApplicationInfo = &ApplicationInfo;
	InstanceInfo.enabledLayerCount = Layouts.size();
	InstanceInfo.ppEnabledLayerNames = Layouts.data();
	InstanceInfo.enabledExtensionCount = Extensions.size();
	InstanceInfo.ppEnabledExtensionNames = Extensions.data();

	this->logger->checkResults(vkCreateInstance(&InstanceInfo, nullptr, &Instance));
}

void Foundation::CreateDevices()
{
	uint32_t DevicesCount;
	logger->checkResults(vkEnumeratePhysicalDevices(Instance, &DevicesCount, NULL));
	vector<VkPhysicalDevice> PhysDevices(DevicesCount);
	logger->checkResults(vkEnumeratePhysicalDevices(Instance, &DevicesCount, PhysDevices.data()));

	VkPhysicalDeviceProperties pProperties;

	for (auto i : PhysDevices)
	{
		vkGetPhysicalDeviceProperties(i, &pProperties);

		logger->LogMessage(pProperties.deviceName);
		logger->LogMessage(pProperties.deviceType);
		
		if (pProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			AvaliablePhysDevices.push_back(PhysDeviceWrapper(i, logger));
		}
	}

}

VkInstance Foundation::GetInstance()
{
	return Instance;
}

vector<VkSurfaceKHR> &Foundation::GetSurfaces()
{
	return Surfaces;
}

void Foundation::addSwapchain(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, LogicDeviceWrapper *logicDevice)
{
	uint32_t FromatsCount;
	logger->checkResults(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &FromatsCount, NULL));
	vector<VkSurfaceFormatKHR> SurfaceFormats(FromatsCount);
	logger->checkResults(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &FromatsCount, SurfaceFormats.data()));

	VkBool32 Supported;
	logger->checkResults(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, logicDevice->GetQueuesFamilies()[0].Index, surface, &Supported));
	if (Supported)
	{
		logger->LogMessage("Support!");
	}

	logger->LogMessage("Finding format");
	
	if (SurfaceFormats.size() == 1 && SurfaceFormats.front().format == VK_FORMAT_UNDEFINED)
	{
		SurfaceFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	}
	for (auto i : SurfaceFormats)
	{
		//
		if (i.format == VK_FORMAT_R8G8B8A8_UNORM && i.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			SurfaceFormat = i;
			break;
		}
	}
	if (!SurfaceFormat.format)
	{
		logger->LogMessage("Cannot find useful surface format!");
		logger->checkResults(VK_ERROR_SURFACE_LOST_KHR);
	}

	VkSurfaceCapabilitiesKHR Capabilities;
	logger->checkResults(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &Capabilities));
	
	uint32_t PresentModeCount;
	logger->checkResults(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &PresentModeCount, NULL));
	vector<VkPresentModeKHR> AllPresentModes(PresentModeCount);
	logger->checkResults(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &PresentModeCount, AllPresentModes.data()));


	logger->LogMessage("Finding present mode");
	VkPresentModeKHR PresentMode;
	for (auto i : AllPresentModes)
	{
		if (i == VK_PRESENT_MODE_FIFO_KHR)
			PresentMode = i;
	}
	if (!PresentMode)
	{
		logger->LogMessage("Cannot find required resent mode. Use anything!");
		PresentMode = AllPresentModes[0];
	}

	logger->LogMessage(surface);

	VkSwapchainCreateInfoKHR SwapchainInfo;
	SwapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapchainInfo.pNext = NULL;
	SwapchainInfo.flags = 0;
	SwapchainInfo.surface = surface;
	SwapchainInfo.minImageCount = 3;//Capabilities.maxImageCount; 
	SwapchainInfo.imageFormat = SurfaceFormat.format;
	SwapchainInfo.imageColorSpace = SurfaceFormat.colorSpace;
	SwapchainInfo.imageExtent = Capabilities.currentExtent;
	SwapchainInfo.imageArrayLayers = 1;
	SwapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	
	if (logicDevice->GetQueuesFamilies().size() == 1 && logicDevice->GetQueuesFamilies()[0].Count == 1)
	{
		SwapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		SwapchainInfo.queueFamilyIndexCount = 1;
		SwapchainInfo.pQueueFamilyIndices = (uint32_t *)&logicDevice->GetQueuesFamilies()[0].Index;
	}
	else
	{
		vector<uint32_t> indexes;
		for (auto i : logicDevice->GetQueuesFamilies())
		{
			indexes.push_back(i.Index);
		}
		SwapchainInfo.queueFamilyIndexCount = logicDevice->GetQueuesFamilies().size();
		SwapchainInfo.pQueueFamilyIndices = indexes.data();
	}

	SwapchainInfo.preTransform = Capabilities.currentTransform;
	SwapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	SwapchainInfo.presentMode = PresentMode;
	SwapchainInfo.clipped = VK_TRUE;
	SwapchainInfo.oldSwapchain = VK_NULL_HANDLE;

	VkSwapchainKHR temp;
	logger->checkResults(vkCreateSwapchainKHR(logicDevice->GetLogicDevice(), &SwapchainInfo, nullptr, &temp));
	logicDevice->addSwapchain(temp);
}


Foundation::~Foundation()
{
}
