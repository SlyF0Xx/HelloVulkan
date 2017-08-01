#pragma once
#include "AbstractVertexBufferWrapper.hpp"

template<class T>
class PrimitiveVertexBufferWrapper :
	public AbstractVertexBufferWrapper<T>
{
public:
	void WriteVertexes();
	PrimitiveVertexBufferWrapper(LogicDeviceWrapper device, vector<T> vertexes);
	~PrimitiveVertexBufferWrapper();
};


template<class T>
inline void PrimitiveVertexBufferWrapper<T>::WriteVertexes()
{
	VkBufferCreateInfo VertexBufferInfo;
	VertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	VertexBufferInfo.pNext = NULL;
	VertexBufferInfo.flags = 0; //Есть варианты
	VertexBufferInfo.size = sizeof(T)*Vertexes.size();
	VertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;// | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	vector<uint32_t> indexes;
	if (Device.GetQueuesFamilies().size() == 1)
	{
		VertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VertexBufferInfo.queueFamilyIndexCount = 1;
		uint32_t index = Device.GetQueuesFamilies()[0].Index;
		VertexBufferInfo.pQueueFamilyIndices = &index;
	}
	else
	{
		VertexBufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		VertexBufferInfo.queueFamilyIndexCount = Device.GetQueuesFamilies().size();
		for (int i(0); i < Device.GetQueuesFamilies().size(); i++)
		{
			indexes.push_back(Device.GetQueuesFamilies()[i].Index);
		}
		VertexBufferInfo.pQueueFamilyIndices = indexes.data();
	}

	vkCreateBuffer(Device.GetLogicDevice(), &VertexBufferInfo, nullptr, &Buffer);



	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(Device.GetLogicDevice(), Buffer, &memReqs);

	VkMemoryAllocateInfo MemoryAllocInfo;
	MemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocInfo.pNext = NULL;
	MemoryAllocInfo.allocationSize = memReqs.size * 3;
	MemoryAllocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;// | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT

	vkAllocateMemory(Device.GetLogicDevice(), &MemoryAllocInfo, nullptr, &Memory);

	void *data;
	vkMapMemory(Device.GetLogicDevice(), Memory, 0, memReqs.size, 0, &data);
	memcpy(data, Vertexes.data(), sizeof(T)*Vertexes.size());
	vkUnmapMemory(Device.GetLogicDevice(), Memory);

	vkBindBufferMemory(Device.GetLogicDevice(), Buffer, Memory, 0);
}

template<class T>
PrimitiveVertexBufferWrapper<T>::PrimitiveVertexBufferWrapper(LogicDeviceWrapper device, vector<T> vertexes): AbstractVertexBufferWrapper<T>(device, vertexes)
{
}

template<class T>
inline PrimitiveVertexBufferWrapper<T>::~PrimitiveVertexBufferWrapper()
{
}
