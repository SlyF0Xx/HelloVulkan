#include "ShadowCamera.h"




ShadowCamera::ShadowCamera(Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR surfaceFormat, 
	int ScreenX, int ScreenY, float zoom, MatrixBuffer _MainMatrixes, VkImage depthTexture, VkImageView texture)
	:Device(device), MainMatrixes(device, ScreenX, ScreenY, zoom), TransformationMat(device, _MainMatrixes), SurfaceFormat(surfaceFormat), DepthTexture(depthTexture), DethView(texture)
{
	this->logger = logger;

	Pipelines.push_back(new ShadowPrimitivePipeline(logger, device, surfaceFormat, MainMatrixes.GetBuffer(), TransformationMat.GetBuffer(), depthTexture, texture));
	Pipelines.push_back(new ShadowColorPipeline(logger, device, surfaceFormat, MainMatrixes.GetBuffer(), TransformationMat.GetBuffer(), depthTexture, texture));
	Pipelines.push_back(new MaterialPipelineWrapper("Material.vert.spv", "Material.frag.spv", logger, device, surfaceFormat,
		MainMatrixes.GetBuffer(), TransformationMat.GetBuffer(), depthTexture, texture,
		PrimitiveMaterial(logger, Device, "NormalMap.jpg",
			glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.9f, 0.9f, 0.9f), "Texture.jpg")));
}

void ShadowCamera::SetTexture(VkCommandBuffer CmdBuffer, VkImage depthTexture)
{
	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = depthTexture;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

	vkCmdPipelineBarrier(
		CmdBuffer,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = DepthTexture;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

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

	VkImageSubresourceLayers lay;
	lay.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	lay.mipLevel = 0;
	lay.baseArrayLayer = 0;
	lay.layerCount = 1;

	VkOffset3D offset;
	offset.x = 0;
	offset.y = 0;
	offset.z = 0;

	VkExtent3D extend;
	extend.width = 884;
	extend.height = 561;
	extend.depth = 1;

	VkImageCopy copy;
	copy.srcSubresource = lay;
	copy.srcOffset = offset;
	copy.dstSubresource = lay;
	copy.dstOffset = offset;
	copy.extent = extend;

	vkCmdCopyImage(CmdBuffer, depthTexture, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, DepthTexture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = depthTexture;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

	vkCmdPipelineBarrier(
		CmdBuffer,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = DepthTexture;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

	vkCmdPipelineBarrier(
		CmdBuffer,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);
}

void ShadowCamera::Draw(VkCommandBuffer CmdBuffer, VkImage image)
{
	VkImageSubresourceRange SubRange;
	SubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	SubRange.baseMipLevel = 0;
	SubRange.levelCount = 1;
	SubRange.baseArrayLayer = 0;
	SubRange.layerCount = 1;

	VkImageViewCreateInfo ColorImageViewInfo{};
	ColorImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ColorImageViewInfo.pNext = NULL;
	ColorImageViewInfo.flags = 0;
	ColorImageViewInfo.image = image;
	ColorImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	ColorImageViewInfo.format = SurfaceFormat.format;
	ColorImageViewInfo.subresourceRange = SubRange;

	VkImageView ColorView;
	vkCreateImageView(Device.GetLogicDevice(), &ColorImageViewInfo, nullptr, &ColorView);

	
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
	if (Device.GetQueuesFamilies().size() == 1)
	{
		DepthImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		DepthImageInfo.queueFamilyIndexCount = 1;
		uint32_t index = Device.GetQueuesFamilies()[0].Index;
		DepthImageInfo.pQueueFamilyIndices = &index;
	}
	else
	{
		DepthImageInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		DepthImageInfo.queueFamilyIndexCount = Device.GetQueuesFamilies().size();

		for (int i(0); i < Device.GetQueuesFamilies().size(); i++)
		{
			indexes.push_back(Device.GetQueuesFamilies()[i].Index);
		}
		DepthImageInfo.pQueueFamilyIndices = indexes.data();
	}
	DepthImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;// VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkImage DepthImage;
	logger->checkResults(vkCreateImage(Device.GetLogicDevice(), &DepthImageInfo, nullptr, &DepthImage));

	VkMemoryRequirements memReqs;
	vkGetImageMemoryRequirements(Device.GetLogicDevice(), DepthImage, &memReqs);

	VkMemoryAllocateInfo MemoryAllocInfo;
	MemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocInfo.pNext = NULL;
	MemoryAllocInfo.allocationSize = memReqs.size;
	MemoryAllocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;// | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT

	VkDeviceMemory memory;
	vkAllocateMemory(Device.GetLogicDevice(), &MemoryAllocInfo, nullptr, &memory);

	vkBindImageMemory(Device.GetLogicDevice(), DepthImage, memory, 0);

	VkImageSubresourceRange DepthSubRange;
	DepthSubRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	DepthSubRange.baseMipLevel = 0;
	DepthSubRange.levelCount = 1;
	DepthSubRange.baseArrayLayer = 0;
	DepthSubRange.layerCount = 1;

	VkImageViewCreateInfo DepthImageViewInfo{};
	DepthImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	DepthImageViewInfo.pNext = NULL;
	DepthImageViewInfo.flags = 0;
	DepthImageViewInfo.image = DepthImage;
	DepthImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	DepthImageViewInfo.format = DepthFormat;
	DepthImageViewInfo.subresourceRange = DepthSubRange;

	VkImageView DepthView;
	vkCreateImageView(Device.GetLogicDevice(), &DepthImageViewInfo, nullptr, &DepthView);
	
	/*VkCommandBufferBeginInfo BeginInfo;
	BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	BeginInfo.pNext = NULL;
	BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	BeginInfo.pInheritanceInfo = NULL;

	logger->checkResults(vkBeginCommandBuffer(CmdBuffer, &BeginInfo));
	*/
	vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<ShadowColorPipeline*>(Pipelines[1])->GetPipeline());

	static_cast<ShadowColorPipeline*>(Pipelines[1])->Draw(CmdBuffer, { ColorView,  DepthView });

	vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<ShadowPrimitivePipeline*>(Pipelines[0])->GetPipeline());

	static_cast<ShadowPrimitivePipeline*>(Pipelines[0])->Draw(CmdBuffer, { ColorView, DepthView });

	vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<MaterialPipelineWrapper*>(Pipelines[2])->GetPipeline());

	for (int i(2); i < Pipelines.size(); i++)
	{
		static_cast<MaterialPipelineWrapper*>(Pipelines[i])->Draw(CmdBuffer, { ColorView,  DepthView });
	}
}

