#include "PrimitiveScene.h"


void PrimitiveScene::Draw(VkSemaphore ImageReady, VkSemaphore CalculationReady, VkQueue Queue)
{
	_ImageReady = ImageReady;
	_CalculationReady = CalculationReady;
	_Queue = Queue;

	Draw();
}

void PrimitiveScene::Draw()
{
	VkPipelineStageFlags stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

	unsigned int ImageIndex;

	logger->checkResults(vkAcquireNextImageKHR(LogicDevice.GetLogicDevice(),
		Swapchain.GetSwapchain(), UINT64_MAX, _ImageReady, NULL, &ImageIndex));


	//static_cast<LightCamera*>(cameras[0])->MainMatrixes.RotateRight();
	//static_cast<ShadowCamera*>(cameras[1])->TransformationMat.SetMatrix(static_cast<LightCamera*>(cameras[0])->MainMatrixes);


	VkSubmitInfo SubmitInfo;
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.pNext = NULL;
	SubmitInfo.waitSemaphoreCount = 1;
	SubmitInfo.pWaitSemaphores = &_ImageReady;
	SubmitInfo.pWaitDstStageMask = &stage;
	SubmitInfo.commandBufferCount = 1;
	SubmitInfo.pCommandBuffers = &ComPool.GetCommandBuffers()[ImageIndex];
	SubmitInfo.signalSemaphoreCount = 1;
	SubmitInfo.pSignalSemaphores = &_CalculationReady;



	logger->checkResults(vkQueueSubmit(_Queue, 1, &SubmitInfo, NULL));

	VkSwapchainKHR swap = Swapchain.GetSwapchain();
	VkResult* Results = new VkResult[1];

	VkPresentInfoKHR PresentInfo;
	PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	PresentInfo.pNext = NULL;
	PresentInfo.waitSemaphoreCount = 1;
	PresentInfo.pWaitSemaphores = &_CalculationReady;
	PresentInfo.swapchainCount = 1;
	PresentInfo.pSwapchains = &swap;
	PresentInfo.pImageIndices = &ImageIndex;
	PresentInfo.pResults = Results;


	logger->checkResults(vkQueuePresentKHR(_Queue, &PresentInfo));
	logger->checkResults(Results[0]);
}

VkImage PrimitiveScene::CreateDepthImage()
{
	VkImageCreateInfo DepthImageInfo;
	DepthImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	DepthImageInfo.pNext = NULL;
	DepthImageInfo.flags = 0;
	DepthImageInfo.imageType = VK_IMAGE_TYPE_2D;
	DepthImageInfo.format = VK_FORMAT_D32_SFLOAT_S8_UINT;
	DepthImageInfo.extent.height = 561;
	DepthImageInfo.extent.width = 884;
	DepthImageInfo.extent.depth = 1;
	DepthImageInfo.mipLevels = 1;
	DepthImageInfo.arrayLayers = 1;
	DepthImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	DepthImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	DepthImageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

	vector<uint32_t> indexes;
	if (LogicDevice.GetQueuesFamilies().size() == 1)
	{
		DepthImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		DepthImageInfo.queueFamilyIndexCount = 1;
		uint32_t index = LogicDevice.GetQueuesFamilies()[0].Index;
		DepthImageInfo.pQueueFamilyIndices = &index;
	}
	else
	{
		DepthImageInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		DepthImageInfo.queueFamilyIndexCount = LogicDevice.GetQueuesFamilies().size();

		for (int i(0); i < LogicDevice.GetQueuesFamilies().size(); i++)
		{
			indexes.push_back(LogicDevice.GetQueuesFamilies()[i].Index);
		}
		DepthImageInfo.pQueueFamilyIndices = indexes.data();
	}
	DepthImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;// VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkImage DepthImage;
	logger->checkResults(vkCreateImage(LogicDevice.GetLogicDevice(), &DepthImageInfo, nullptr, &DepthImage));

	VkMemoryRequirements memReqs;
	vkGetImageMemoryRequirements(LogicDevice.GetLogicDevice(), DepthImage, &memReqs);

	VkMemoryAllocateInfo MemoryAllocInfo;
	MemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocInfo.pNext = NULL;
	MemoryAllocInfo.allocationSize = memReqs.size;
	MemoryAllocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;// | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT

	VkDeviceMemory memory;
	vkAllocateMemory(LogicDevice.GetLogicDevice(), &MemoryAllocInfo, nullptr, &memory);

	vkBindImageMemory(LogicDevice.GetLogicDevice(), DepthImage, memory, 0);

	
	return DepthImage;
}

