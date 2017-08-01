#pragma once
#include "AbstractVertex.h"
class PrimitiveVertex :
	public AbstractVertex
{
public:
	float position[3];
	PrimitiveVertex(float x, float y, float z);
	PrimitiveVertex();
	~PrimitiveVertex();
};

