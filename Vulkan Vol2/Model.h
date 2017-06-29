#pragma once
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif // _WIN32

#include <vulkan\vulkan.h>
#include <vulkan\vk_platform.h>
#include <vector>
#include "Logger.h"
#include <string>
#include "Foundation.h"

#pragma comment (lib, "vulkan-1.lib")

using namespace std;


struct VertexBuffer
{
	struct Vertex
	{
		float position[3];
	};
	vector<Vertex> vertexes;
	VkBuffer Buffer;
	VkDeviceMemory memory;

	VertexBuffer(LogicDeviceWrapper Device)
	{
		vertexes =
		{
			{ { 1.0f,  1.0f, 0.0f } },
			{ { -1.0f,  1.0f, 0.0f } },
			{ { 0.0f, -1.0f, 0.0f } }
		};
		
		
		//{ { 0.0, 0.0, -10.0 },{ 0.0,5.0, -10.0 },{ 5.0,0.0, -10.0 }};

		VkBufferCreateInfo VertexBufferInfo;
		VertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		VertexBufferInfo.pNext = NULL;
		VertexBufferInfo.flags = 0; //Есть варианты
		VertexBufferInfo.size = sizeof(Vertex)*vertexes.size();
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
		MemoryAllocInfo.allocationSize = memReqs.size*3;
		MemoryAllocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;// | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT

		vkAllocateMemory(Device.GetLogicDevice(), &MemoryAllocInfo, nullptr, &memory);

		void *data;
		vkMapMemory(Device.GetLogicDevice(), memory, 0, memReqs.size, 0, &data);
		memcpy(data, vertexes.data(), sizeof(Vertex)*vertexes.size());
		vkUnmapMemory(Device.GetLogicDevice(), memory);

		vkBindBufferMemory(Device.GetLogicDevice(), Buffer, memory, 0);
	}
};

struct IndexBuffer {
	vector<uint32_t> indexes;
	VkBuffer Buffer;
	VkDeviceMemory memory;
	IndexBuffer(LogicDeviceWrapper Device)
	{
		indexes = {0,1,2};
		VkBufferCreateInfo IndexBufferInfo;
		IndexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		IndexBufferInfo.pNext = NULL;
		IndexBufferInfo.flags = 0; //Есть варианты
		IndexBufferInfo.size = sizeof(uint32_t)*indexes.size();
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
		memcpy(data, indexes.data(), sizeof(uint32_t)*indexes.size());
		vkUnmapMemory(Device.GetLogicDevice(), memory);

		vkBindBufferMemory(Device.GetLogicDevice(), Buffer, memory, 0);
	}
};

class Model
{
private:
	Logger *logger;
	VkShaderModule loadSPIRVShader(std::string filename, VkDevice device);
	VkPipeline Pipeline;
	VkRenderPass RenderPass;
	VkPipelineLayout PipelineLayout;
	vector<VkDescriptorSetLayout> Descriptors;
	VertexBuffer VertBuf;
	LogicDeviceWrapper Device;
	IndexBuffer IndexBuf;
public:
	IndexBuffer GetIndexBuf();
	VertexBuffer GetVertBuf();
	VkPipelineLayout GetPipelineLayout();
	VkRenderPass GetRenderPass();
	VkPipeline GetPipeline();
	Model(Logger *logger, LogicDeviceWrapper device, VkSurfaceFormatKHR SurfaceFormat, VkFormat DepthFormat, vector<VkDescriptorSetLayout> Descriptors);
	~Model();
};

