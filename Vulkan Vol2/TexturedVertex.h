#pragma once
#include "AbstractVertex.h"
class TexturedVertex :
	public AbstractVertex
{
public:
	float position[3];
	float normcoord[2];
	float norm[3];
	//float tang[3];
	//float bitang[3];

	float texcoord[2];
	TexturedVertex(float x, float y, float z, float normcoordx, float normcoordy, float normx, float normy, float normz,
		float texcoordx, float texcoordy);
	~TexturedVertex();
};

