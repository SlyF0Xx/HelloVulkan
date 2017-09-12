#include "TransformationBuffer.h"


TransformationBuffer::TransformationBuffer(LogicDeviceWrapper LogicDevaice, MatrixBuffer MainMatrixes)
	:Devaice(LogicDevaice)
{
	VkBufferCreateInfo BufferCreateInfo;
	BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferCreateInfo.pNext = NULL;
	BufferCreateInfo.flags = 0; //Есть варианты

	BufferCreateInfo.size = 92;
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

	SetMatrix(MainMatrixes);
}

TransformationBuffer::~TransformationBuffer()
{
}

VkBuffer TransformationBuffer::GetBuffer()
{
	return Buffer;
}

void TransformationBuffer::SetMatrix(MatrixBuffer MainMatrixes)
{
	light.TransformationMatrix = MainMatrixes.Matrixs.ProjectionMatrix * MainMatrixes.Matrixs.ViewMatrix;
	light.LightPos = MainMatrixes.GetPosition();
	light.LightDir = MainMatrixes.GetDirView();

	
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, 92, 0, (void **)&pData);
	memcpy(pData, &light, 76);

	memcpy(reinterpret_cast<uint8_t*>(reinterpret_cast<char *>(pData)+80), reinterpret_cast<char *>(&light)+76, 12);


	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}