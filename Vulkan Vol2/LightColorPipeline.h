#pragma once
#include "LightPrimitiveBasePipelineWrapper.hpp"
#include "ColorVertex.h"
#include "AbstractWorldModel.hpp"

class LightColorPipeline :
	public LightPrimitiveBasePipelineWrapper<ColorVertex>
{
private:
	static vector<VkVertexInputBindingDescription> InitVertexInputDesc();
	static vector<VkVertexInputAttributeDescription> InitVertexInputAttrDesc();
	static vector<VkAttachmentDescription> InitAttachments(VkFormat DepthFormat);
public:
	LightColorPipeline(Logger * logger, LogicDeviceWrapper device, VkFormat DepthFormat, VkBuffer ViewProjMatrixBuffer);
	~LightColorPipeline();
};

