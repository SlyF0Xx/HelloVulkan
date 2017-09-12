#pragma once
#include "ShadowPrimitiveBasePipelineWrapper.hpp"
#include "PrimitiveVertex.h"
#include "AbstractWorldModel.hpp"

class ShadowPrimitivePipeline :
	public ShadowPrimitiveBasePipelineWrapper<PrimitiveVertex>
{
protected:
	static vector<VkVertexInputBindingDescription> InitVertexInputDesc();
	static vector<VkVertexInputAttributeDescription> InitVertexInputAttrDesc();
	static vector<VkAttachmentDescription> InitAttachments(VkSurfaceFormatKHR SurfaceFormat);
public:
	ShadowPrimitivePipeline(Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR SurfaceFormat, VkBuffer ViewProjMatrixBuffer,
		VkBuffer TransformationMatrixBuffer, VkImage DepthImage, VkImageView TextureBuffer);
	~ShadowPrimitivePipeline();
};

