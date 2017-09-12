#include "LightMaterialPipeline.h"



vector<VkVertexInputBindingDescription> LightMaterialPipeline::InitVertexInputDesc()
{
	vector<VkVertexInputBindingDescription> VertexInputDesc(1);
	VertexInputDesc[0].binding = 0;
	VertexInputDesc[0].stride = sizeof(TexturedVertex);
	VertexInputDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return VertexInputDesc;
}

vector<VkVertexInputAttributeDescription> LightMaterialPipeline::InitVertexInputAttrDesc()
{
	//Описывает связи между шейдером и хостом. Для каждой переменной, передающейся через конвейер
	vector<VkVertexInputAttributeDescription> VertexInputAttrDesc;
	VertexInputAttrDesc.push_back(VkVertexInputAttributeDescription());
	VertexInputAttrDesc[0].location = 0;
	VertexInputAttrDesc[0].binding = 0;
	VertexInputAttrDesc[0].offset = 0;
	VertexInputAttrDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	return VertexInputAttrDesc;
}

vector<VkAttachmentDescription> LightMaterialPipeline::InitAttachments(VkFormat DepthFormat)
{
	vector<VkAttachmentDescription> Attachments;
	Attachments.push_back(VkAttachmentDescription());
	Attachments[0].flags = 0;
	Attachments[0].format = DepthFormat;
	Attachments[0].samples = Sample;
	Attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	Attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	Attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	Attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	Attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	Attachments[0].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	return Attachments;
}

LightMaterialPipeline::LightMaterialPipeline(Logger * logger, LogicDeviceWrapper device, VkFormat DepthFormat, VkBuffer ViewProjMatrixBuffer)
	:LightPrimitiveBasePipelineWrapper<TexturedVertex>("FirstShadowPrim.vert.spv", "FirstShadowPrim.frag.spv", InitVertexInputDesc(), InitVertexInputAttrDesc(),
		InitAttachments(DepthFormat), logger, device, ViewProjMatrixBuffer)
{
}


LightMaterialPipeline::~LightMaterialPipeline()
{
}