#include "ShadowPrimitivePipeline.h"



vector<VkVertexInputBindingDescription> ShadowPrimitivePipeline::InitVertexInputDesc()
{
	vector<VkVertexInputBindingDescription> VertexInputDesc(1);
	VertexInputDesc[0].binding = 0;
	VertexInputDesc[0].stride = sizeof(PrimitiveVertex);
	VertexInputDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return VertexInputDesc;
}

vector<VkAttachmentDescription> ShadowPrimitivePipeline::InitAttachments(VkSurfaceFormatKHR SurfaceFormat)
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
	Attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //Выбрасываем буффер глубины после вычислений - а могли бы сохранить 
	Attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	Attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	Attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	Attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	return Attachments;
}

vector<VkVertexInputAttributeDescription> ShadowPrimitivePipeline::InitVertexInputAttrDesc()
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

ShadowPrimitivePipeline::ShadowPrimitivePipeline(Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR SurfaceFormat, VkBuffer ViewProjMatrixBuffer,
	VkBuffer TransformationMatrixBuffer, VkImage DepthImage, VkImageView TextureBuffer)
	:ShadowPrimitiveBasePipelineWrapper<PrimitiveVertex>("ShadowPrim.vert.spv", "ShadowPrim.frag.spv", InitVertexInputDesc(),
		InitVertexInputAttrDesc(), InitAttachments(SurfaceFormat), logger, device, ViewProjMatrixBuffer, TransformationMatrixBuffer, DepthImage, TextureBuffer)
{
}

ShadowPrimitivePipeline::~ShadowPrimitivePipeline()
{
}

