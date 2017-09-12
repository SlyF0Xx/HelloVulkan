#include "FirstWorldMaterialModel.h"


FirstWorldMaterialModel::FirstWorldMaterialModel(Logger* logger, LogicDeviceWrapper Device, glm::vec3 rotation, glm::vec3 translation, vector<TexturedVertex> vertexes)
	:AbstractWorldMaterialModel<TexturedVertex, PrimitiveVertexBufferWrapper<TexturedVertex>/*, PrimitiveMaterial*/>(Device, vertexes,
		rotation, translation/*, PrimitiveMaterial(logger, Device, "NormalMap.jpg",
		glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.9f, 0.9f, 0.9f))*/)
{
	this->logger = logger;
}


vector<TexturedVertex> FirstWorldMaterialModel::InitVertex()
{
	return {

		{  6.0f, -2.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f },
		{ -6.0f,  1.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f },
		{  6.0f,  1.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,		0.0f, 1.0f },
		{  6.0f,  0.0f, 2.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f }
		

		/*{ 4.0f,	-6.0f,	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ -4.0f,	2.0f,	0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ 4.0f,		2.0f,	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 4.0f,		0.0f,	4.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }
		*/

		/*{ 6.0f,		-2.0f,	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, -24.0f/25.0f, 7.0f/25.0f, 0.0f, 1.0f, 0.0f, 0.0f},
		{ -6.0f,	1.5f,	0.0f, 1.0f, 1.0f, -4.0f/static_cast<float>(sqrt(122+12* static_cast<float>(sqrt(61.0)))),
			-3.0f/ static_cast<float>(sqrt(122 + 12 * sqrt(61.0))), (static_cast<float>(sqrt(61))+6)/ static_cast<float>(sqrt(122 + 12 * static_cast<float>(sqrt(61.0)))),
			-0.9758329f, 0.20250249f, -0.08211543f, 1.0f, 0.0f, 0.0f},
		{ 6.0f,		1.5f,	0.0f, 1.0f, 0.0f, -4.0f / static_cast<float>(sqrt(122 + 12 * static_cast<float>(sqrt(61.0)))),
			-3.0f / static_cast<float>(sqrt(122 + 12 * static_cast<float>(sqrt(61.0)))), 
			(static_cast<float>(sqrt(61)) + 6) / static_cast<float>(sqrt(122 + 12 * static_cast<float>(sqrt(61.0)))),
			-0.9758329f, 0.20250249f, -0.08211543f, 1.0f, 0.0f, 0.0f},
		{ 6.0f,		0.0f,	2.0f, 0.0f, 1.0f, -4.0f/ static_cast<float>(sqrt(61.0)), -3.0f/ static_cast<float>(sqrt(61.0)),
			6.0f/ static_cast<float>(sqrt(61.0)), -12.0f/ static_cast<float>(sqrt(150.25)),
			1.5f/ static_cast<float>(sqrt(150.25)), -2.0f/ static_cast<float>(sqrt(150.25)), 1.0f, 0.0f, 0.0f}*/
		//{ -6.0f,	1.5f,	11.0f }
	};
}

FirstWorldMaterialModel::~FirstWorldMaterialModel()
{
}
