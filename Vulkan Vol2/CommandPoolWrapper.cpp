#include "CommandPoolWrapper.h"



vector<VkCommandBuffer> &CommandPoolWrapper::GetCommandBuffers()
{
	return CommandBuffers;
}

VkCommandPool CommandPoolWrapper::GetCommandPool()
{
	return CommandPool;
}

void CommandPoolWrapper::AddCommandBuffer(vector<VkCommandBuffer> Buffers)
{
	CommandBuffers.insert(CommandBuffers.end(), Buffers.begin(), Buffers.end());
}

void CommandPoolWrapper::AddCommandBuffer(VkCommandBuffer Buffer)
{
	CommandBuffers.push_back(Buffer);
}

CommandPoolWrapper::CommandPoolWrapper(VkCommandPool commandPool, Logger * logger)
{
	CommandPool = commandPool;
	this->logger = logger;
}

CommandPoolWrapper::~CommandPoolWrapper()
{
}
