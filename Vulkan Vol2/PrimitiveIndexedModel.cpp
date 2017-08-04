#include "PrimitiveIndexedModel.h"



//PrimitiveIndexedModel::PrimitiveIndexedModel(LogicDeviceWrapper Device) :
//	AbstractIndexedModel<PrimitiveVertex, PrimitiveVertexBufferWrapper<PrimitiveVertex>, PrimitiveIndexedModel, PrimitiveIndexBuffer>(Device)
//{
//}

PrimitiveIndexedModel::PrimitiveIndexedModel(LogicDeviceWrapper Device, glm::vec3 rotation):
	AbstractIndexedModel<PrimitiveVertex, PrimitiveVertexBufferWrapper<PrimitiveVertex>, PrimitiveIndexBuffer>(Device, PrimitiveIndexedModel::InitVertexes(), PrimitiveIndexedModel::InitIndexes(), rotation)
{
}

PrimitiveIndexedModel::~PrimitiveIndexedModel()
{
}

void PrimitiveIndexedModel::Draw(VkCommandBuffer CmdBuffer)
{
	VkBuffer temp = VertBuffer.GetBuffer();
	VkDeviceSize offsets[1] = { 0 };
	vkCmdBindVertexBuffers(CmdBuffer, 0, 1, &temp, offsets);
	vkCmdBindIndexBuffer(CmdBuffer, IndBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

	vkCmdDrawIndexed(CmdBuffer, IndBuffer.GetIndexes().size(), 1, 0, 0, 1);
}

vector<PrimitiveVertex> PrimitiveIndexedModel::InitVertexes()
{
	return {
		{ 2.0f,  1.0f, 0.0f },
		{ 0.0f,  1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f }
	};
}

vector<uint32_t> PrimitiveIndexedModel::InitIndexes()
{
	return {0,1,2};
}

