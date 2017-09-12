#include "MatrixBuffer.h"


MatrixBuffer::~MatrixBuffer()
{
}

glm::vec3 MatrixBuffer::GetDirView()
{
	return dirView;
}

glm::vec3 MatrixBuffer::GetPosition()
{
	return position;
}

VkBuffer MatrixBuffer::GetBuffer()
{
	return Buffer;
}

void MatrixBuffer::Update()
{
	Matrixs.ViewMatrix = glm::lookAt(position, position+dirView, dirUp);

	uint8_t *pData;
	vkMapMemory(Devaice.GetLogicDevice(), memory, 0, sizeof(Matrixs), 0, (void **)&pData);
	memcpy(pData, &Matrixs, sizeof(Matrixs));
	// Unmap after data has been copied
	// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
	vkUnmapMemory(Devaice.GetLogicDevice(), memory);
}

void MatrixBuffer::GoForward()
{
	position += dirView*0.1f;
	Update();
}

void MatrixBuffer::GoBackward()
{
	position -= dirView*0.1f;
	Update();
}

void MatrixBuffer::StrafeLeft()
{
	position -= glm::cross(dirView, dirUp)*0.1f;
	Update();
}

void MatrixBuffer::StrafeRight()
{
	position += glm::cross(dirView, dirUp)*0.1f;
	Update();
}

void MatrixBuffer::Up()
{
	position += dirUp*0.1f;
	Update();
}

void MatrixBuffer::Down()
{
	position -= dirUp*0.1f;
	Update();
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
	identity = glm::rotate(identity, scale, dirUp);

	dirView = glm::vec3(identity * glm::vec4(dirView, 0.0f));
	Update();
}

void MatrixBuffer::RotateRight()
{
	glm::mat4 identity(1);
	identity = glm::rotate(identity, -scale, dirUp);

	dirView = glm::vec3(identity * glm::vec4(dirView, 0.0f));
	Update();
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
	if (zoom == std::numeric_limits<float>::infinity())
	{
		Matrixs.ProjectionMatrix = glm::infinitePerspective(glm::radians(60.0f), (float)ScreenX / (float)ScreenY, 0.1f);
	}
	else
	{
		Matrixs.ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)ScreenX / (float)ScreenY, 0.1f, zoom);
	}

	Update();
}
