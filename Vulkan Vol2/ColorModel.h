#pragma once
#include "AbstractModel.hpp"
#include "ColorVertex.h"
#include "PrimitiveVertexBufferWrapper.h"
#include "AbstractWorldModel.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

class ColorModel :
	public AbstractModel<ColorVertex, PrimitiveVertexBufferWrapper<ColorVertex>>,
	public AbstractWorldModel<ColorVertex>
{
public:
	ColorModel(LogicDeviceWrapper Device, glm::vec3 rotation);
	void Draw(VkCommandBuffer CmdBuffer);
	~ColorModel();
protected:
	static vector<ColorVertex> Init();
};

