#pragma once
#include "PrimitiveVertex.h"
#include "AbstractMaterialPipelineWrapper.hpp"
#include "PrimitiveBasePipelineWrapper.hpp"
#include "PrimitiveVertexBufferWrapper.h"
#include "PrimitiveModel.h"
#include "PrimitiveVertexBufferWrapper.h"
#include "PrimitiveMaterial.h"
#include "FirstWorldMaterialModel.h"

class MaterialPipelineWrapper
	:public AbstractMaterialPipelineWrapper<TexturedVertex, AbstractWorldMaterialModel<TexturedVertex, PrimitiveVertexBufferWrapper<TexturedVertex>>, PrimitiveMaterial>
{
protected:
	vector<VkDescriptorSetLayout> InitDescriptorSetsLayout(LogicDeviceWrapper device);
	void InitDescriptors();
	static vector<VkVertexInputBindingDescription> InitVertexInputDesc();
	static vector<VkVertexInputAttributeDescription> InitVertexInputAttrDesc();
	static vector<VkAttachmentDescription> InitAttachments(VkSurfaceFormatKHR SurfaceFormat);
	//vector<VkDescriptorSetLayout> DescriptorSets;

	static const VkSampleCountFlagBits Sample = VK_SAMPLE_COUNT_1_BIT;
	static const VkFormat DepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

	vector<VkDescriptorSet> Descriptors;

	WorldBuffer WorldMatrixBuffer;
	VkBuffer ViewPtojMatrixBuffer;
	VkBuffer TransformationMatrixBuffer;
	void UpdateViewPtojMatrixDescriptor();
	void UpdateTransformationMatrixDescriptor();
	void UpdateDepthTextureDescriptor();
	void UpdateWorldMatrixDescriptor();
	void UpdateTextureDescriptor();
	VkImageView DepthTexture;
	VkImage DepthImage;
public:
	MaterialPipelineWrapper(string vertShader, string fragShader, Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR SurfaceFormat,
		VkBuffer ViewProjMatrixBuffer, VkBuffer TransformationMatrixBuffer, VkImage depthImage, VkImageView DepthTexture, PrimitiveMaterial mat);
	~MaterialPipelineWrapper();

	void _Draw(VkCommandBuffer CmdBuffer, vector<VkImageView> ImageViews);

};

