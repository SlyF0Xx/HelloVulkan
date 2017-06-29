#include "LogicDeviceWrapper.h"


void LogicDeviceWrapper::addSwapchain(VkSwapchainKHR swapchain)
{
	uint32_t CountOfImages;
	logger->checkResults(vkGetSwapchainImagesKHR(LogicDevice, swapchain, &CountOfImages, NULL));
	vector<VkImage> Images(CountOfImages);
	logger->checkResults(vkGetSwapchainImagesKHR(LogicDevice, swapchain, &CountOfImages, Images.data()));

	Swapchins.push_back(SwapchinWrapper(swapchain, Images, logger));
}

VkDevice LogicDeviceWrapper::GetLogicDevice()
{
	return LogicDevice;
}

vector<SwapchinWrapper> &LogicDeviceWrapper::GetSwapchins()
{
	return Swapchins;
}

void LogicDeviceWrapper::addCommandPool(int num)
{
	VkCommandPoolCreateInfo CommandPoolInfo;
	CommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CommandPoolInfo.pNext = NULL;
	CommandPoolInfo.flags = 0;
	CommandPoolInfo.queueFamilyIndex = num;

	VkCommandPool temp;
	logger->checkResults(vkCreateCommandPool(LogicDevice, &CommandPoolInfo, nullptr, &temp));
	CommandPools.push_back(CommandPoolWrapper(temp, logger));
}

void LogicDeviceWrapper::addCommandBuffers(CommandPoolWrapper* CommandPool, bool isPrimary, int count)
{
	VkCommandBufferAllocateInfo CommandBufferInfo;
	CommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	CommandBufferInfo.pNext = NULL;
	CommandBufferInfo.commandPool = CommandPool->GetCommandPool();
	CommandBufferInfo.level = isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	CommandBufferInfo.commandBufferCount = count;

	vector<VkCommandBuffer> temp(count);
	logger->checkResults(vkAllocateCommandBuffers(LogicDevice, &CommandBufferInfo, temp.data()));
	CommandPool->AddCommandBuffer(temp);
}

vector<FamilyOfQueue> &LogicDeviceWrapper::GetQueuesFamilies()
{
	return QueuesFamilies;
}

vector<CommandPoolWrapper> &LogicDeviceWrapper::GetCommandPools()
{
	return CommandPools;
}

LogicDeviceWrapper::LogicDeviceWrapper(VkDevice logicDevice, vector<FamilyOfQueue> queuesFamilyIndexes, Logger * logger)
{
	LogicDevice = logicDevice;
	this->logger = logger;
	QueuesFamilies = queuesFamilyIndexes;
}

LogicDeviceWrapper::~LogicDeviceWrapper()
{
}
