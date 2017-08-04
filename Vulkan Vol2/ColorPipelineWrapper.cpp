#include "ColorPipelineWrapper.h"


vector<VkVertexInputBindingDescription> ColorPipelineWrapper::InitVertexInputDesc()
{
	vector<VkVertexInputBindingDescription> VertexInputDesc(1);
	VertexInputDesc[0].binding = 0;
	VertexInputDesc[0].stride = sizeof(ColorVertex);
	VertexInputDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return VertexInputDesc;
}

vector<VkVertexInputAttributeDescription> ColorPipelineWrapper::InitVertexInputAttrDesc()
{
	//Описывает связи между шейдером и хостом. Для каждой переменной, передающейся через конвейер
	vector<VkVertexInputAttributeDescription> VertexInputAttrDesc;
	VertexInputAttrDesc.push_back(VkVertexInputAttributeDescription());
	VertexInputAttrDesc[0].location = 0;
	VertexInputAttrDesc[0].binding = 0;
	VertexInputAttrDesc[0].offset = 0;
	VertexInputAttrDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT;

	VertexInputAttrDesc.push_back(VkVertexInputAttributeDescription());
	VertexInputAttrDesc[1].location = 1;
	VertexInputAttrDesc[1].binding = 0;
	VertexInputAttrDesc[1].offset = 12;
	VertexInputAttrDesc[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	return VertexInputAttrDesc;
}

vector<VkAttachmentDescription> ColorPipelineWrapper::InitAttachments(VkSurfaceFormatKHR SurfaceFormat)
{
	vector<VkAttachmentDescription> Attachments;
	Attachments.push_back(VkAttachmentDescription());
	Attachments[0].flags = 0;
	Attachments[0].format = SurfaceFormat.format;
	Attachments[0].samples = Sample;
	Attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	Attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	Attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	Attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	Attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	Attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	Attachments.push_back(VkAttachmentDescription());
	Attachments[1].flags = 0;
	Attachments[1].format = DepthFormat;
	Attachments[1].samples = Sample;
	Attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	Attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //Выбрасываем буффер глубины после вычислений - а могли бы сохранить 
	Attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	Attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	Attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	Attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	return Attachments;
}

vector<VkDescriptorSetLayout> ColorPipelineWrapper::InitDescriptorSetsLayout(LogicDeviceWrapper device)
{
	vector<VkDescriptorSetLayoutBinding> Bindings;
	Bindings.push_back(VkDescriptorSetLayoutBinding());
	Bindings[0].binding = 0;
	Bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	Bindings[0].descriptorCount = 1;
	Bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	Bindings[0].pImmutableSamplers = NULL;

	Bindings.push_back(VkDescriptorSetLayoutBinding());
	Bindings[1].binding = 1;
	Bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	Bindings[1].descriptorCount = 1;
	Bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	Bindings[1].pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo DescInfo{};
	DescInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	DescInfo.pNext = NULL;
	DescInfo.flags = 0; //Возможен флаг
	DescInfo.bindingCount = Bindings.size();
	DescInfo.pBindings = Bindings.data();

	vector<VkDescriptorSetLayout> Descriptors;
	Descriptors.push_back(VkDescriptorSetLayout());
	vkCreateDescriptorSetLayout(device.GetLogicDevice(), &DescInfo, nullptr, &Descriptors.back());
	return Descriptors;
}

vector<VkDescriptorSet> ColorPipelineWrapper::InitDescriptors(LogicDeviceWrapper device)
{
	vector<VkDescriptorSetLayout> Descriptors = InitDescriptorSetsLayout(device);

	vector<VkDescriptorSet> Descriptor(1);
	vector<VkDescriptorPoolSize> typeCounts;
	// This example only uses one descriptor type (uniform buffer) and only requests one descriptor of this type
	typeCounts.push_back(VkDescriptorPoolSize());
	typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
	typeCounts[0].descriptorCount = 1;

	typeCounts.push_back(VkDescriptorPoolSize());
	typeCounts[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
	typeCounts[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo DescPoolInfo;
	DescPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	DescPoolInfo.pNext = NULL;
	DescPoolInfo.flags = 0; //Есть варианты
	DescPoolInfo.maxSets = 1;
	DescPoolInfo.poolSizeCount = typeCounts.size();
	DescPoolInfo.pPoolSizes = typeCounts.data();

	VkDescriptorPool DescPool;
	vkCreateDescriptorPool(device.GetLogicDevice(), &DescPoolInfo, nullptr, &DescPool);

	VkDescriptorSetAllocateInfo DescriptorInfo;
	DescriptorInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	DescriptorInfo.pNext = NULL;
	DescriptorInfo.descriptorPool = DescPool;
	DescriptorInfo.descriptorSetCount = Descriptors.size();
	DescriptorInfo.pSetLayouts = Descriptors.data();

	vkAllocateDescriptorSets(device.GetLogicDevice(), &DescriptorInfo, &Descriptor[0]);
	return Descriptor;
}

void ColorPipelineWrapper::UpdateViewPtojMatrixDescriptor(VkBuffer MatrixBuffer)
{
	VkDescriptorBufferInfo BufferDescriptor;
	BufferDescriptor.buffer = MatrixBuffer;
	BufferDescriptor.offset = 0;
	BufferDescriptor.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet WriteDescriptorSetInfo;
	WriteDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	WriteDescriptorSetInfo.pNext = NULL;
	WriteDescriptorSetInfo.dstSet = Descriptors[0];
	WriteDescriptorSetInfo.dstBinding = 0;
	WriteDescriptorSetInfo.dstArrayElement = 0;
	WriteDescriptorSetInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
	WriteDescriptorSetInfo.descriptorCount = 1;
	WriteDescriptorSetInfo.pBufferInfo = &BufferDescriptor;
	WriteDescriptorSetInfo.pImageInfo = NULL;
	WriteDescriptorSetInfo.pTexelBufferView = NULL;

	vkUpdateDescriptorSets(Device.GetLogicDevice(), 1, &WriteDescriptorSetInfo, 0, nullptr);
}

void ColorPipelineWrapper::UpdateWorldMatrixDescriptor(VkBuffer MatrixBuffer)
{
	VkDescriptorBufferInfo BufferDescriptor;
	BufferDescriptor.buffer = MatrixBuffer;
	BufferDescriptor.offset = 0;
	BufferDescriptor.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet WriteDescriptorSetInfo;
	WriteDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	WriteDescriptorSetInfo.pNext = NULL;
	WriteDescriptorSetInfo.dstSet = Descriptors[0];
	WriteDescriptorSetInfo.dstBinding = 1;
	WriteDescriptorSetInfo.dstArrayElement = 0;
	WriteDescriptorSetInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
	WriteDescriptorSetInfo.descriptorCount = 1;
	WriteDescriptorSetInfo.pBufferInfo = &BufferDescriptor;
	WriteDescriptorSetInfo.pImageInfo = NULL;
	WriteDescriptorSetInfo.pTexelBufferView = NULL;

	vkUpdateDescriptorSets(Device.GetLogicDevice(), 1, &WriteDescriptorSetInfo, 0, nullptr);
}

ColorPipelineWrapper::ColorPipelineWrapper(Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR SurfaceFormat, VkBuffer ViewProjMatrixBuffer) :
	PrimitiveBasePipelineWrapper<ColorVertex, AbstractWorldModel<ColorVertex>>("Text.vert.spv", "Text.frag.spv", InitVertexInputDesc(), InitVertexInputAttrDesc(), InitAttachments(SurfaceFormat),
		logger, device, InitDescriptorSetsLayout(device), InitDescriptors(device))
{
	UpdateViewPtojMatrixDescriptor(ViewProjMatrixBuffer);
}

ColorPipelineWrapper::~ColorPipelineWrapper()
{
}

void ColorPipelineWrapper::_Draw(VkCommandBuffer CmdBuffer, vector<VkImageView>ImageViews)
{
	vector<VkImageView> IV(ImageViews.begin(), ImageViews.end());

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
	IV.push_back(DepthView);


	vector<VkClearValue> clearValues;
	clearValues.push_back(VkClearValue());
	clearValues[0].color = { { 0.1f, 0.0f, 0.2f, 0.1f } };
	clearValues.push_back(VkClearValue());
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkFramebufferCreateInfo FramebufferCreateInfo;
	FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	FramebufferCreateInfo.pNext = NULL;
	FramebufferCreateInfo.flags = 0;
	FramebufferCreateInfo.renderPass = RenderPass;
	FramebufferCreateInfo.attachmentCount = IV.size();
	FramebufferCreateInfo.pAttachments = IV.data();
	FramebufferCreateInfo.height = 561;
	FramebufferCreateInfo.width = 884;
	FramebufferCreateInfo.layers = 1;

	VkFramebuffer FrameBuffer;
	vkCreateFramebuffer(Device.GetLogicDevice(), &FramebufferCreateInfo, nullptr, &FrameBuffer);

	VkRenderPassBeginInfo RenderBeginInfo;
	RenderBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	RenderBeginInfo.pNext = NULL;
	RenderBeginInfo.renderPass = RenderPass;
	RenderBeginInfo.renderArea.offset.x = 0;
	RenderBeginInfo.renderArea.offset.y = 0;
	RenderBeginInfo.renderArea.extent.height = 561;
	RenderBeginInfo.renderArea.extent.width = 884;
	RenderBeginInfo.clearValueCount = clearValues.size();
	RenderBeginInfo.pClearValues = clearValues.data();
	RenderBeginInfo.framebuffer = FrameBuffer;

	vkCmdBeginRenderPass(CmdBuffer, &RenderBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Update dynamic viewport state
	VkViewport viewport = {};
	viewport.height = (float)561;
	viewport.width = (float)884;
	viewport.minDepth = (float) 0.0f;
	viewport.maxDepth = (float) 1.0f;
	vkCmdSetViewport(CmdBuffer, 0, 1, &viewport);

	// Update dynamic scissor state
	VkRect2D scissor = {};
	scissor.extent.width = 884;
	scissor.extent.height = 561;
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	vkCmdSetScissor(CmdBuffer, 0, 1, &scissor);

	vkCmdBindDescriptorSets(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		PipelineLayout, 0, Descriptors.size(), Descriptors.data(), 0, nullptr);

	//ВЫНЕСТИ ОТДЕЛЬНО!!
	//vkCmdBindPipeline(CmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);

	for (auto j : Models)
	{
		UpdateWorldMatrixDescriptor(j->GetWorld().GetBuffer());
		j->Draw(CmdBuffer);
	}

	vkCmdEndRenderPass(CmdBuffer);

}
