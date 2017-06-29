#include "SwapchinWrapper.h"



SwapchinWrapper::SwapchinWrapper(VkSwapchainKHR swapchain, vector<VkImage> images, Logger* logger)
{
	Swapchain = swapchain;
	Images = images;
	this->logger = logger;
}

vector<VkImage>& SwapchinWrapper::GetImages()
{
	return Images;
}

VkSwapchainKHR SwapchinWrapper::GetSwapchain()
{
	return Swapchain;
}

SwapchinWrapper::~SwapchinWrapper()
{
}
