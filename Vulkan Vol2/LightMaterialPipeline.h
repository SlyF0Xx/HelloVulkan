#pragma once
#include "LightPrimitiveBasePipelineWrapper.hpp"
#include "TexturedVertex.h"

class LightMaterialPipeline :
	public LightPrimitiveBasePipelineWrapper<TexturedVertex>
{
private:
	static vector<VkVertexInputBindingDescription> InitVertexInputDesc();
	static vector<VkVertexInputAttributeDescription> InitVertexInputAttrDesc();
	static vector<VkAttachmentDescription> InitAttachments(VkFormat DepthFormat);

public:
	LightMaterialPipeline(Logger * logger, LogicDeviceWrapper device, VkFormat DepthFormat, VkBuffer ViewProjMatrixBuffer);
	~LightMaterialPipeline();
};
