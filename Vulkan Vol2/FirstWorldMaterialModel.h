#pragma once

#include "AbstractWorldMaterialModel.hpp"
#include "TexturedVertex.h"
//#include "PrimitiveMaterial.h"
#include "PrimitiveVertexBufferWrapper.h"

class FirstWorldMaterialModel :
	public AbstractWorldMaterialModel<TexturedVertex, PrimitiveVertexBufferWrapper<TexturedVertex>/*, PrimitiveMaterial*/>
{
private:
	static vector<TexturedVertex> InitVertex();
protected:
	Logger* logger;
public:
	FirstWorldMaterialModel(Logger* logger, LogicDeviceWrapper Device, glm::vec3 rotation, glm::vec3 translation, vector<TexturedVertex> vertexes = InitVertex());
	~FirstWorldMaterialModel();
};

