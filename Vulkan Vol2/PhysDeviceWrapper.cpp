#include "PhysDeviceWrapper.h"



PhysDeviceWrapper::PhysDeviceWrapper(VkPhysicalDevice physDevice, Logger* logger)
{
	this->logger = logger;
	PhysDevice = physDevice;
}

void PhysDeviceWrapper::addLogicDevice(int numOfQueues)
{
	uint32_t QueuesFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &QueuesFamilyCount, NULL);
	vector<VkQueueFamilyProperties> QueuesFamily(QueuesFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &QueuesFamilyCount, QueuesFamily.data());

	vector<FamilyOfQueue> QueuesFamilies;
	for (int k(0); k< QueuesFamily.size(); k++)
	{
		//Т.к. условий нет - пихаем все индексы
		QueuesFamilies.push_back({ uint32_t(k), numOfQueues,});
	}

	//--------------------------------------------------------------------------------

	vector<VkDeviceQueueCreateInfo> QueueCreateInfos(QueuesFamilies.size());
	for (int i(0); i < QueuesFamilies.size(); i++)
	{
		QueueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QueueCreateInfos[i].pNext = NULL;
		QueueCreateInfos[i].flags = 0;
		QueueCreateInfos[i].queueFamilyIndex = QueuesFamilies[i].Index;
		QueueCreateInfos[i].queueCount = numOfQueues;

		float* priorities = new float [numOfQueues];
		for (int j(0); j < numOfQueues; j++)
		{
			priorities[j] = 1.0 / (j+1);
		}

		QueueCreateInfos[i].pQueuePriorities = priorities;
	}


	//----------------------------------------------------------------

	uint32_t CountExt;
	logger->checkResults(vkEnumerateDeviceExtensionProperties(PhysDevice, nullptr, &CountExt, NULL));
	vector<VkExtensionProperties> Ext(CountExt);
	logger->checkResults(vkEnumerateDeviceExtensionProperties(PhysDevice, nullptr, &CountExt, Ext.data()));

	//--------------------------------------------------------------

	vector<char *>Extensions;
	Extensions.push_back("VK_KHR_swapchain");


	VkDeviceCreateInfo DeviceCreateInfo;
	DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	DeviceCreateInfo.pNext = NULL;
	DeviceCreateInfo.flags = 0;
	DeviceCreateInfo.queueCreateInfoCount = QueuesFamilies.size();
	DeviceCreateInfo.pQueueCreateInfos = QueueCreateInfos.data();
	DeviceCreateInfo.enabledLayerCount = 0;
	DeviceCreateInfo.enabledExtensionCount = Extensions.size();
	DeviceCreateInfo.ppEnabledExtensionNames = Extensions.data();
	DeviceCreateInfo.pEnabledFeatures = NULL;

	VkDevice temp;
	logger->checkResults(vkCreateDevice(PhysDevice, &DeviceCreateInfo, nullptr, &temp));

	for (int k(0); k< QueuesFamily.size(); k++)
	{
		//Т.к. условий нет - пихаем все индексы
		vector<VkQueue> Queues;
		for (int j(0); j < numOfQueues; j++)
		{
			VkQueue tempQueue;
			vkGetDeviceQueue(temp, k,j, &tempQueue);
			Queues.push_back(tempQueue);
		}
		QueuesFamilies[k].Queues = Queues;
	}

	Logic.push_back(LogicDeviceWrapper(temp, QueuesFamilies, logger));
}

vector<LogicDeviceWrapper> &PhysDeviceWrapper::GetLogicDevices()
{
	return Logic;
}

VkPhysicalDevice PhysDeviceWrapper::GetPhysDevice()
{
	return PhysDevice;
}

PhysDeviceWrapper::~PhysDeviceWrapper()
{
}
