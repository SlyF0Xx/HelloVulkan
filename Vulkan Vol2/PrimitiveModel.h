#pragma once
#include "AbstractWorldModel.hpp"
#include "AbstractModel.hpp"
#include "PrimitiveVertex.h"
#include "PrimitiveVertexBufferWrapper.h"

class PrimitiveModel :
	public AbstractWorldModel<PrimitiveVertex>,
	public AbstractModel<PrimitiveVertex, PrimitiveVertexBufferWrapper<PrimitiveVertex>>
{
public:
	PrimitiveModel(LogicDeviceWrapper Device, glm::vec3 rotation, glm::vec3 translation);
	void Draw(VkCommandBuffer CmdBuffer);
	~PrimitiveModel();
protected:
	static vector<PrimitiveVertex> Init();
};