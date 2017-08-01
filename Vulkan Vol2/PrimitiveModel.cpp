#include "PrimitiveModel.h"

vector<PrimitiveVertex> PrimitiveModel::Init()
{
	return {
		{ 1.0f,  1.0f, 0.0f },
		{ -1.0f,  1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f }
	};
}

PrimitiveModel::PrimitiveModel(LogicDeviceWrapper Device) : 
	AbstractModel<PrimitiveVertex, PrimitiveVertexBufferWrapper<PrimitiveVertex>>(Device, PrimitiveModel::Init())
{
}

void PrimitiveModel::Draw(VkCommandBuffer CmdBuffer)
{
	VkBuffer temp = VertBuffer.GetBuffer();
	VkDeviceSize offsets[1] = { 0 };
	vkCmdBindVertexBuffers(CmdBuffer, 0, 1, &temp, offsets);

	vkCmdDraw(CmdBuffer, VertBuffer.GetVertexes().size(), 1, 0, 0);
}

PrimitiveModel::~PrimitiveModel()
{
}
