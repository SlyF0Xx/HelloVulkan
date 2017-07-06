#define VK_USE_PLATFORM_WIN32_KHR

#include <Windows.h>
#include <stdlib.h>
#include<time.h>
#include <string>
#include "Logger.h"
#include "Foundation.h"
#include <vulkan\vk_platform.h>
#include <vulkan\vulkan.h>

#include "Model.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PrimitiveVertex.h"
#include "PrimitiveModel.h"
#include "PrimitivePipelineWrapper.h"

#pragma comment (lib, "vulkan-1.lib")

using namespace std;


const int ScreenX = 900;
const int ScreenY = 600;
const float zoom = -2.5f;
glm::vec3 rotation = glm::vec3();

#ifdef _WIN32
WNDCLASSEX windowsclass;
MSG msg;
HWND hwnd;
HINSTANCE HInstance;
#endif

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}


	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

Logger logger;

VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t object,
	size_t location,
	int32_t messageCode,
	const char *pLayerPrefix,
	const char *pMessage,
	void *pUserData)
{
	logger.LogMessage(pMessage);
	return VK_FALSE;
}


struct MatrixesBufer
{
	struct Matrixes
	{
		glm::mat4 WorldMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
	}Matrixs;
	VkBuffer MatrixBuffer;
	VkDeviceMemory memory;
	vector<VkDescriptorSetLayout> Descriptors;
	VkDescriptorSet Descriptor;

	MatrixesBufer(LogicDeviceWrapper LogicDevaice)
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

		vkCreateBuffer(LogicDevaice.GetLogicDevice(), &BufferCreateInfo, nullptr, &MatrixBuffer);

		VkMemoryRequirements memReqs;
		vkGetBufferMemoryRequirements(LogicDevaice.GetLogicDevice(), MatrixBuffer, &memReqs);

		VkMemoryAllocateInfo MemoryAllocInfo;
		MemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		MemoryAllocInfo.pNext = NULL;
		MemoryAllocInfo.allocationSize = memReqs.size;
		MemoryAllocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT; //memReqs.memoryTypeBits; //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; //

		vkAllocateMemory(LogicDevaice.GetLogicDevice(), &MemoryAllocInfo, nullptr, &memory);
		vkBindBufferMemory(LogicDevaice.GetLogicDevice(), MatrixBuffer, memory, 0);

		vector<VkDescriptorSetLayoutBinding> Bindings;
		Bindings.push_back(VkDescriptorSetLayoutBinding());
		Bindings[0].binding = 0;
		Bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		Bindings[0].descriptorCount = 1;
		Bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		Bindings[0].pImmutableSamplers = NULL;

		VkDescriptorSetLayoutCreateInfo DescInfo{};
		DescInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		DescInfo.pNext = NULL;
		DescInfo.flags = 0; //Возможен флаг
		DescInfo.bindingCount = Bindings.size();
		DescInfo.pBindings = Bindings.data();

		Descriptors.push_back(VkDescriptorSetLayout());
		vkCreateDescriptorSetLayout(LogicDevaice.GetLogicDevice(), &DescInfo, nullptr, &Descriptors.back());

		VkDescriptorBufferInfo BufferDescriptor;
		BufferDescriptor.buffer = MatrixBuffer;
		BufferDescriptor.offset = 0;
		BufferDescriptor.range = VK_WHOLE_SIZE;

		vector<VkDescriptorPoolSize> typeCounts;
		// This example only uses one descriptor type (uniform buffer) and only requests one descriptor of this type
		typeCounts.push_back(VkDescriptorPoolSize());
		typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
		typeCounts[0].descriptorCount = 1;

		VkDescriptorPoolCreateInfo DescPoolInfo;
		DescPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		DescPoolInfo.pNext = NULL;
		DescPoolInfo.flags = 0; //Есть варианты
		DescPoolInfo.maxSets = 1;
		DescPoolInfo.poolSizeCount = typeCounts.size();
		DescPoolInfo.pPoolSizes = typeCounts.data();

		VkDescriptorPool DescPool;
		vkCreateDescriptorPool(LogicDevaice.GetLogicDevice(), &DescPoolInfo, nullptr, &DescPool);

		VkDescriptorSetAllocateInfo DescriptorInfo;
		DescriptorInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		DescriptorInfo.pNext = NULL;
		DescriptorInfo.descriptorPool = DescPool;
		DescriptorInfo.descriptorSetCount = Descriptors.size();
		DescriptorInfo.pSetLayouts = Descriptors.data();

