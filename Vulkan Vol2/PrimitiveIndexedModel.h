#pragma once
#include "AbstractIndexedModel.hpp"
#include "PrimitiveVertex.h"
#include "PrimitiveVertexBufferWrapper.h"
#include "PrimitiveIndexBuffer.h"

class PrimitiveIndexedModel :
	public AbstractIndexedModel<PrimitiveVertex, PrimitiveVertexBufferWrapper<PrimitiveVertex>, PrimitiveIndexBuffer>
{
public:
	PrimitiveIndexedModel(LogicDeviceWrapper Device, glm::vec3 rotation);
	~PrimitiveIndexedModel();
	void Draw(VkCommandBuffer CmdBuffer);
protected:
	static vector<PrimitiveVertex> InitVertexes();
	static vector<uint32_t> InitIndexes();
};

