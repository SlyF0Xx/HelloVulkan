#include "PrimitiveModel.h"

void PrimitiveModel::Init()
{
	VertBuffer.SetVetrexes({
		{ 1.0f,  1.0f, 0.0f },
		{ -1.0f,  1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f }
	});
}

PrimitiveModel::PrimitiveModel(LogicDeviceWrapper Device) : AbstractModel<PrimitiveVertex, PrimitiveModel>(Device)
{
}

void PrimitiveModel::Draw()
{
}

PrimitiveModel::~PrimitiveModel()
{
}