		vkAllocateDescriptorSets(LogicDevaice.GetLogicDevice(), &DescriptorInfo, &Descriptor);

		VkWriteDescriptorSet WriteDescriptorSetInfo;
		WriteDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		WriteDescriptorSetInfo.pNext = NULL;
		WriteDescriptorSetInfo.dstSet = Descriptor;
		WriteDescriptorSetInfo.dstBinding = 0;
		WriteDescriptorSetInfo.dstArrayElement = 0;
		WriteDescriptorSetInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
		WriteDescriptorSetInfo.descriptorCount = 1;
		WriteDescriptorSetInfo.pBufferInfo = &BufferDescriptor;
		WriteDescriptorSetInfo.pImageInfo = NULL;
		WriteDescriptorSetInfo.pTexelBufferView = NULL;

		vkUpdateDescriptorSets(LogicDevaice.GetLogicDevice(), 1, &WriteDescriptorSetInfo, 0, nullptr);
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	windowsclass.cbSize = sizeof(windowsclass);
	windowsclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	windowsclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowsclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowsclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	windowsclass.hInstance = hInstance;
	windowsclass.lpfnWndProc = WinProc;
	windowsclass.lpszClassName = L"windowsclass";
	windowsclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowsclass);

	hwnd = CreateWindow(
		L"windowsclass",
		L"VulkanApp",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100,
		ScreenX,
		ScreenY,
		NULL,
		NULL,
		hInstance,
		NULL);
	HInstance = hInstance;

	logger.LogMessage("Start");

	try {
		Foundation foundation = Foundation(&logger);
		
		
		PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = NULL;
		PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = NULL;
		fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)
			vkGetInstanceProcAddr(foundation.GetInstance(), "vkCreateDebugReportCallbackEXT");
		fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)
			vkGetInstanceProcAddr(foundation.GetInstance(), "vkDestroyDebugReportCallbackEXT");

		VkDebugReportCallbackCreateInfoEXT debug_report_callback_info;
		memset(&debug_report_callback_info, 0, sizeof(debug_report_callback_info));
		debug_report_callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debug_report_callback_info.flags = VK_DEBUG_REPORT_DEBUG_BIT_EXT |
			VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
		debug_report_callback_info.pfnCallback = DebugReportCallback;

		VkDebugReportCallbackEXT debug_report_callback = VK_NULL_HANDLE;
		logger.checkResults(fvkCreateDebugReportCallbackEXT(foundation.GetInstance(), &debug_report_callback_info, nullptr, &debug_report_callback));
		

		foundation.CreateDevices();
		logger.LogMessage("Generate Devacie");
		
		
		foundation.addSurface(HInstance, hwnd);

		logger.LogMessage("Generate Surface");
		logger.LogMessage(foundation.GetSurfaces()[0]);

		for (auto &i : foundation.GetPhysDevices())
		{
			i.addLogicDevice(1);
			logger.LogMessage("Generate Logic Devacie");
			logger.LogMessage(i.GetLogicDevices()[0].GetLogicDevice());

			for (auto &j : i.GetLogicDevices())
			{
				foundation.addSwapchain(i.GetPhysDevice(), foundation.GetSurfaces()[0], &j);
				logger.LogMessage("Generate Swapchain");
				for (auto k : j.GetQueuesFamilies())
				{
					j.addCommandPool(k.Index);
					logger.LogMessage("Generate Queue Family");
				}
				for (CommandPoolWrapper& k : j.GetCommandPools())
				{
					j.addCommandBuffers(&k, true, j.GetSwapchins()[0].GetImages().size());					
					logger.LogMessage("Generate Command Pool");
				}
			}
		}


		ZeroMemory(&msg, sizeof(msg));

		ShowWindow(hwnd, nShowCmd);
		UpdateWindow(hwnd);
		ZeroMemory(&msg, sizeof(msg));


		logger.LogMessage("Generate");


		const VkFormat DepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
		
		PrimitiveModel* mode = new PrimitiveModel(foundation.GetPhysDevices()[0].GetLogicDevices()[0]);

		MatrixesBufer MatrixesMainBufer = MatrixesBufer(foundation.GetPhysDevices()[0].GetLogicDevices()[0]);
		
		PrimitivePipelineWrapper Pipe(&logger, foundation.GetPhysDevices()[0].GetLogicDevices()[0], foundation.GetSurfaceFormat(), DepthFormat, MatrixesMainBufer.Descriptors);
		
		Pipe.Models.push_back((AbstractModelBase<PrimitiveVertex>*)mode);



		Model model(&logger, foundation.GetPhysDevices()[0].GetLogicDevices()[0], foundation.GetSurfaceFormat(), DepthFormat, MatrixesMainBufer.Descriptors);

		logger.LogMessage("Done");


		VkSemaphoreCreateInfo SemaphoreInfo;
		SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		SemaphoreInfo.pNext = NULL;
		SemaphoreInfo.flags = 0;

		VkSemaphore ImageReady;
		VkSemaphore CalculationReady;
		logger.checkResults(vkCreateSemaphore(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &SemaphoreInfo, nullptr, &ImageReady));
		logger.checkResults(vkCreateSemaphore(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &SemaphoreInfo, nullptr, &CalculationReady));

		VkImageSubresourceRange SubRange;
		SubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		SubRange.baseMipLevel = 0;
		SubRange.levelCount = 1;
		SubRange.baseArrayLayer = 0;
		SubRange.layerCount = 1;

		VkImageSubresourceRange DepthSubRange;
		DepthSubRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		DepthSubRange.baseMipLevel = 0;
		DepthSubRange.levelCount = 1;
		DepthSubRange.baseArrayLayer = 0;
		DepthSubRange.layerCount = 1;

		vector<VkClearValue> clearValues;
		clearValues.push_back(VkClearValue());
		clearValues[0].color = { { 0.1f, 0.0f, 0.2f, 0.1f } };
		clearValues.push_back(VkClearValue());
		clearValues[1].depthStencil = { 1.0f, 0 };


		VkCommandBufferBeginInfo BeginInfo;
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		BeginInfo.pNext = NULL;
		BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		BeginInfo.pInheritanceInfo = NULL;
		VkClearColorValue color = { 1.0f, 0.5f, 0.0f};

		logger.LogMessage("Prepare");
		ofstream fout("test.txt");

		vector<VkFramebuffer> Framebeffers;
		for (int i(0); i < foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers().size(); i++)
		{
			vector<VkImageView> ImageViews;
			
			VkImageViewCreateInfo ColorImageViewInfo{};
			ColorImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ColorImageViewInfo.pNext = NULL;
			ColorImageViewInfo.flags = 0;
			ColorImageViewInfo.image = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0].GetImages()[i];
			ColorImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			ColorImageViewInfo.format = foundation.GetSurfaceFormat().format;
			ColorImageViewInfo.subresourceRange = SubRange;

			VkImageView ColorView;
			vkCreateImageView(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &ColorImageViewInfo, nullptr, &ColorView);
			ImageViews.push_back(ColorView);



			VkImageCreateInfo DepthImageInfo;
			DepthImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			DepthImageInfo.pNext = NULL;
			DepthImageInfo.flags = 0;
			DepthImageInfo.imageType = VK_IMAGE_TYPE_2D;
			DepthImageInfo.format = DepthFormat;
			DepthImageInfo.extent.height = 561;
			DepthImageInfo.extent.width = 884;
			DepthImageInfo.extent.depth = 1;
			DepthImageInfo.mipLevels = 1;
			DepthImageInfo.arrayLayers = 1;
			DepthImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			DepthImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			DepthImageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

			vector<uint32_t> indexes;
			if (foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies().size() == 1)
			{
				DepthImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				DepthImageInfo.queueFamilyIndexCount = 1;
				uint32_t index = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Index;
				DepthImageInfo.pQueueFamilyIndices = &index;
			}
			else
			{
				DepthImageInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
				DepthImageInfo.queueFamilyIndexCount = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies().size();
				
				for (int i(0); i < foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies().size(); i++)
				{
					indexes.push_back(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[i].Index);
				}
				DepthImageInfo.pQueueFamilyIndices = indexes.data();
			}
			DepthImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;// VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			VkImage DepthImage;
			logger.checkResults(vkCreateImage(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &DepthImageInfo, nullptr, &DepthImage));

			VkMemoryRequirements memReqs;
			vkGetImageMemoryRequirements(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), DepthImage, &memReqs);
			
			VkMemoryAllocateInfo MemoryAllocInfo;
			MemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			MemoryAllocInfo.pNext = NULL;
			MemoryAllocInfo.allocationSize = memReqs.size;
			MemoryAllocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;// | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT

			VkDeviceMemory memory;
			vkAllocateMemory(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &MemoryAllocInfo, nullptr, &memory);

			vkBindImageMemory(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), DepthImage, memory, 0);
			
			VkImageViewCreateInfo DepthImageViewInfo{};
			DepthImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			DepthImageViewInfo.pNext = NULL;
			DepthImageViewInfo.flags = 0;
			DepthImageViewInfo.image = DepthImage;
			DepthImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			DepthImageViewInfo.format = DepthFormat;
			DepthImageViewInfo.subresourceRange = DepthSubRange;

			VkImageView DepthView;
			vkCreateImageView(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &DepthImageViewInfo, nullptr, &DepthView);
			ImageViews.push_back(DepthView);

			VkFramebufferCreateInfo FramebufferCreateInfo;
			FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			FramebufferCreateInfo.pNext = NULL;
			FramebufferCreateInfo.flags = 0;
			FramebufferCreateInfo.renderPass = model.GetRenderPass();
			FramebufferCreateInfo.attachmentCount = ImageViews.size();
			FramebufferCreateInfo.pAttachments = ImageViews.data();
			FramebufferCreateInfo.height = 561;
			FramebufferCreateInfo.width = 884;
			FramebufferCreateInfo.layers = 1;

			VkFramebuffer FrameBuffer;
			vkCreateFramebuffer(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &FramebufferCreateInfo, nullptr, &FrameBuffer);

			VkRenderPassBeginInfo RenderBeginInfo;
			RenderBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			RenderBeginInfo.pNext = NULL;
			RenderBeginInfo.renderPass = model.GetRenderPass();
			RenderBeginInfo.renderArea.offset.x = 0;
			RenderBeginInfo.renderArea.offset.y = 0;
			RenderBeginInfo.renderArea.extent.height = 561;
			RenderBeginInfo.renderArea.extent.width = 884;
			RenderBeginInfo.clearValueCount = clearValues.size();
			RenderBeginInfo.pClearValues = clearValues.data();
			RenderBeginInfo.framebuffer = FrameBuffer;


			/*
			VkImageMemoryBarrier BeforeBarier;
			BeforeBarier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			BeforeBarier.pNext = NULL;
			BeforeBarier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			BeforeBarier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			BeforeBarier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			BeforeBarier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			BeforeBarier.srcQueueFamilyIndex = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Index;
			BeforeBarier.dstQueueFamilyIndex = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Index; //Надо бы поиграть
			BeforeBarier.image = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0].GetImages()[i];
			BeforeBarier.subresourceRange = SubRange;

			VkImageMemoryBarrier AfterBarier;
			AfterBarier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			AfterBarier.pNext = NULL;
			AfterBarier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			AfterBarier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			AfterBarier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			AfterBarier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			AfterBarier.srcQueueFamilyIndex = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Index;
			AfterBarier.dstQueueFamilyIndex = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Index;
			AfterBarier.image = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0].GetImages()[i];
			AfterBarier.subresourceRange = SubRange;
			*/

			logger.LogMessage("Prepare for Begin");
			logger.checkResults(vkBeginCommandBuffer(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], &BeginInfo));


				
				vkCmdBeginRenderPass(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], &RenderBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
				
					// Update dynamic viewport state
					VkViewport viewport = {};
					viewport.height = (float)561;
					viewport.width = (float)884;
					viewport.minDepth = (float) 0.0f;
					viewport.maxDepth = (float) 1.0f;
					vkCmdSetViewport(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], 0, 1, &viewport);

					// Update dynamic scissor state
					VkRect2D scissor = {};
					scissor.extent.width = 884;
					scissor.extent.height = 561;
					scissor.offset.x = 0; 
					scissor.offset.y = 0;
					vkCmdSetScissor(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], 0, 1, &scissor);

					vkCmdBindDescriptorSets(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
						model.GetPipelineLayout(), 0, 1, &MatrixesMainBufer.Descriptor, 0, nullptr);

					//ВЫНЕСТИ ОТДЕЛЬНО!!
					vkCmdBindPipeline(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], VK_PIPELINE_BIND_POINT_GRAPHICS, model.GetPipeline());

					VkBuffer temp = model.GetVertBuf().Buffer;
					VkDeviceSize offsets[1] = { 0 };
					vkCmdBindVertexBuffers(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], 0, 1, &temp, offsets);
					vkCmdBindIndexBuffer(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], model.GetIndexBuf().Buffer,0, VK_INDEX_TYPE_UINT32);

					//vkCmdPipelineBarrier(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
					//	VK_PIPELINE_STAGE_TRANSFER_BIT, 0 , 0, nullptr, 0, nullptr, 1, &BeforeBarier);
					//								//VK_DEPENDENCY_VIEW_LOCAL_BIT_KHX
			
					//vkCmdClearColorImage(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i],
					//	foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0].GetImages()[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &color, 1, &SubRange);

					vkCmdDrawIndexed(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], 3, 1, 0, 0, 1);

					//vkCmdPipelineBarrier(
					//	foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i], VK_PIPELINE_STAGE_TRANSFER_BIT,
					//	VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &AfterBarier);
					//						//VK_DEPENDENCY_VIEW_LOCAL_BIT_KHX

				vkCmdEndRenderPass(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i]);

			logger.checkResults(vkEndCommandBuffer(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[i]));
		}

		VkPipelineStageFlags stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

		unsigned int ImageIndex;

		logger.LogMessage("All prepare for draw");

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			else
			{
				logger.LogMessage("Start draw");
				logger.checkResults(vkAcquireNextImageKHR(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(),
					foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0].GetSwapchain(), UINT64_MAX, ImageReady, NULL, &ImageIndex));
				
					// Update matrices
					MatrixesMainBufer.Matrixs.ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)ScreenX / (float)ScreenY, 0.1f, 256.0f);

					MatrixesMainBufer.Matrixs.ViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, zoom));

					MatrixesMainBufer.Matrixs.WorldMatrix = glm::mat4();
					MatrixesMainBufer.Matrixs.WorldMatrix = glm::rotate(MatrixesMainBufer.Matrixs.WorldMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
					MatrixesMainBufer.Matrixs.WorldMatrix = glm::rotate(MatrixesMainBufer.Matrixs.WorldMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
					MatrixesMainBufer.Matrixs.WorldMatrix = glm::rotate(MatrixesMainBufer.Matrixs.WorldMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

					// Map uniform buffer and update it
					uint8_t *pData;
					vkMapMemory(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), MatrixesMainBufer.memory, 0, sizeof(MatrixesMainBufer.Matrixs), 0, (void **)&pData);
					memcpy(pData, &MatrixesMainBufer.Matrixs, sizeof(MatrixesMainBufer.Matrixs));
					// Unmap after data has been copied
					// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
					vkUnmapMemory(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), MatrixesMainBufer.memory);

					glm::mat4 Other = MatrixesMainBufer.Matrixs.ProjectionMatrix * MatrixesMainBufer.Matrixs.ViewMatrix * MatrixesMainBufer.Matrixs.WorldMatrix;
					glm::vec4 vector = MatrixesMainBufer.Matrixs.ProjectionMatrix  * glm::vec4(model.GetVertBuf().vertexes[0].position[0], model.GetVertBuf().vertexes[0].position[1], model.GetVertBuf().vertexes[0].position[2], 1.0f);
				VkSubmitInfo SubmitInfo;
				SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				SubmitInfo.pNext = NULL;
				SubmitInfo.waitSemaphoreCount = 1;
				SubmitInfo.pWaitSemaphores = &ImageReady;
				SubmitInfo.pWaitDstStageMask = &stage;
				SubmitInfo.commandBufferCount = 1;
				SubmitInfo.pCommandBuffers = &foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0].GetCommandBuffers()[ImageIndex];
				SubmitInfo.signalSemaphoreCount = 1;
				SubmitInfo.pSignalSemaphores = &CalculationReady;



				logger.checkResults(vkQueueSubmit(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Queues[0], 1, &SubmitInfo ,NULL));

				VkSwapchainKHR swap = foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0].GetSwapchain();
				VkResult* Results = new VkResult[1];

				VkPresentInfoKHR PresentInfo;
				PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
				PresentInfo.pNext = NULL;
				PresentInfo.waitSemaphoreCount = 1;
				PresentInfo.pWaitSemaphores = &CalculationReady;
				PresentInfo.swapchainCount = 1;
				PresentInfo.pSwapchains = &swap;
				PresentInfo.pImageIndices = &ImageIndex;
				PresentInfo.pResults = Results;

				
				logger.checkResults(vkQueuePresentKHR(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Queues[0], &PresentInfo));
				logger.checkResults(Results[0]);
			}
		}

	}	
	catch (exception e)
	{
		logger.LogMessage((string) e.what());
	}
}