#pragma once
#include "LightPrimitiveBasePipelineWrapper.hpp"
#include "PrimitiveVertex.h"
#include "AbstractWorldModel.hpp"

class LightPrimitivePipeline :
	public LightPrimitiveBasePipelineWrapper<PrimitiveVertex>
{
private:
	static vector<VkVertexInputBindingDescription> InitVertexInputDesc();
	static vector<VkVertexInputAttributeDescription> InitVertexInputAttrDesc();
	static vector<VkAttachmentDescription> InitAttachments(VkFormat DepthFormat);
	
public:
	LightPrimitivePipeline(Logger * logger, LogicDeviceWrapper device, VkFormat DepthFormat, VkBuffer ViewProjMatrixBuffer);
	~LightPrimitivePipeline();
};

