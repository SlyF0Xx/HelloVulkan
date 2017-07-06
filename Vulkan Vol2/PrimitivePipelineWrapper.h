#pragma once
#include "AbstractPipelineWrapper.hpp"
#include "PrimitiveModel.h"

class PrimitivePipelineWrapper: 
	public AbstractPipelineWrapper<PrimitiveVertex>
{
private:
	VkShaderModule loadSPIRVShader(std::string filename);
	LogicDeviceWrapper Device;
	Logger *logger;

	VkRenderPass RenderPass;
	VkPipelineLayout PipelineLayout;

public:
	PrimitivePipelineWrapper(Logger *logger, LogicDeviceWrapper device, VkSurfaceFormatKHR SurfaceFormat, VkFormat DepthFormat, vector<VkDescriptorSetLayout> Descriptors);
	~PrimitivePipelineWrapper();
};

