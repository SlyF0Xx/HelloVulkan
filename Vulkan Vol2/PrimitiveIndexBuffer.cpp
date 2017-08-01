#include "PrimitiveIndexBuffer.h"

void PrimitiveIndexBuffer::WriteIndexes()
{
	VkBufferCreateInfo IndexBufferInfo;
	IndexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	IndexBufferInfo.pNext = NULL;
	IndexBufferInfo.flags = 0; //Есть варианты
	IndexBufferInfo.size = sizeof(uint32_t)*Indexes.size();
	IndexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	vector<uint32_t> indexForMany;
	if (Device.GetQueuesFamilies().size() == 1)
	{
		IndexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		IndexBufferInfo.queueFamilyIndexCount = 1;
		uint32_t index = Device.GetQueuesFamilies()[0].Index;
		IndexBufferInfo.pQueueFamilyIndices = &index;
	}
	else
	{
		IndexBufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		IndexBufferInfo.queueFamilyIndexCount = Device.GetQueuesFamilies().size();

		for (int i(0); i < Device.GetQueuesFamilies().size(); i++)
		{
			indexForMany.push_back(Device.GetQueuesFamilies()[i].Index);
		}
		IndexBufferInfo.pQueueFamilyIndices = indexForMany.data();
	}

	vkCreateBuffer(Device.GetLogicDevice(), &IndexBufferInfo, nullptr, &Buffer);

	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(Device.GetLogicDevice(), Buffer, &memReqs);

	VkMemoryAllocateInfo MemoryAllocInfo;
	MemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocInfo.pNext = NULL;
	MemoryAllocInfo.allocationSize = memReqs.size;
	MemoryAllocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;// | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT

	vkAllocateMemory(Device.GetLogicDevice(), &MemoryAllocInfo, nullptr, &memory);

	void *data;
	vkMapMemory(Device.GetLogicDevice(), memory, 0, memReqs.size, 0, &data);
	memcpy(data, Indexes.data(), sizeof(uint32_t)*Indexes.size());
	vkUnmapMemory(Device.GetLogicDevice(), memory);

	vkBindBufferMemory(Device.GetLogicDevice(), Buffer, memory, 0);
}

PrimitiveIndexBuffer::PrimitiveIndexBuffer(LogicDeviceWrapper device, vector<uint32_t> indexes) : AbstractIndexBufferWrapper(device, indexes)
{
}


PrimitiveIndexBuffer::~PrimitiveIndexBuffer()
{
}
