#include "ColorModel.h"


ColorModel::ColorModel(LogicDeviceWrapper Device, glm::vec3 rotation, glm::vec3 translation)
	:AbstractModel<ColorVertex, PrimitiveVertexBufferWrapper<ColorVertex>>(Device, ColorModel::Init()),
	AbstractWorldModel<ColorVertex>(rotation, translation)
{
}

void ColorModel::Draw(VkCommandBuffer CmdBuffer)
{
	VkBuffer temp = VertBuffer.GetBuffer();
	VkDeviceSize offsets[1] = { 0 };
	vkCmdBindVertexBuffers(CmdBuffer, 0, 1, &temp, offsets);

	vkCmdDraw(CmdBuffer, VertBuffer.GetVertexes().size(), 1, 0, 0);
}

ColorModel::~ColorModel()
{
}

vector<ColorVertex> ColorModel::Init()
{
	vector<ColorVertex> temp;
	const int R = 1;
	const double r = 0.5;
	const float a = M_PI_2/ 100000.0;

	for (int i(0); i < 100000.0; i++)
	{

		temp.push_back(ColorVertex(r*sin(a*i * 4), r*cos(a*i * 4)+ 0.5f , 0.0f, 1.0, 0, 1.0, 1.0f));
		temp.push_back(ColorVertex(R*sin(a*i*4), R*cos(a*i*4) + 0.5f, 0.0f, 1.0, 0, 1.0, 1.0f));
		temp.push_back(ColorVertex(r*sin(a*(i + 1) * 4), r*cos(a*(i + 1) * 4) + 0.5f, 0.0f, 1.0, 0, 1.0, 1.0f));


		temp.push_back(ColorVertex(r*sin(a*(i + 1) * 4), r*cos(a*(i + 1) * 4) + 0.5f, 0.0f, 1.0, 0, 1.0, 1.0f));
		temp.push_back(ColorVertex(R*sin(a*i * 4), R*cos(a*i * 4) + 0.5f, 0.0f, 1.0, 0, 1.0, 1.0f));
		temp.push_back(ColorVertex(R*sin(a*(i + 1) * 4), R*cos(a*(i + 1) * 4) + 0.5f, 0.0f, 1.0, 0, 1.0, 1.0f));
		//temp.push_back(ColorVertex(R*sin(a*i*4)+1.0, R*cos(a*i*4), -1.0f, 1.0, i/ 200000.0, 1.0, 1.0f));
		//temp.push_back(ColorVertex(r*sin(a*i*4)+1.0, r*cos(a*i*4), -1.0f, 1.0, i / 200000.0, 1.0, 1.0f));
	}

	return temp;
}
