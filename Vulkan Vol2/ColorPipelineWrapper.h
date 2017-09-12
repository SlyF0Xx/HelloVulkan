#pragma once
#include "PrimitiveBasePipelineWrapper.hpp"
#include "AbstractWorldModel.hpp"
#include "ColorVertex.h"

class ColorPipelineWrapper :
	public PrimitiveBasePipelineWrapper<ColorVertex, AbstractWorldModel<ColorVertex>>
{
private:
	static vector<VkVertexInputBindingDescription> InitVertexInputDesc();
	static vector<VkVertexInputAttributeDescription> InitVertexInputAttrDesc();
	static vector<VkAttachmentDescription> InitAttachments(VkSurfaceFormatKHR SurfaceFormat);
	static vector<VkDescriptorSetLayout> InitDescriptorSetsLayout(LogicDeviceWrapper device);
	static vector<VkDescriptorSet> InitDescriptors(LogicDeviceWrapper device);

	//vector<VkDescriptorSetLayout> DescriptorSets;

	WorldBuffer WorldMatrixBuffer;
	VkBuffer ViewPtojMatrixBuffer;
	void UpdateViewPtojMatrixDescriptor();
	void UpdateWorldMatrixDescriptor();

	vector<VkDescriptorSet> Descriptors;

	static const VkFormat DepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
	static const VkSampleCountFlagBits Sample = VK_SAMPLE_COUNT_1_BIT;
public:
	ColorPipelineWrapper(Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR SurfaceFormat, VkBuffer ViewProjMatrixBuffer);
	~ColorPipelineWrapper();
	void _Draw(VkCommandBuffer CmdBuffer, vector<VkImageView> ImageViews);
};

