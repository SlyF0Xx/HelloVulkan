#pragma once
#include "AbstractVertex.h"
class ColorVertex :
	public AbstractVertex
{
public:
	float position[3];
	float color[4];
	ColorVertex(float positionx, float positiony, float positionz, float colorr, float colorg, float colorb, float colora);
	~ColorVertex();
};

