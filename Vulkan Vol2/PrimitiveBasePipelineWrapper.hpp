#pragma once
#include "AbstractPipelineWrapper.hpp"

template<class Vertex, class Model>
class PrimitiveBasePipelineWrapper :
	public AbstractPipelineWrapper<Vertex, Model, 1>
{
private:
	VkShaderModule loadSPIRVShader(std::string filename);
protected:
	LogicDeviceWrapper Device;
	Logger *logger;

	VkRenderPass RenderPass;
	VkPipelineLayout PipelineLayout;
	vector<VkDescriptorSet> Descriptors;

public:
	VkRenderPass GetRenderPass();
	PrimitiveBasePipelineWrapper(string vertShader, string fragShader,
		vector<VkVertexInputBindingDescription> VertexInputDesc, vector<VkVertexInputAttributeDescription> VertexInputAttrDesc, vector<VkAttachmentDescription> Attachments,
		Logger *logger, LogicDeviceWrapper Device, vector<VkDescriptorSetLayout> DescriptorSets, vector<VkDescriptorSet> descriptors);
	virtual ~PrimitiveBasePipelineWrapper();
};


template<class Vertex, class Model>
inline VkShaderModule PrimitiveBasePipelineWrapper<Vertex, Model>::loadSPIRVShader(std::string filename)
{
	size_t shaderSize;
	char* shaderCode;

#if defined(__ANDROID__)
	// Load shader from compressed asset
	AAsset* asset = AAssetManager_open(androidApp->activity->assetManager, filename.c_str(), AASSET_MODE_STREAMING);
	assert(asset);
	shaderSize = AAsset_getLength(asset);
	assert(shaderSize > 0);

	shaderCode = new char[shaderSize];
	AAsset_read(asset, shaderCode, shaderSize);
	AAsset_close(asset);
#else
	std::ifstream is(filename, std::ios::binary | std::ios::in | std::ios::ate);

	if (is.is_open())
	{
		shaderSize = is.tellg();
		is.seekg(0, std::ios::beg);
		// Copy file contents into a buffer
		shaderCode = new char[shaderSize];
		is.read(shaderCode, shaderSize);
		is.close();
		assert(shaderSize > 0);
	}
#endif
	if (shaderCode)
	{
		// Create a new shader module that will be used for pipeline creation
		VkShaderModuleCreateInfo moduleCreateInfo{};
		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleCreateInfo.codeSize = shaderSize;
		moduleCreateInfo.pCode = (uint32_t*)shaderCode;

		VkShaderModule shaderModule;
		logger->checkResults(vkCreateShaderModule(Device.GetLogicDevice(), &moduleCreateInfo, NULL, &shaderModule));

		delete[] shaderCode;

		return shaderModule;
	}
	else
	{
		logger->LogMessage("Error: Could not open shader file \"" + filename + "\"");
		return VK_NULL_HANDLE;
	}
}

template<class Vertex, class Model>
inline VkRenderPass PrimitiveBasePipelineWrapper<Vertex, Model>::GetRenderPass()
{
	return RenderPass;
}

