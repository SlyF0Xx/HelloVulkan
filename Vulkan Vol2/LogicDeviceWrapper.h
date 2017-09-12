#pragma once
#include "CommandPoolWrapper.h"
#include "SwapchainWrapper.h"

using namespace std;

struct FamilyOfQueue
{
	uint32_t Index;
	int Count;
	vector<VkQueue> Queues;
};

class LogicDeviceWrapper
{
private:
	VkDevice LogicDevice;
	vector<FamilyOfQueue> QueuesFamilies;
	vector<CommandPoolWrapper> CommandPools;
	vector<SwapchainWrapper> Swapchins;
	Logger* logger;
public:
	void addSwapchain(VkSwapchainKHR swapchain);
	VkDevice GetLogicDevice();
	vector<SwapchainWrapper> &GetSwapchins();
	void addCommandPool(int num);
	void addCommandBuffers(CommandPoolWrapper* CommandPool, bool isPrimary, int count);
	vector<FamilyOfQueue> &GetQueuesFamilies();
	vector<CommandPoolWrapper> &GetCommandPools();
	LogicDeviceWrapper(VkDevice LogicDevice, vector<FamilyOfQueue> queuesFamilyIndexes, Logger* logger);
	~LogicDeviceWrapper();

	LogicDeviceWrapper() {};
};

