#include "WorldBuffer.h"



void WorldBuffer::CreateBuffer()
{
	VkBufferCreateInfo BufferCreateInfo;
	BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferCreateInfo.pNext = NULL;
	BufferCreateInfo.flags = 0; //Есть варианты
	BufferCreateInfo.size = sizeof(WorldMatrix);
	BufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	vector<uint32_t> indexes;
	if (Devaice.GetQueuesFamilies().size() == 1)
	{
		BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		BufferCreateInfo.queueFamilyIndexCount = 1;
		uint32_t index = Devaice.GetQueuesFamilies()[0].Index;
		BufferCreateInfo.pQueueFamilyIndices = &index;
	}
	else
	{
		BufferCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		BufferCreateInfo.queueFamilyIndexCount = Devaice.GetQueuesFamilies().size();

		for (int i(0); i < Devaice.GetQueuesFamilies().size(); i++)
		{
			indexes.push_back(Devaice.GetQueuesFamilies()[i].Index);
		}
		BufferCreateInfo.pQueueFamilyIndices = indexes.data();
	}

	vkCreateBuffer(Devaice.GetLogicDevice(), &BufferCreateInfo, nullptr, &Buffer);

	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(Devaice.GetLogicDevice(), Buffer, &memReqs);

	VkMemoryAllocateInfo MemoryAllocInfo;
	MemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocInfo.pNext = NULL;
	MemoryAllocInfo.allocationSize = memReqs.size;
	MemoryAllocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT; //memReqs.memoryTypeBits; //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; //

	vkAllocateMemory(Devaice.GetLogicDevice(), &MemoryAllocInfo, nullptr, &memory);
	vkBindBufferMemory(Devaice.GetLogicDevice(), Buffer, memory, 0);
}

WorldBuffer::WorldBuffer(LogicDeviceWrapper LogicDevaice, glm::vec3 rotation, glm::vec3 translation): Devaice(LogicDevaice)
{
	CreateBuffer();

	SetMatrix(rotation, translation);
}

WorldBuffer::~WorldBuffer()
{
}

VkBuffer WorldBuffer::GetBuffer()
{
	return Buffer;
}

void WorldBuffer::SetMatrix(glm::vec3 rotation, glm::vec3 translation)
{
	WorldMatrix = glm::mat4();
	WorldMatrix = glm::rotate(WorldMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	WorldMatrix = glm::rotate(WorldMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	WorldMatrix = glm::rotate(WorldMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	WorldMatrix = glm::translate(WorldMatrix, translation);
	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(WorldMatrix), 0, (void **)&pData);
	memcpy(pData, &WorldMatrix, sizeof(WorldMatrix));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}
