#include "PrimitiveVertex.h"



PrimitiveVertex::PrimitiveVertex(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

PrimitiveVertex::PrimitiveVertex()
{
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;
}

PrimitiveVertex::~PrimitiveVertex()
{
}