template<class Vertex, class Model>
PrimitiveBasePipelineWrapper<Vertex, Model>::PrimitiveBasePipelineWrapper(string vertShader, string fragShader,
	vector<VkVertexInputBindingDescription> VertexInputDesc, vector<VkVertexInputAttributeDescription> VertexInputAttrDesc, vector<VkAttachmentDescription> Attachments,
	Logger *logger, LogicDeviceWrapper device, vector<VkDescriptorSetLayout> DescriptorSets, vector<VkDescriptorSet> descriptors) :Device(device), Descriptors(descriptors)
{
	this->logger = logger;

	VkShaderModule VertexShader = loadSPIRVShader(vertShader);
	VkShaderModule PixelShader = loadSPIRVShader(fragShader);

	vector<VkPipelineShaderStageCreateInfo> Stages(2);
	//Vertex Stage
	Stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	Stages[0].pNext = NULL;
	Stages[0].flags = 0;
	Stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	Stages[0].module = VertexShader;
	Stages[0].pName = "main";
	Stages[0].pSpecializationInfo = NULL; //Забавная штука, позволяет инициализировать шейдерные константы

										  //Pixel Stage
	Stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	Stages[1].pNext = NULL;
	Stages[1].flags = 0;
	Stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	Stages[1].module = PixelShader;
	Stages[1].pName = "main";
	Stages[1].pSpecializationInfo = NULL; //Забавная штука, позволяет инициализировать шейдерные константы


	VkPipelineVertexInputStateCreateInfo VertexInputInfo{};
	VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInputInfo.pNext = NULL;
	VertexInputInfo.flags = 0;
	VertexInputInfo.vertexBindingDescriptionCount = VertexInputDesc.size();
	VertexInputInfo.pVertexBindingDescriptions = VertexInputDesc.data();
	VertexInputInfo.vertexAttributeDescriptionCount = VertexInputAttrDesc.size();
	VertexInputInfo.pVertexAttributeDescriptions = VertexInputAttrDesc.data();



	//Здесь мы можем добавить свои Descriptor Set Layout-ы и Descriptor Layout-ы

	VkPipelineLayoutCreateInfo LayoutInfo{};
	LayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	LayoutInfo.pNext = NULL;
	LayoutInfo.flags = 0;
	LayoutInfo.setLayoutCount = DescriptorSets.size();
	LayoutInfo.pSetLayouts = DescriptorSets.data();
	LayoutInfo.pushConstantRangeCount = 0;
	LayoutInfo.pPushConstantRanges = NULL;


	vkCreatePipelineLayout(Device.GetLogicDevice(), &LayoutInfo, nullptr, &PipelineLayout);



	VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo{};
	InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	InputAssemblyInfo.pNext = NULL;
	InputAssemblyInfo.flags = 0;
	InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	InputAssemblyInfo.primitiveRestartEnable = VK_FALSE; //РАзобраться!!



														 /*
														 VkViewport Viewport;
														 Viewport.x = 0;
														 Viewport.y = 0;
														 Viewport.minDepth = 0.1;
														 Viewport.maxDepth = 1000.0;
														 Viewport.height = Extent.height;
														 Viewport.width = Extent.width;
														 */
	VkPipelineViewportStateCreateInfo ViewportStateInfo{};
	ViewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	ViewportStateInfo.pNext = NULL;
	ViewportStateInfo.flags = 0;
	ViewportStateInfo.viewportCount = 1;
	ViewportStateInfo.scissorCount = 1;
	//ViewportStateInfo.pViewports = &Viewport;


	VkPipelineRasterizationStateCreateInfo RasterizationInfo{};
	RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	RasterizationInfo.pNext = NULL;
	RasterizationInfo.flags = 0;
	RasterizationInfo.depthClampEnable = VK_FALSE;
	RasterizationInfo.rasterizerDiscardEnable = VK_FALSE; //НЕ ТРОГАТЬ НИКОГДА В ЖИЗНИ!!! ВЫКИДЫВАЕТ ИЗОБРАЖЕНИЕ ПОСЛЕ РАСТЕРИЗАЦИИ
	RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	RasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; // VK_FRONT_FACE_COUNTER_CLOCKWISE;// VK_FRONT_FACE_COUNTER_CLOCKWISE;
	RasterizationInfo.depthBiasEnable = VK_FALSE; //Не разобрался
	RasterizationInfo.depthBiasConstantFactor = 0.0;
	RasterizationInfo.depthBiasClamp = 0.0; //Не разобрался
	RasterizationInfo.depthBiasSlopeFactor = 0.0; //Не разобрался
	RasterizationInfo.lineWidth = 1.0;


	VkSampleCountFlagBits Sample = VK_SAMPLE_COUNT_1_BIT;
	//Говорят, это про anti-aliasing
	VkPipelineMultisampleStateCreateInfo MultisampleInfo{};
	MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	MultisampleInfo.pNext = NULL;
	MultisampleInfo.flags = 0;
	MultisampleInfo.rasterizationSamples = Sample;
	MultisampleInfo.sampleShadingEnable = VK_FALSE;
	MultisampleInfo.minSampleShading = 0;
	MultisampleInfo.pSampleMask = nullptr;
	MultisampleInfo.alphaToCoverageEnable = VK_TRUE;
	MultisampleInfo.alphaToOneEnable = VK_FALSE;



	VkPipelineDepthStencilStateCreateInfo DepthStensilInfo{};
	DepthStensilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	DepthStensilInfo.pNext = NULL;
	DepthStensilInfo.flags = 0;
	DepthStensilInfo.depthTestEnable = VK_TRUE;
	DepthStensilInfo.depthWriteEnable = VK_TRUE;
	DepthStensilInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	DepthStensilInfo.depthBoundsTestEnable = VK_FALSE; //Если VK_TRUE, то делает отдельный z тест на minDepthBounds и maxDepthBounds
	DepthStensilInfo.stencilTestEnable = VK_FALSE; // Разобраться и добавить - классная, вроде, штука

	DepthStensilInfo.back.failOp = VK_STENCIL_OP_KEEP;
	DepthStensilInfo.back.passOp = VK_STENCIL_OP_KEEP;
	DepthStensilInfo.back.depthFailOp = VK_STENCIL_OP_KEEP;
	DepthStensilInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;

	DepthStensilInfo.front = DepthStensilInfo.back;



	VkPipelineColorBlendAttachmentState AttachState{};
	AttachState.blendEnable = VK_FALSE;
	AttachState.colorWriteMask = 0xf;

	VkPipelineColorBlendStateCreateInfo ColorBlendInfo{};
	ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ColorBlendInfo.pNext = NULL;
	ColorBlendInfo.flags = 0;
	//ColorBlendInfo.logicOpEnable = VK_FALSE; //Насколько я понял, можно сделать операцию по сравнению с текущим значением в кадре
	ColorBlendInfo.attachmentCount = 1;
	ColorBlendInfo.pAttachments = &AttachState;



	vector<VkDynamicState> states;
	states.push_back(VK_DYNAMIC_STATE_VIEWPORT);
	states.push_back(VK_DYNAMIC_STATE_SCISSOR);

	VkPipelineDynamicStateCreateInfo DynamicStateInfo{};
	DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	DynamicStateInfo.pNext = NULL;
	DynamicStateInfo.flags = 0;
	DynamicStateInfo.dynamicStateCount = states.size();
	DynamicStateInfo.pDynamicStates = states.data();


	VkAttachmentReference colorReference{};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference{};
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	vector<VkSubpassDescription> Subpasses;
	Subpasses.push_back(VkSubpassDescription());
	Subpasses[0].flags = 0; // Возможны варианты
	Subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	Subpasses[0].colorAttachmentCount = 1;
	Subpasses[0].pColorAttachments = &colorReference;

	Subpasses[0].pDepthStencilAttachment = &depthReference;

	Subpasses[0].preserveAttachmentCount = 0;									// Preserved attachments can be used to loop (and preserve) attachments through subpasses
	Subpasses[0].pPreserveAttachments = nullptr;								// (Preserve attachments not used by this example)
	Subpasses[0].pResolveAttachments = NULL;								// Resolve attachments are resolved at the end of a sub pass and can be used for e.g. multi sampling
	Subpasses[0].inputAttachmentCount = 0;									// Input attachments can be used to sample from contents of a previous subpass
	Subpasses[0].pInputAttachments = nullptr;									// (Input attachments not used by this example)



																				// Setup subpass dependencies
																				// These will add the implicit ttachment layout transitionss specified by the attachment descriptions
																				// The actual usage layout is preserved through the layout specified in the attachment reference		
																				// Each subpass dependency will introduce a memory and execution dependency between the source and dest subpass described by
																				// srcStageMask, dstStageMask, srcAccessMask, dstAccessMask (and dependencyFlags is set)
																				// Note: VK_SUBPASS_EXTERNAL is a special constant that refers to all commands executed outside of the actual renderpass)
	std::array<VkSubpassDependency, 2> dependencies;

	// First dependency at the start of the renderpass
	// Does the transition from final to initial layout 
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;								// Producer of the dependency 
	dependencies[0].dstSubpass = 0;													// Consumer is our single subpass that will wait for the execution depdendency
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	// Second dependency at the end the renderpass
	// Does the transition from the initial to the final layout
	dependencies[1].srcSubpass = 0;													// Producer of the dependency is our single subpass
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;								// Consumer are all commands outside of the renderpass
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;



	VkRenderPassCreateInfo RenderPassInfo{};
	RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassInfo.pNext = NULL;
	RenderPassInfo.flags = 0;
	RenderPassInfo.attachmentCount = Attachments.size();
	RenderPassInfo.pAttachments = Attachments.data();
	RenderPassInfo.subpassCount = Subpasses.size();
	RenderPassInfo.pSubpasses = Subpasses.data();
	RenderPassInfo.dependencyCount = dependencies.size();
	RenderPassInfo.pDependencies = dependencies.data();

	vkCreateRenderPass(Device.GetLogicDevice(), &RenderPassInfo, nullptr, &RenderPass);


	VkPipelineCacheCreateInfo CacheInfo{};
	CacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	CacheInfo.pNext = NULL;
	CacheInfo.flags = 0;
	CacheInfo.initialDataSize = NULL;
	CacheInfo.pInitialData = NULL;

	VkPipelineCache Cache;
	vkCreatePipelineCache(Device.GetLogicDevice(), &CacheInfo, nullptr, &Cache);

	VkGraphicsPipelineCreateInfo PipelineInfo{};
	//PipelineInfo.push_back(VkGraphicsPipelineCreateInfo());
	PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	PipelineInfo.pNext = NULL;
	PipelineInfo.flags = 0;
	PipelineInfo.stageCount = Stages.size();
	PipelineInfo.pStages = Stages.data();
	PipelineInfo.pVertexInputState = &VertexInputInfo;
	PipelineInfo.pInputAssemblyState = &InputAssemblyInfo;
	PipelineInfo.pViewportState = &ViewportStateInfo;
	PipelineInfo.pRasterizationState = &RasterizationInfo;
	PipelineInfo.pMultisampleState = &MultisampleInfo;
	PipelineInfo.pDepthStencilState = &DepthStensilInfo;
	PipelineInfo.pColorBlendState = &ColorBlendInfo;
	PipelineInfo.pDynamicState = &DynamicStateInfo;
	PipelineInfo.layout = PipelineLayout;
	PipelineInfo.renderPass = RenderPass;
	PipelineInfo.subpass = 0;
	PipelineInfo.basePipelineHandle = NULL;
	PipelineInfo.basePipelineIndex = NULL;

	logger->checkResults(vkCreateGraphicsPipelines(Device.GetLogicDevice(), Cache, 1, &PipelineInfo, nullptr, &Pipeline));
}

template<class Vertex, class Model>
inline PrimitiveBasePipelineWrapper<Vertex, Model>::~PrimitiveBasePipelineWrapper()
{
}
