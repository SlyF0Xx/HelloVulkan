#include "LightCamera.h"



LightCamera::LightCamera(Logger * logger, LogicDeviceWrapper device, int ScreenX, int ScreenY)
	:Device(device), MainMatrixes(device, ScreenX, ScreenX, std::numeric_limits<float>::infinity()), AbstractControlCamera()
{
	this->logger = logger;
	Pipelines.push_back(new LightPrimitivePipeline(logger, device, DepthFormat, MainMatrixes.GetBuffer()));
	Pipelines.push_back(new LightColorPipeline(logger, device, DepthFormat, MainMatrixes.GetBuffer()));
	Pipelines.push_back(new LightMaterialPipeline(logger, device, DepthFormat, MainMatrixes.GetBuffer()));
}

void LightCamera::keyPressed(uint32_t keyCode)
{
		switch (keyCode)
		{
		case 'w':
		case 'W':
		{
			MainMatrixes.GoForward();
			break;
		}
		case 's':
		case 'S':
		{
			MainMatrixes.GoBackward();
			break;
		}
		case 'a':
		case 'A':
		{
			MainMatrixes.StrafeLeft();
			break;
		}
		case 'd':
		case 'D':
		{
			MainMatrixes.StrafeRight();
			break;
		}
		case VK_SPACE:
		{
			MainMatrixes.Up();
			break;
		}
		case VK_SHIFT:
		{
			MainMatrixes.Down();
			break;
		}

		case 'q':
		case 'Q':
		{
			MainMatrixes.RotateLeft();
			break;
		}

		case 'e':
		case 'E':
		{
			MainMatrixes.RotateRight();
			break;
		}
		default:
			break;
		}
}


void LightCamera::Draw(VkCommandBuffer CmdBuffer, VkImage image)
{
	VkImageSubresourceRange SubRange;
	SubRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	SubRange.baseMipLevel = 0;
	SubRange.levelCount = 1;
	SubRange.baseArrayLayer = 0;
	SubRange.layerCount = 1;

	VkImageViewCreateInfo DepthImageViewInfo{};
	DepthImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	DepthImageViewInfo.pNext = NULL;
	DepthImageViewInfo.flags = 0;
	DepthImageViewInfo.image = image;
	DepthImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	DepthImageViewInfo.format = DepthFormat;
	DepthImageViewInfo.subresourceRange = SubRange;

	VkImageView DepthView;
	vkCreateImageView(Device.GetLogicDevice(), &DepthImageViewInfo, nullptr, &DepthView);

	vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<LightPrimitivePipeline*>(Pipelines[0])->GetPipeline());

	static_cast<LightPrimitivePipeline*>(Pipelines[0])->Draw(CmdBuffer, { DepthView });

	vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<LightMaterialPipeline*>(Pipelines[2])->GetPipeline());

	static_cast<LightMaterialPipeline*>(Pipelines[2])->Draw(CmdBuffer, { DepthView });

	vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<LightColorPipeline*>(Pipelines[1])->GetPipeline());

	static_cast<LightColorPipeline*>(Pipelines[1])->Draw(CmdBuffer, { DepthView });



	//logger->checkResults(vkEndCommandBuffer(CmdBuffer));
}

void LightCamera::loadMatModel(string name, string path, glm::vec3 translation)
{
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	std::string err;

	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, name.c_str(), path.c_str());

	for (int i(0); i< shapes.size(); i++)
	{
		FirstWorldMaterialModel* m;

		vector<TexturedVertex> vert;

		for (auto j : shapes[i].mesh.indices)
		{
			vert.push_back(TexturedVertex(attrib.vertices[3 * j.vertex_index], -attrib.vertices[3 * j.vertex_index + 1], attrib.vertices[3 * j.vertex_index + 2],
				attrib.texcoords[2 * j.texcoord_index], 1.0f - attrib.texcoords[2 * j.texcoord_index + 1],
				attrib.normals[3 * j.normal_index], attrib.normals[3 * j.normal_index + 1], attrib.normals[3 * j.normal_index + 2],
				attrib.texcoords[2 * j.texcoord_index], 1.0f - attrib.texcoords[2 * j.texcoord_index + 1]));
		}

		m = new FirstWorldMaterialModel(logger, Device, glm::vec3(), translation, vert);
		static_cast<LightMaterialPipeline*>(Pipelines.back())->Models.push_back(m);
	}
}

LightCamera::~LightCamera()
{
}
