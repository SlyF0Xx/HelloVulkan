#pragma once
#include "PrimitiveBasePipelineWrapper.hpp"
#include "PrimitiveModel.h"
#include "AbstractWorldModel.hpp"

class PrimitivePipelineWrapper: 
	public PrimitiveBasePipelineWrapper<PrimitiveVertex, AbstractWorldModel<PrimitiveVertex>>
{
private:
	static vector<VkVertexInputBindingDescription> InitVertexInputDesc();
	static vector<VkVertexInputAttributeDescription> InitVertexInputAttrDesc();
	static vector<VkAttachmentDescription> InitAttachments(VkSurfaceFormatKHR SurfaceFormat);
	static vector<VkDescriptorSetLayout> InitDescriptorSetsLayout(LogicDeviceWrapper device);
	static vector<VkDescriptorSet> InitDescriptors(LogicDeviceWrapper device);

	//vector<VkDescriptorSetLayout> DescriptorSets;

	static const VkFormat DepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
	static const VkSampleCountFlagBits Sample = VK_SAMPLE_COUNT_1_BIT;
public:
	PrimitivePipelineWrapper(Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR SurfaceFormat, VkBuffer ViewProjMatrixBuffer);
	~PrimitivePipelineWrapper();
	void _Draw(VkCommandBuffer CmdBuffer, vector<VkImageView> ImageViews);
	void UpdateViewPtojMatrixDescriptor(VkBuffer MatrixBuffer);
	void UpdateWorldMatrixDescriptor(VkBuffer MatrixBuffer);
};

