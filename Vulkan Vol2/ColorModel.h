#pragma once
#include "AbstractModel.hpp"
#include "ColorVertex.h"
#include "PrimitiveVertexBufferWrapper.h"


#define _USE_MATH_DEFINES
#include <math.h>

class ColorModel :
	public AbstractModel<ColorVertex, PrimitiveVertexBufferWrapper<ColorVertex>>
{
public:
	ColorModel(LogicDeviceWrapper Device);
	void Draw(VkCommandBuffer CmdBuffer);
	~ColorModel();
protected:
	static vector<ColorVertex> Init();
};

