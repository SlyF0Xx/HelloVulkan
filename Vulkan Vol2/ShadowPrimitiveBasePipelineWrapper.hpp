#pragma once
#include "PrimitiveBasePipelineWrapper.hpp"
#include "AbstractWorldModel.hpp"

template <class Vertex>
class ShadowPrimitiveBasePipelineWrapper :
	public PrimitiveBasePipelineWrapper<Vertex, AbstractWorldModel<Vertex>>
{
protected:
	vector<VkDescriptorSetLayout> InitDescriptorSetsLayout(LogicDeviceWrapper device);
	void InitDescriptors();

	//vector<VkDescriptorSetLayout> DescriptorSets;

	static const VkSampleCountFlagBits Sample = VK_SAMPLE_COUNT_1_BIT;
	static const VkFormat DepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

	vector<VkDescriptorSet> Descriptors;
	VkImage DepthImage;
	VkImageView DepthTexture;
	WorldBuffer WorldMatrixBuffer;
	VkBuffer ViewPtojMatrixBuffer;
	VkBuffer TransformationMatrixBuffer;
	void UpdateViewPtojMatrixDescriptor();
	void UpdateTransformationMatrixDescriptor();
	void UpdateDepthTextureDescriptor();
	void UpdateWorldMatrixDescriptor();
public:
	ShadowPrimitiveBasePipelineWrapper(string vertShader, string fragShader,
		vector<VkVertexInputBindingDescription> VertexInputDesc, vector<VkVertexInputAttributeDescription> VertexInputAttrDesc,
		vector<VkAttachmentDescription> Attachments, Logger * logger, LogicDeviceWrapper device,
		VkBuffer ViewProjMatrixBuffer, VkBuffer TransformationMatrixBuffer, VkImage DepthImage, VkImageView DepthTexture);
	~ShadowPrimitiveBasePipelineWrapper();
	void _Draw(VkCommandBuffer CmdBuffer, vector<VkImageView> ImageViews);
};

template<class Vertex>
inline vector<VkDescriptorSetLayout> ShadowPrimitiveBasePipelineWrapper<Vertex>::InitDescriptorSetsLayout(LogicDeviceWrapper device)
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

	Bindings.push_back(VkDescriptorSetLayoutBinding());
	Bindings[2].binding = 2;
	Bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	Bindings[2].descriptorCount = 1;
	Bindings[2].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	Bindings[2].pImmutableSamplers = NULL;

	Bindings.push_back(VkDescriptorSetLayoutBinding());
	Bindings[3].binding = 3;
	Bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	Bindings[3].descriptorCount = 1;
	Bindings[3].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	Bindings[3].pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo DescInfo{};
	DescInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	DescInfo.pNext = NULL;
	DescInfo.flags = 0; //Âîçìîæåí ôëàã
	DescInfo.bindingCount = Bindings.size();
	DescInfo.pBindings = Bindings.data();

	vector<VkDescriptorSetLayout> DescriptorsLayout = vector<VkDescriptorSetLayout>();
	DescriptorsLayout.push_back(VkDescriptorSetLayout());
	vkCreateDescriptorSetLayout(device.GetLogicDevice(), &DescInfo, nullptr, &DescriptorsLayout.back());

	return DescriptorsLayout;
}

template<class Vertex>
inline void ShadowPrimitiveBasePipelineWrapper<Vertex>::InitDescriptors()
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
	DescPoolInfo.flags = 0; //Åñòü âàðèàíòû
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

template<class Vertex>
inline ShadowPrimitiveBasePipelineWrapper<Vertex>::ShadowPrimitiveBasePipelineWrapper(string vertShader, string fragShader, 
	vector<VkVertexInputBindingDescription> VertexInputDesc, vector<VkVertexInputAttributeDescription> VertexInputAttrDesc,
	vector<VkAttachmentDescription> Attachments, Logger * logger, LogicDeviceWrapper device,
	VkBuffer ViewProjMatrixBuffer, VkBuffer TransformationMatrixBuffer, VkImage depthImage, VkImageView DepthTexture)
	:PrimitiveBasePipelineWrapper<Vertex, AbstractWorldModel<Vertex>>(vertShader, fragShader, VertexInputDesc,
		VertexInputAttrDesc, Attachments, logger, device, InitDescriptorSetsLayout(device)), 
	TransformationMatrixBuffer(TransformationMatrixBuffer), DepthTexture(DepthTexture), DepthImage(depthImage), ViewPtojMatrixBuffer(ViewProjMatrixBuffer), WorldMatrixBuffer(device, glm::vec3())
{
	InitDescriptors();
	UpdateViewPtojMatrixDescriptor();
	UpdateTransformationMatrixDescriptor();
	UpdateDepthTextureDescriptor();
	UpdateWorldMatrixDescriptor();
}

template<class Vertex>
inline ShadowPrimitiveBasePipelineWrapper<Vertex>::~ShadowPrimitiveBasePipelineWrapper()
{
}

template<class Vertex>
inline void ShadowPrimitiveBasePipelineWrapper<Vertex>::_Draw(VkCommandBuffer CmdBuffer, vector<VkImageView> ImageViews)
{
	vector<VkClearValue> clearValues;
	clearValues.push_back(VkClearValue());
	clearValues[0].color = { { 1.0f, 1.0f, 1.0f, 1.0f } };
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


	//ÂÛÍÅÑÒÈ ÎÒÄÅËÜÍÎ!!
	//vkCmdBindPipeline(CmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);

	//Binding CameraMatrixs and TransformationMatrix
	

	vkCmdBindDescriptorSets(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		PipelineLayout, 0, 1, &Descriptors[0], 0, nullptr);
	int i = 1;
	for (auto j : Models)
	{
		//Binding WorldMatrix
		WorldMatrixBuffer.SetMatrix(j->GetRotation(), j->GetTranslation());

		//UpdateWorldMatrixDescriptor(j->GetWorld().GetBuffer());
		j->Draw(CmdBuffer);
		i++;
	}

	vkCmdEndRenderPass(CmdBuffer);
}

template<class Vertex>
inline void ShadowPrimitiveBasePipelineWrapper<Vertex>::UpdateViewPtojMatrixDescriptor()
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

template<class Vertex>
inline void ShadowPrimitiveBasePipelineWrapper<Vertex>::UpdateTransformationMatrixDescriptor()
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

template<class Vertex>
inline void ShadowPrimitiveBasePipelineWrapper<Vertex>::UpdateWorldMatrixDescriptor()
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

template<class Vertex>
inline void ShadowPrimitiveBasePipelineWrapper<Vertex>::UpdateDepthTextureDescriptor()
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