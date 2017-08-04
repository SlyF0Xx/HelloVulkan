#include "WorldBuffer.h"



WorldBuffer::WorldBuffer(LogicDeviceWrapper LogicDevaice, glm::vec3 rotation): Devaice(LogicDevaice)
{
	VkBufferCreateInfo BufferCreateInfo;
	BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferCreateInfo.pNext = NULL;
	BufferCreateInfo.flags = 0; //Есть варианты
	BufferCreateInfo.size = sizeof(WorldMatrix);
	BufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	vector<uint32_t> indexes;
	if (LogicDevaice.GetQueuesFamilies().size() == 1)
	{
		BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		BufferCreateInfo.queueFamilyIndexCount = 1;
		uint32_t index = LogicDevaice.GetQueuesFamilies()[0].Index;
		BufferCreateInfo.pQueueFamilyIndices = &index;
	}
	else
	{
		BufferCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		BufferCreateInfo.queueFamilyIndexCount = LogicDevaice.GetQueuesFamilies().size();

		for (int i(0); i < LogicDevaice.GetQueuesFamilies().size(); i++)
		{
			indexes.push_back(LogicDevaice.GetQueuesFamilies()[i].Index);
		}
		BufferCreateInfo.pQueueFamilyIndices = indexes.data();
	}

	vkCreateBuffer(LogicDevaice.GetLogicDevice(), &BufferCreateInfo, nullptr, &Buffer);

	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(LogicDevaice.GetLogicDevice(), Buffer, &memReqs);

	VkMemoryAllocateInfo MemoryAllocInfo;
	MemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocInfo.pNext = NULL;
	MemoryAllocInfo.allocationSize = memReqs.size;
	MemoryAllocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT; //memReqs.memoryTypeBits; //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; //

	vkAllocateMemory(LogicDevaice.GetLogicDevice(), &MemoryAllocInfo, nullptr, &memory);
	vkBindBufferMemory(LogicDevaice.GetLogicDevice(), Buffer, memory, 0);


	WorldMatrix = glm::mat4();
	WorldMatrix = glm::rotate(WorldMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	WorldMatrix = glm::rotate(WorldMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	WorldMatrix = glm::rotate(WorldMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(LogicDevaice.GetLogicDevice(), memory, 0, sizeof(WorldMatrix), 0, (void **)&pData);
	memcpy(pData, &WorldMatrix, sizeof(WorldMatrix));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(LogicDevaice.GetLogicDevice(), memory);
}

WorldBuffer::~WorldBuffer()
{
}

VkBuffer WorldBuffer::GetBuffer()
{
	return Buffer;
}
