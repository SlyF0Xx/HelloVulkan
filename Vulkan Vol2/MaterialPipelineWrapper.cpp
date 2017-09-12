#include "MaterialPipelineWrapper.h"



void MaterialPipelineWrapper::UpdateViewPtojMatrixDescriptor()
{
	VkDescriptorBufferInfo BufferDescriptor;
	BufferDescriptor.buffer = ViewPtojMatrixBuffer;
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

void MaterialPipelineWrapper::UpdateTransformationMatrixDescriptor()
{
	VkDescriptorBufferInfo BufferDescriptor;
	BufferDescriptor.buffer = TransformationMatrixBuffer;
	BufferDescriptor.offset = 0;
	BufferDescriptor.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet WriteDescriptorSetInfo;
	WriteDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	WriteDescriptorSetInfo.pNext = NULL;
	WriteDescriptorSetInfo.dstSet = Descriptors[0];
	WriteDescriptorSetInfo.dstBinding = 2;
	WriteDescriptorSetInfo.dstArrayElement = 0;
	WriteDescriptorSetInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
	WriteDescriptorSetInfo.descriptorCount = 1;
	WriteDescriptorSetInfo.pBufferInfo = &BufferDescriptor;
	WriteDescriptorSetInfo.pImageInfo = NULL;
	WriteDescriptorSetInfo.pTexelBufferView = NULL;

	vkUpdateDescriptorSets(Device.GetLogicDevice(), 1, &WriteDescriptorSetInfo, 0, nullptr);
}

void MaterialPipelineWrapper::UpdateDepthTextureDescriptor()
{
	VkSamplerCreateInfo SamplerCreateInfo;
	SamplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	SamplerCreateInfo.pNext = NULL;
	SamplerCreateInfo.flags = 0;
	SamplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	SamplerCreateInfo.minFilter = VK_FILTER_LINEAR;
	SamplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	SamplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerCreateInfo.mipLodBias = 0.0f;
	SamplerCreateInfo.anisotropyEnable = VK_FALSE;
	SamplerCreateInfo.maxAnisotropy = 1.0f;
	SamplerCreateInfo.compareEnable = VK_FALSE;
	SamplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
	SamplerCreateInfo.minLod = 0;
	SamplerCreateInfo.maxLod = 0;
	SamplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	SamplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	VkSampler Sampler;
	vkCreateSampler(Device.GetLogicDevice(), &SamplerCreateInfo, nullptr, &Sampler);

	VkDescriptorImageInfo ImageDescriptor;
	ImageDescriptor.sampler = Sampler;
	ImageDescriptor.imageView = DepthTexture;
	ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkWriteDescriptorSet WriteDescriptorSetInfo;
	WriteDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	WriteDescriptorSetInfo.pNext = NULL;
	WriteDescriptorSetInfo.dstSet = Descriptors[0];
	WriteDescriptorSetInfo.dstBinding = 3;
	WriteDescriptorSetInfo.dstArrayElement = 0;
	WriteDescriptorSetInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	WriteDescriptorSetInfo.descriptorCount = 1;
	WriteDescriptorSetInfo.pBufferInfo = NULL;
	WriteDescriptorSetInfo.pImageInfo = &ImageDescriptor;
	WriteDescriptorSetInfo.pTexelBufferView = NULL;

	vkUpdateDescriptorSets(Device.GetLogicDevice(), 1, &WriteDescriptorSetInfo, 0, nullptr);
}

void MaterialPipelineWrapper::UpdateWorldMatrixDescriptor()
{
	VkDescriptorBufferInfo BufferDescriptor;
	BufferDescriptor.buffer = WorldMatrixBuffer.GetBuffer();
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

void MaterialPipelineWrapper::UpdateTextureDescriptor()
{
	VkSamplerCreateInfo SamplerCreateInfo;
	SamplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	SamplerCreateInfo.pNext = NULL;
	SamplerCreateInfo.flags = 0;
	SamplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	SamplerCreateInfo.minFilter = VK_FILTER_LINEAR;
	SamplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	SamplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerCreateInfo.mipLodBias = 0.0f;
	SamplerCreateInfo.anisotropyEnable = VK_FALSE;
	SamplerCreateInfo.maxAnisotropy = 1.0f;
	SamplerCreateInfo.compareEnable = VK_FALSE;
	SamplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
	SamplerCreateInfo.minLod = 0;
	SamplerCreateInfo.maxLod = 0;
	SamplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	SamplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	VkSampler Sampler;
	vkCreateSampler(Device.GetLogicDevice(), &SamplerCreateInfo, nullptr, &Sampler);

	VkDescriptorImageInfo ImageDescriptor;
	ImageDescriptor.sampler = Sampler;
	ImageDescriptor.imageView = material.GetNormalMap();
	ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkWriteDescriptorSet WriteDescriptorSetInfo;
	WriteDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	WriteDescriptorSetInfo.pNext = NULL;
	WriteDescriptorSetInfo.dstSet = Descriptors[0];
	WriteDescriptorSetInfo.dstBinding = 4;
	WriteDescriptorSetInfo.dstArrayElement = 0;
	WriteDescriptorSetInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	WriteDescriptorSetInfo.descriptorCount = 1;
	WriteDescriptorSetInfo.pBufferInfo = NULL;
	WriteDescriptorSetInfo.pImageInfo = &ImageDescriptor;
	WriteDescriptorSetInfo.pTexelBufferView = NULL;

	vkUpdateDescriptorSets(Device.GetLogicDevice(), 1, &WriteDescriptorSetInfo, 0, nullptr);


	ImageDescriptor.sampler = Sampler;
	ImageDescriptor.imageView = material.GetTexture();
	ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	WriteDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	WriteDescriptorSetInfo.pNext = NULL;
	WriteDescriptorSetInfo.dstSet = Descriptors[0];
	WriteDescriptorSetInfo.dstBinding = 5;
	WriteDescriptorSetInfo.dstArrayElement = 0;
	WriteDescriptorSetInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	WriteDescriptorSetInfo.descriptorCount = 1;
	WriteDescriptorSetInfo.pBufferInfo = NULL;
	WriteDescriptorSetInfo.pImageInfo = &ImageDescriptor;
	WriteDescriptorSetInfo.pTexelBufferView = NULL;

	vkUpdateDescriptorSets(Device.GetLogicDevice(), 1, &WriteDescriptorSetInfo, 0, nullptr);
}

MaterialPipelineWrapper::MaterialPipelineWrapper(string vertShader, string fragShader, Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR SurfaceFormat,
	VkBuffer ViewProjMatrixBuffer, VkBuffer TransformationMatrixBuffer, VkImage depthImage, VkImageView depthView, PrimitiveMaterial mat)
	:AbstractMaterialPipelineWrapper<TexturedVertex, AbstractWorldMaterialModel<TexturedVertex, PrimitiveVertexBufferWrapper<TexturedVertex>>, PrimitiveMaterial>
	(vertShader, fragShader, InitVertexInputDesc(), InitVertexInputAttrDesc(), InitAttachments(SurfaceFormat), logger, device, InitDescriptorSetsLayout(device), 
		PrimitiveMaterial(mat)),
	TransformationMatrixBuffer(TransformationMatrixBuffer), DepthTexture(depthView), DepthImage(depthImage), ViewPtojMatrixBuffer(ViewProjMatrixBuffer), WorldMatrixBuffer(device, glm::vec3())
{
	InitDescriptors();
	UpdateViewPtojMatrixDescriptor();
	UpdateTransformationMatrixDescriptor();
	UpdateDepthTextureDescriptor();
	UpdateWorldMatrixDescriptor();
	UpdateTextureDescriptor();
}

MaterialPipelineWrapper::~MaterialPipelineWrapper()
{
}

void MaterialPipelineWrapper::_Draw(VkCommandBuffer CmdBuffer, vector<VkImageView> ImageViews)
{
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
	FramebufferCreateInfo.attachmentCount = ImageViews.size();
	FramebufferCreateInfo.pAttachments = ImageViews.data();
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


	//ВЫНЕСТИ ОТДЕЛЬНО!!
	//vkCmdBindPipeline(CmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);

	//Binding CameraMatrixs and TransformationMatrix


	vkCmdBindDescriptorSets(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		PipelineLayout, 0, 1, &Descriptors[0], 0, nullptr);
	int i = 1;
	
	VkImageSubresourceLayers lay;
	lay.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	lay.mipLevel = 0;
	lay.baseArrayLayer = 0;
	lay.layerCount = 1;


	
	for (auto j : Models)
	{
		//Binding WorldMatrix
		WorldMatrixBuffer.SetMatrix(j->GetRotation(), j->GetTranslation());

		//_NormTexture = j->GetMaterial().GetNormalMap();


		j->Draw(CmdBuffer);
		i++;
	}

	vkCmdEndRenderPass(CmdBuffer);
}

vector<VkDescriptorSetLayout> MaterialPipelineWrapper::InitDescriptorSetsLayout(LogicDeviceWrapper device)
{
	vector<VkDescriptorSetLayoutBinding> Bindings(6);
	Bindings[0].binding = 0;
	Bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	Bindings[0].descriptorCount = 1;
	Bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	Bindings[0].pImmutableSamplers = NULL;

	Bindings[1].binding = 1;
	Bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	Bindings[1].descriptorCount = 1;
	Bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	Bindings[1].pImmutableSamplers = NULL;

	Bindings[2].binding = 2;
	Bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	Bindings[2].descriptorCount = 1;
	Bindings[2].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	Bindings[2].pImmutableSamplers = NULL;

	Bindings[3].binding = 3;
	Bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	Bindings[3].descriptorCount = 1;
	Bindings[3].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	Bindings[3].pImmutableSamplers = NULL;

	Bindings[4].binding = 4;
	Bindings[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	Bindings[4].descriptorCount = 1;
	Bindings[4].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	Bindings[4].pImmutableSamplers = NULL;

	Bindings[5].binding = 5;
	Bindings[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	Bindings[5].descriptorCount = 1;
	Bindings[5].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	Bindings[5].pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo DescInfo{};
	DescInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	DescInfo.pNext = NULL;
	DescInfo.flags = 0; //Возможен флаг
	DescInfo.bindingCount = Bindings.size();
	DescInfo.pBindings = Bindings.data();

	vector<VkDescriptorSetLayout> DescriptorsLayout = vector<VkDescriptorSetLayout>();
	DescriptorsLayout.push_back(VkDescriptorSetLayout());
	vkCreateDescriptorSetLayout(device.GetLogicDevice(), &DescInfo, nullptr, &DescriptorsLayout.back());

	return DescriptorsLayout;
}

void MaterialPipelineWrapper::InitDescriptors()
{
	//DescriptorsLay = InitDescriptorSetsLayout(device);

	vector<VkDescriptorPoolSize> typeCounts;
	// This example only uses one descriptor type (uniform buffer) and only requests one descriptor of this type
	typeCounts.push_back(VkDescriptorPoolSize());
	typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
	typeCounts[0].descriptorCount = 1;

	typeCounts.push_back(VkDescriptorPoolSize());
	typeCounts[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
	typeCounts[1].descriptorCount = 30;

	typeCounts.push_back(VkDescriptorPoolSize());
	typeCounts[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
	typeCounts[2].descriptorCount = 50;

	VkDescriptorPoolCreateInfo DescPoolInfo;
	DescPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	DescPoolInfo.pNext = NULL;
	DescPoolInfo.flags = 0; //Есть варианты
	DescPoolInfo.maxSets = 100;
	DescPoolInfo.poolSizeCount = typeCounts.size();
	DescPoolInfo.pPoolSizes = typeCounts.data();

	VkDescriptorPool DescPool;
	vkCreateDescriptorPool(Device.GetLogicDevice(), &DescPoolInfo, nullptr, &DescPool);

	VkDescriptorSetAllocateInfo DescriptorInfo;
	DescriptorInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	DescriptorInfo.pNext = NULL;
	DescriptorInfo.descriptorPool = DescPool;
	DescriptorInfo.descriptorSetCount = 1;
	DescriptorInfo.pSetLayouts = &DescriptorsLay[0];

	Descriptors.push_back(VkDescriptorSet());
	vkAllocateDescriptorSets(Device.GetLogicDevice(), &DescriptorInfo, &Descriptors[0]);
}

vector<VkVertexInputBindingDescription> MaterialPipelineWrapper::InitVertexInputDesc()
{
	vector<VkVertexInputBindingDescription> VertexInputDesc(1);
	VertexInputDesc[0].binding = 0;
	VertexInputDesc[0].stride = sizeof(TexturedVertex);
	VertexInputDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return VertexInputDesc;
}

vector<VkVertexInputAttributeDescription> MaterialPipelineWrapper::InitVertexInputAttrDesc()
{
	//Описывает связи между шейдером и хостом. Для каждой переменной, передающейся через конвейер
	vector<VkVertexInputAttributeDescription> VertexInputAttrDesc(4);
	VertexInputAttrDesc[0].location = 0;
	VertexInputAttrDesc[0].binding = 0;
	VertexInputAttrDesc[0].offset = 0;
	VertexInputAttrDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT;

	VertexInputAttrDesc[1].location = 1;
	VertexInputAttrDesc[1].binding = 0;
	VertexInputAttrDesc[1].offset = 12;
	VertexInputAttrDesc[1].format = VK_FORMAT_R32G32_SFLOAT;
	
	VertexInputAttrDesc[2].location = 2;
	VertexInputAttrDesc[2].binding = 0;
	VertexInputAttrDesc[2].offset = 20;
	VertexInputAttrDesc[2].format = VK_FORMAT_R32G32B32_SFLOAT;

	VertexInputAttrDesc[3].location = 3;
	VertexInputAttrDesc[3].binding = 0;
	VertexInputAttrDesc[3].offset = 32;
	VertexInputAttrDesc[3].format = VK_FORMAT_R32G32_SFLOAT;
	return VertexInputAttrDesc;
}

vector<VkAttachmentDescription> MaterialPipelineWrapper::InitAttachments(VkSurfaceFormatKHR SurfaceFormat)
{
	vector<VkAttachmentDescription> Attachments;
	Attachments.push_back(VkAttachmentDescription());
	Attachments[0].flags = 0;
	Attachments[0].format = SurfaceFormat.format;
	Attachments[0].samples = Sample;
	Attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; //НЕ ЗАБЫТЬ!
	Attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	Attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	Attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	Attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	Attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	Attachments.push_back(VkAttachmentDescription());
	Attachments[1].flags = 0;
	Attachments[1].format = DepthFormat;
	Attachments[1].samples = Sample;
	Attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	Attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE; //Выбрасываем буффер глубины после вычислений - а могли бы сохранить 
	Attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	Attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	Attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	Attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	return Attachments;
}
