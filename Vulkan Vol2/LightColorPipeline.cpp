#include "LightColorPipeline.h"




vector<VkVertexInputBindingDescription> LightColorPipeline::InitVertexInputDesc()
{
	vector<VkVertexInputBindingDescription> VertexInputDesc(1);
	VertexInputDesc[0].binding = 0;
	VertexInputDesc[0].stride = sizeof(ColorVertex);
	VertexInputDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return VertexInputDesc;
}

vector<VkVertexInputAttributeDescription> LightColorPipeline::InitVertexInputAttrDesc()
{
	//��������� ����� ����� �������� � ������. ��� ������ ����������, ������������ ����� ��������
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

vector<VkAttachmentDescription> LightColorPipeline::InitAttachments(VkFormat DepthFormat)
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

LightColorPipeline::LightColorPipeline(Logger * logger, LogicDeviceWrapper device, VkFormat DepthFormat, VkBuffer ViewProjMatrixBuffer)
	:LightPrimitiveBasePipelineWrapper<ColorVertex>("FirstShadowColor.vert.spv", "FirstShadowColor.frag.spv", InitVertexInputDesc(), InitVertexInputAttrDesc(),
		InitAttachments(DepthFormat), logger, device, ViewProjMatrixBuffer)
{
}

LightColorPipeline::~LightColorPipeline()
{
}
