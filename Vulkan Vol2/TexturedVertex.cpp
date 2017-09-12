#include "TexturedVertex.h"



TexturedVertex::TexturedVertex(float x, float y, float z, float normcoordx, float normcoordy, float normx, float normy, float normz,
	float texcoordx, float texcoordy)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;

	normcoord[0] = normcoordx;
	normcoord[1] = normcoordy;

	texcoord[0] = texcoordx;
	texcoord[1] = texcoordy;

	norm[0] = normx;
	norm[1] = normy;
	norm[2] = normz;
}

TexturedVertex::~TexturedVertex()
{
}
