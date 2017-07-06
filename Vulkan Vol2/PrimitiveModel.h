#pragma once
#include "AbstractModel.hpp"
#include "PrimitiveVertex.h"

class PrimitiveModel :
	public AbstractModel<PrimitiveVertex, PrimitiveModel>
{
public:
	PrimitiveModel(LogicDeviceWrapper Device);
	void Draw();
	~PrimitiveModel();
	void Init();
};