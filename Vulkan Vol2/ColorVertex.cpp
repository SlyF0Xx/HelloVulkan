#include "ColorVertex.h"



ColorVertex::ColorVertex(float positionx, float positiony, float positionz, float colorr, float colorg, float colorb, float colora)
{
	position[0] = positionx;
	position[1] = positiony;
	position[2] = positionz;

	color[0] = colorr;
	color[1] = colorg;
	color[2] = colorb;
	color[3] = colora;
}

ColorVertex::~ColorVertex()
{
}
