#pragma once
#include "PrimitiveBasePipelineWrapper.hpp"

template<class Vertex, class Model, class Material>
class AbstractMaterialPipelineWrapper :
	public PrimitiveBasePipelineWrapper<Vertex, Model>
{
protected:
	Material material;
public:
	AbstractMaterialPipelineWrapper(string vertShader, string fragShader,
		vector<VkVertexInputBindingDescription> VertexInputDesc, vector<VkVertexInputAttributeDescription> VertexInputAttrDesc, vector<VkAttachmentDescription> Attachments,
		Logger *logger, LogicDeviceWrapper Device, vector<VkDescriptorSetLayout> DescriptorSets, Material material);
	~AbstractMaterialPipelineWrapper();

};

template<class Vertex, class Model, class Material>
inline AbstractMaterialPipelineWrapper<Vertex, Model, Material>::AbstractMaterialPipelineWrapper(string vertShader, string fragShader, 
	vector<VkVertexInputBindingDescription> VertexInputDesc, vector<VkVertexInputAttributeDescription> VertexInputAttrDesc, vector<VkAttachmentDescription> Attachments, 
	Logger * logger, LogicDeviceWrapper Device, vector<VkDescriptorSetLayout> DescriptorSets, Material _material)
	:PrimitiveBasePipelineWrapper<Vertex, Model>(vertShader, fragShader, VertexInputDesc, VertexInputAttrDesc, Attachments, logger, Device, DescriptorSets), material(_material)
{
}

template<class Vertex, class Model, class Material>
inline AbstractMaterialPipelineWrapper<Vertex, Model, Material>::~AbstractMaterialPipelineWrapper()
{
}