void ShadowCamera::loadMatModel(string name, string path, glm::vec3 translation)
{
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	std::string err;

	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, name.c_str(), path.c_str());
	
	
	for (int i(0); i< shapes.size(); i++)
	{
		FirstWorldMaterialModel* m;

		vector<TexturedVertex> vert;

		for (auto j : shapes[i].mesh.indices)
		{
			vert.push_back(TexturedVertex(attrib.vertices[3 * j.vertex_index], -attrib.vertices[3 * j.vertex_index + 1], attrib.vertices[3 * j.vertex_index + 2],
				attrib.texcoords[2 * j.texcoord_index], 1.0f - attrib.texcoords[2 * j.texcoord_index + 1],
				attrib.normals[3 * j.normal_index], attrib.normals[3 * j.normal_index + 1], attrib.normals[3 * j.normal_index + 2],
				attrib.texcoords[2 * j.texcoord_index], 1.0f - attrib.texcoords[2 * j.texcoord_index + 1]));
		}

		Pipelines.push_back(new MaterialPipelineWrapper("Material.vert.spv", "Material.frag.spv", logger, Device,
			SurfaceFormat, MainMatrixes.GetBuffer(), TransformationMat.GetBuffer(), DepthTexture, DethView,
			PrimitiveMaterial(logger, Device, shapes[i].name + "_normal.jpg",
				glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.9f, 0.9f, 0.9f), shapes[i].name + "_albedo.jpg")));
		m = new FirstWorldMaterialModel(logger, Device, glm::vec3(), translation, vert);
		static_cast<MaterialPipelineWrapper*>(Pipelines.back())->Models.push_back(m);
	}
}

void ShadowCamera::keyPressed(uint32_t keyCode)
{
	switch (keyCode)
	{
	case 'w':
	case 'W':
	{
		MainMatrixes.GoForward();
		break;
	}
	case 's':
	case 'S':
	{
		MainMatrixes.GoBackward();
		break;
	}
	case 'a':
	case 'A':
	{
		MainMatrixes.StrafeLeft();
		break;
	}
	case 'd':
	case 'D':
	{
		MainMatrixes.StrafeRight();
		break;
	}
	case VK_SPACE:
	{
		MainMatrixes.Up();
		break;
	}
	case VK_SHIFT:
	{
		MainMatrixes.Down();
		break;
	}

	case 'q':
	case 'Q':
	{
		MainMatrixes.RotateLeft();
		break;
	}

	case 'e':
	case 'E':
	{
		MainMatrixes.RotateRight();
		break;
	}
	default:
		break;
	}
}


ShadowCamera::~ShadowCamera()
{
}
