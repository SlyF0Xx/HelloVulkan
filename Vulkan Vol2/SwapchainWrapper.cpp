#include "SwapchainWrapper.h"



SwapchainWrapper::SwapchainWrapper(VkSwapchainKHR swapchain, vector<VkImage> images, Logger* logger)
{
	Swapchain = swapchain;
	Images = images;
	this->logger = logger;
}

vector<VkImage>& SwapchainWrapper::GetImages()
{
	return Images;
}

VkSwapchainKHR SwapchainWrapper::GetSwapchain()
{
	return Swapchain;
}

SwapchainWrapper::~SwapchainWrapper()
{
}
