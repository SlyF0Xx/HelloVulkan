#include "MatrixBuffer.h"


MatrixBuffer::~MatrixBuffer()
{
}

VkBuffer MatrixBuffer::GetBuffer()
{
	return Buffer;
}

void MatrixBuffer::GoForward()
{
	Matrixs.ViewMatrix = glm::translate(Matrixs.ViewMatrix,  glm::vec3(Matrixs.ViewMatrix[2]));//glm::vec3(0.0f, 0.0f, 1.0f));// 
	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::GoBackward()
{
	Matrixs.ViewMatrix = glm::translate(Matrixs.ViewMatrix, -glm::vec3(Matrixs.ViewMatrix[2]));

	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::StrafeLeft()
{
	Matrixs.ViewMatrix = glm::translate(Matrixs.ViewMatrix, glm::vec3(Matrixs.ViewMatrix[0]));

	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::StrafeRight()
{
	Matrixs.ViewMatrix = glm::translate(Matrixs.ViewMatrix, -glm::vec3(Matrixs.ViewMatrix[0]));

	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::Up()
{
	Matrixs.ViewMatrix = glm::translate(Matrixs.ViewMatrix, glm::vec3(Matrixs.ViewMatrix[1]));
	
	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::Down()
{
	Matrixs.ViewMatrix = glm::translate(Matrixs.ViewMatrix, -glm::vec3(Matrixs.ViewMatrix[1]));

	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::RotateUp()
{
	glm::mat4 identity(1);
	identity = glm::rotate(identity, scale, glm::vec3(Matrixs.ViewMatrix[0]));
	Matrixs.ViewMatrix[1] = identity* Matrixs.ViewMatrix[1];
	Matrixs.ViewMatrix[2] = identity* -Matrixs.ViewMatrix[2];
	Matrixs.ViewMatrix[3] = identity* Matrixs.ViewMatrix[3];

	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::RotateDown()
{
	glm::mat4 identity(1);
	identity = glm::rotate(identity, scale, -glm::vec3(Matrixs.ViewMatrix[0]));
	Matrixs.ViewMatrix[1] = identity* Matrixs.ViewMatrix[1];
	Matrixs.ViewMatrix[2] = identity* -Matrixs.ViewMatrix[2];
	Matrixs.ViewMatrix[3] = identity* Matrixs.ViewMatrix[3];

	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::RotateLeft()
{
	glm::mat4 identity(1);
	identity = glm::rotate(identity, scale, -glm::vec3(Matrixs.ViewMatrix[1]));
	Matrixs.ViewMatrix[0] = identity* Matrixs.ViewMatrix[0];
	Matrixs.ViewMatrix[3] = identity* Matrixs.ViewMatrix[3];


	glm::mat4 identit(1);
	identit = glm::rotate(identit, scale, glm::vec3(Matrixs.ViewMatrix[1]));
	Matrixs.ViewMatrix[2] = identit* Matrixs.ViewMatrix[2];

	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::RotateRight()
{
	glm::mat4 identity(1);
	identity = glm::rotate(identity, scale, glm::vec3(Matrixs.ViewMatrix[1]));
	Matrixs.ViewMatrix[0] = identity* Matrixs.ViewMatrix[0];
	Matrixs.ViewMatrix[3] = identity* Matrixs.ViewMatrix[3];


	glm::mat4 identit(1);
	identit = glm::rotate(identit, scale, -glm::vec3(Matrixs.ViewMatrix[1]));
	Matrixs.ViewMatrix[2] = identit* Matrixs.ViewMatrix[2];

	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::RotateZUp()
{
}

void MatrixBuffer::RotateZDown()
{
}

MatrixBuffer::MatrixBuffer(LogicDeviceWrapper LogicDevaice, int ScreenX, int ScreenY, float zoom):Devaice(LogicDevaice)
{
	VkBufferCreateInfo BufferCreateInfo;
	BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferCreateInfo.pNext = NULL;
	BufferCreateInfo.flags = 0; //Есть варианты
	BufferCreateInfo.size = sizeof(Matrixes);
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



	// Update matrices
	Matrixs.ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)ScreenX / (float)ScreenY, 0.1f, 256.0f);
	Matrixs.ViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, zoom));

	/*
	Matrixs.WorldMatrix = glm::mat4();
	Matrixs.WorldMatrix = glm::rotate(Matrixs.WorldMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	Matrixs.WorldMatrix = glm::rotate(Matrixs.WorldMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	Matrixs.WorldMatrix = glm::rotate(Matrixs.WorldMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	*/
	// Map uniform buffer and update it
	uint8_t *pData;
	vkMapMemory(LogicDevaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(LogicDevaice.GetLogicDevice(), memory);
}
