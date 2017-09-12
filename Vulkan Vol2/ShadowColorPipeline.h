#pragma once
#include "ShadowPrimitiveBasePipelineWrapper.hpp"
#include "ColorVertex.h"

class ShadowColorPipeline :
	public ShadowPrimitiveBasePipelineWrapper<ColorVertex>
{
protected:
	static vector<VkVertexInputBindingDescription> InitVertexInputDesc();
	static vector<VkVertexInputAttributeDescription> InitVertexInputAttrDesc();
	static vector<VkAttachmentDescription> InitAttachments(VkSurfaceFormatKHR SurfaceFormat);
public:
	ShadowColorPipeline(Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR SurfaceFormat, VkBuffer ViewProjMatrixBuffer,
		VkBuffer TransformationMatrixBuffer, VkImage DepthImage, VkImageView TextureBuffer);
	~ShadowColorPipeline();
};