void PrimitiveScene::PreDraw(VkCommandBuffer CmdBuffer, VkImage Color)
{
	VkCommandBufferBeginInfo BeginInfo;
	BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	BeginInfo.pNext = NULL;
	BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	BeginInfo.pInheritanceInfo = NULL;

	logger->checkResults(vkBeginCommandBuffer(CmdBuffer, &BeginInfo));


	VkImageSubresourceRange ColorSubRange;
	ColorSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ColorSubRange.baseMipLevel = 0;
	ColorSubRange.levelCount = 1;
	ColorSubRange.baseArrayLayer = 0;
	ColorSubRange.layerCount = 1;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = Color;
	barrier.subresourceRange = ColorSubRange;

	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(
		CmdBuffer,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	VkClearColorValue ClearColor;
	ClearColor.float32[0] = 1.0f;
	ClearColor.float32[1] = 1.0f;
	ClearColor.float32[2] = 1.0f;
	ClearColor.float32[3] = 1.0f;

	vkCmdClearColorImage(CmdBuffer, Color, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &ClearColor, 1, &ColorSubRange);
	

	for (int i(1); i < cameras.size(); i++)
	{
		static_cast<LightCamera*>(cameras[i])->Draw(CmdBuffer, ColorViews.at(Color));
		static_cast<ShadowCamera*>(cameras[0])->SetTexture(CmdBuffer, ColorViews.at(Color));
		static_cast<ShadowCamera*>(cameras[0])->TransformationMat.SetMatrix(static_cast<LightCamera*>(cameras[i])->MainMatrixes);
		static_cast<ShadowCamera*>(cameras[0])->Draw(CmdBuffer, Color);
	}

	logger->checkResults(vkEndCommandBuffer(CmdBuffer));
}

void PrimitiveScene::Update()
{
	int j = 0;
	for (auto i : ColorViews)
	{
		PreDraw(ComPool.GetCommandBuffers()[j], i.first);
		
		j++;
	}
}

PrimitiveScene::PrimitiveScene(Logger* logger, LogicDeviceWrapper logicDevice, int ScreenX, int ScreenY, int zoom, VkSurfaceFormatKHR surfaceFormat,
	SwapchainWrapper swapchain, CommandPoolWrapper comPool, VkQueue Queue)
	:LogicDevice(logicDevice), Swapchain(swapchain), ComPool(comPool)
{
	this->logger = logger;

	MatrixBuffer MainMatrixes(logicDevice, ScreenX, ScreenX, std::numeric_limits<float>::infinity());

	VkImage DepthImage = CreateDepthImage();

	VkImageSubresourceRange SubRange;
	SubRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	SubRange.baseMipLevel = 0;
	SubRange.levelCount = 1;
	SubRange.baseArrayLayer = 0;
	SubRange.layerCount = 1;

	VkImageViewCreateInfo DepthImageViewInfo{};
	DepthImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	DepthImageViewInfo.pNext = NULL;
	DepthImageViewInfo.flags = 0;
	DepthImageViewInfo.image = DepthImage;
	DepthImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	DepthImageViewInfo.format = VK_FORMAT_D32_SFLOAT_S8_UINT;
	DepthImageViewInfo.subresourceRange = SubRange;

	VkImageView DepthView;
	vkCreateImageView(LogicDevice.GetLogicDevice(), &DepthImageViewInfo, nullptr, &DepthView);


	cameras.push_back(new ShadowCamera(logger, logicDevice, surfaceFormat, ScreenX, ScreenY, zoom, MainMatrixes,
		DepthImage, DepthView));

	//PreDraw(comPool.GetCommandBuffers()[0], Swapchain.GetImages()[0]);

	cameras.push_back(new LightCamera(logger, logicDevice, ScreenX, ScreenY));

	for (int i = 0; i < swapchain.GetImages().size(); i++)
	{
		ColorViews.insert(pair<VkImage, VkImage>(Swapchain.GetImages()[i], CreateDepthImage()));
		
		PreDraw(comPool.GetCommandBuffers()[i], Swapchain.GetImages()[i]);
	}
}

PrimitiveScene::~PrimitiveScene()
{
}
