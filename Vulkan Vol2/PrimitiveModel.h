#pragma once
#include "AbstractModel.hpp"
#include "PrimitiveVertex.h"
#include "PrimitiveVertexBufferWrapper.h"

class PrimitiveModel :
	public AbstractModel<PrimitiveVertex, PrimitiveVertexBufferWrapper<PrimitiveVertex>>
{
public:
	PrimitiveModel(LogicDeviceWrapper Device);
	void Draw(VkCommandBuffer CmdBuffer);
	~PrimitiveModel();
protected:
	static vector<PrimitiveVertex> Init();
};