#define VK_USE_PLATFORM_WIN32_KHR

#include <Windows.h>
#include <stdlib.h>
#include<time.h>
#include <string>
#include "Logger.h"
#include "Foundation.h"
#include <vulkan\vk_platform.h>
#include <vulkan\vulkan.h>

#include "Model.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef TINYOBJLOADER_IMPLEMENTATION
	#define TINYOBJLOADER_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
#endif

#include "FirstWorldMaterialModel.h"
#include "PrimitiveModel.h"
#include "PrimitiveIndexedModel.h"
#include "ColorModel.h"
#include "PrimitiveCamera.h"
#include "LightCamera.h"
#include "ShadowCamera.h"
#include "PrimitiveScene.h"
#include "MaterialPipelineWrapper.h"
#include "MatrixBuffer.h"
#include "TransformationBuffer.h"

#pragma comment (lib, "vulkan-1.lib")


using namespace std;


const int ScreenX = 900;
const int ScreenY = 600;
const float zoom = -2.5f;
glm::vec3 rotation = glm::vec3();

#ifdef _WIN32
WNDCLASSEX windowsclass;
MSG msg;
HWND hwnd;
HINSTANCE HInstance;
#endif


Logger logger;
AbstractControlCamera* ControlCamera;
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN:
	{
		ControlCamera->keyPressed(wparam);
		break;
	}
	}


	return (DefWindowProc(hwnd, msg, wparam, lparam));
}


VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t object,
	size_t location,
	int32_t messageCode,
	const char *pLayerPrefix,
	const char *pMessage,
	void *pUserData)
{
	logger.LogMessage(pMessage);
	return VK_FALSE;
}


void ComputeTangentBasis(
	const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3,
	const glm::vec2& UV1, const glm::vec2& UV2, const glm::vec2& UV3,
	glm::vec3 &tangent, glm::vec3 &bitangent)
{
	glm::vec3 Edge1 = P2 - P1;
	glm::vec3 Edge2 = P3 - P1;
	glm::vec2 Edge1uv = UV2 - UV1;
	glm::vec2 Edge2uv = UV3 - UV1;

	float cp = Edge1uv.y * Edge2uv.x - Edge1uv.x * Edge2uv.y;

	if (cp != 0.0f) {
		float mul = 1.0f / cp;
		tangent = (Edge1 * -Edge2uv.y + Edge2 * Edge1uv.y) * mul;
		bitangent = (Edge1 * -Edge2uv.x + Edge2 * Edge1uv.x) * mul;

		glm::normalize(tangent);
		glm::normalize(bitangent);
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	windowsclass.cbSize = sizeof(windowsclass);
	windowsclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	windowsclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowsclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowsclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	windowsclass.hInstance = hInstance;
	windowsclass.lpfnWndProc = WinProc;
	windowsclass.lpszClassName = L"windowsclass";
	windowsclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowsclass);

	hwnd = CreateWindow(
		L"windowsclass",
		L"VulkanApp",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100,
		ScreenX,
		ScreenY,
		NULL,
		NULL,
		hInstance,
		NULL);
	HInstance = hInstance;

	logger.LogMessage("Start");

	try {
		Foundation foundation = Foundation(&logger);
		
#ifdef _DEBUG
		PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = NULL;
		PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = NULL;
		fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)
			vkGetInstanceProcAddr(foundation.GetInstance(), "vkCreateDebugReportCallbackEXT");
		fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)
			vkGetInstanceProcAddr(foundation.GetInstance(), "vkDestroyDebugReportCallbackEXT");

		VkDebugReportCallbackCreateInfoEXT debug_report_callback_info;
		memset(&debug_report_callback_info, 0, sizeof(debug_report_callback_info));
		debug_report_callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debug_report_callback_info.flags = VK_DEBUG_REPORT_DEBUG_BIT_EXT |
			VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
		debug_report_callback_info.pfnCallback = DebugReportCallback;

		VkDebugReportCallbackEXT debug_report_callback = VK_NULL_HANDLE;
		logger.checkResults(fvkCreateDebugReportCallbackEXT(foundation.GetInstance(), &debug_report_callback_info, nullptr, &debug_report_callback));
#endif // DEBUG

		foundation.CreateDevices();
		logger.LogMessage("Generate Devacie");
		
		
		foundation.addSurface(HInstance, hwnd);

		logger.LogMessage("Generate Surface");
		logger.LogMessage(foundation.GetSurfaces()[0]);

		for (auto &i : foundation.GetPhysDevices())
		{
			i.addLogicDevice(1);
			logger.LogMessage("Generate Logic Devacie");
			logger.LogMessage(i.GetLogicDevices()[0].GetLogicDevice());

			for (auto &j : i.GetLogicDevices())
			{
				foundation.addSwapchain(i.GetPhysDevice(), foundation.GetSurfaces()[0], &j);
				logger.LogMessage("Generate Swapchain");
				for (auto k : j.GetQueuesFamilies())
				{
					j.addCommandPool(k.Index);
					logger.LogMessage("Generate Queue Family");
				}
				for (CommandPoolWrapper& k : j.GetCommandPools())
				{
					j.addCommandBuffers(&k, true, j.GetSwapchins()[0].GetImages().size()*2);					
					logger.LogMessage("Generate Command Pool");
				}
			}
		}


		ZeroMemory(&msg, sizeof(msg));

		ShowWindow(hwnd, nShowCmd);
		UpdateWindow(hwnd);
		ZeroMemory(&msg, sizeof(msg));


		logger.LogMessage("Generate");

		PrimitiveModel* mode = new PrimitiveModel(foundation.GetPhysDevices()[0].GetLogicDevices()[0], glm::vec3(0, 0, 0), glm::vec3(0,0,6));
		//PrimitiveIndexedModel* Imode = new PrimitiveIndexedModel(foundation.GetPhysDevices()[0].GetLogicDevices()[0]);
		//PrimitiveModel* mode = PrimitiveModel::create(foundation.GetPhysDevices()[0].GetLogicDevices()[0]);
		//PrimitiveIndexedModel* Imode = PrimitiveIndexedModel::create(foundation.GetPhysDevices()[0].GetLogicDevices()[0]);
		ColorModel * model = new ColorModel(foundation.GetPhysDevices()[0].GetLogicDevices()[0], glm::vec3(), glm::vec3(0, 0, 5));


		logger.LogMessage("Done");


		VkSemaphoreCreateInfo SemaphoreInfo;
		SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		SemaphoreInfo.pNext = NULL;
		SemaphoreInfo.flags = 0;

		VkSemaphore ImageReady;
		VkSemaphore CalculationReady;
		logger.checkResults(vkCreateSemaphore(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &SemaphoreInfo, nullptr, &ImageReady));
		logger.checkResults(vkCreateSemaphore(foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetLogicDevice(), &SemaphoreInfo, nullptr, &CalculationReady));

		logger.LogMessage("Prepare");
		
		glm::vec3 tangient, bitangient;
		/*ComputeTangentBasis(
			glm::vec3(-6.0f, 1.5f, 0.0f),
			glm::vec3(6.0f, 1.5f, 0.0f),
			glm::vec3(6.0f, 0.0f, 2.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),

			/*glm::vec3(6.0f, -2.0f, 0.0f),
			glm::vec3(-6.0f, 1.5f, 0.0f),
			glm::vec3(6.0f, 1.5f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
			tangient, bitangient
		);*/

		/*tinyobj::attrib_t attrib;
		vector<tinyobj::shape_t> shapes;
		vector<tinyobj::material_t> materials;
		std::string err;

		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "C:/Users/SlyFox/Downloads/Box/BOX.obj", "C:/Users/SlyFox/Downloads/Box/");
		FirstWorldMaterialModel* m;
		
		vector<TexturedVertex> vert;
		vector<MaterialPipelineWrapper*> pipes;
		/*for (int i(0); i < shapes[1].mesh.indices.size(); i+=3)
		{
			glm::vec3 pos1(attrib.vertices[3 * shapes[1].mesh.indices[i].vertex_index],
				attrib.vertices[3 * shapes[1].mesh.indices[i].vertex_index + 1], attrib.vertices[3 * shapes[1].mesh.indices[i].vertex_index + 2]);
			glm::vec3 pos2(attrib.vertices[3 * shapes[1].mesh.indices[i+1].vertex_index],
				attrib.vertices[3 * shapes[1].mesh.indices[i+1].vertex_index + 1], attrib.vertices[3 * shapes[1].mesh.indices[i+1].vertex_index + 2]);
			glm::vec3 pos3(attrib.vertices[3 * shapes[1].mesh.indices[i+2].vertex_index],
				attrib.vertices[3 * shapes[1].mesh.indices[i+2].vertex_index + 1], attrib.vertices[3 * shapes[1].mesh.indices[i+2].vertex_index + 2]);

			glm::vec2 uv1(attrib.texcoords[2 * shapes[1].mesh.indices[i].texcoord_index], attrib.texcoords[2 * shapes[1].mesh.indices[i].texcoord_index + 1]);
			glm::vec2 uv2(attrib.texcoords[2 * shapes[1].mesh.indices[i+1].texcoord_index], attrib.texcoords[2 * shapes[1].mesh.indices[i+1].texcoord_index + 1]);
			glm::vec2 uv3(attrib.texcoords[2 * shapes[1].mesh.indices[i+2].texcoord_index], attrib.texcoords[2 * shapes[1].mesh.indices[i+2].texcoord_index + 1]);

			glm::vec3 norm1(attrib.normals[3 * shapes[1].mesh.indices[i].normal_index],
				attrib.normals[3 * shapes[1].mesh.indices[i].normal_index + 1], attrib.normals[3 * shapes[1].mesh.indices[i].normal_index + 2]);
			glm::vec3 norm2(attrib.normals[3 * shapes[1].mesh.indices[i+1].normal_index],
				attrib.normals[3 * shapes[1].mesh.indices[i+1].normal_index + 1], attrib.normals[3 * shapes[1].mesh.indices[i+1].normal_index + 2]);
			glm::vec3 norm3(attrib.normals[3 * shapes[1].mesh.indices[i+2].normal_index],
				attrib.normals[3 * shapes[1].mesh.indices[i+2].normal_index + 1], attrib.normals[3 * shapes[1].mesh.indices[i+2].normal_index + 2]);

			ComputeTangentBasis(pos1, pos2, pos3, uv1, uv2, uv3, tangient, bitangient);
			vert.push_back(TexturedVertex(pos1.x, pos1.y, pos1.z, uv1.x, uv1.y, norm1.x, norm1.y, norm1.z, tangient.x, tangient.y, tangient.z, bitangient.x, bitangient.y, bitangient.z, uv1.x, uv1.y));

			vert.push_back(TexturedVertex(pos2.x, pos2.y, pos2.z, uv2.x, uv2.y, norm2.x, norm2.y, norm2.z, tangient.x, tangient.y, tangient.z, bitangient.x, bitangient.y, bitangient.z, uv2.x, uv2.y));

			vert.push_back(TexturedVertex(pos3.x, pos3.y, pos3.z, uv3.x, uv3.y, norm3.x, norm3.y, norm3.z, tangient.x, tangient.y, tangient.z, bitangient.x, bitangient.y, bitangient.z, uv3.x, uv3.y));
		}


		for (auto j : shapes[1].mesh.indices)
		{
			vert.push_back(TexturedVertex(attrib.vertices[3 * j.vertex_index], attrib.vertices[3 * j.vertex_index + 1], attrib.vertices[3 * j.vertex_index + 2],
				attrib.texcoords[2 * j.texcoord_index], 1.0f - attrib.texcoords[2 * j.texcoord_index + 1],
				attrib.normals[3 * j.normal_index], attrib.normals[3 * j.normal_index + 1], attrib.normals[3 * j.normal_index + 2],
				attrib.texcoords[2 * j.texcoord_index], 1.0f - attrib.texcoords[2 * j.texcoord_index + 1]));
		}

		m = new FirstWorldMaterialModel(&logger, foundation.GetPhysDevices()[0].GetLogicDevices()[0], glm::vec3(), glm::vec3(-8, -8, 30), vert);
		*/

		PrimitiveScene scene(&logger, foundation.GetPhysDevices()[0].GetLogicDevices()[0], ScreenX, ScreenY, zoom, foundation.GetSurfaceFormat(), 
			foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetSwapchins()[0], foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetCommandPools()[0],
			foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Queues[0]);


		//FirstWorldMaterialModel* m = new FirstWorldMaterialModel(&logger, foundation.GetPhysDevices()[0].GetLogicDevices()[0], glm::vec3(), glm::vec3(0,0,6));
		

		//ControlCamera = static_cast<LightCamera*>(scene.cameras[0]);
		ControlCamera = static_cast<ShadowCamera*>(scene.cameras[0]);


		static_cast<LightPrimitivePipeline*>(static_cast<LightCamera*>(scene.cameras[1])->Pipelines[0])->Models.push_back((AbstractWorldModel<PrimitiveVertex>*)mode);
		static_cast<LightColorPipeline*>(static_cast<LightCamera*>(scene.cameras[1])->Pipelines[1])->
			Models.push_back((AbstractWorldModel<ColorVertex>*)model);
		//static_cast<LightMaterialPipeline*>(static_cast<LightCamera*>(scene.cameras[0])->Pipelines[2])->
		//	Models.push_back((AbstractWorldModel<TexturedVertex>*)m);

		//static_cast<LightColorPipeline*>(static_cast<LightCamera*>(scene.cameras[0])->Pipelines[1])->Models.push_back((AbstractWorldModel<ColorVertex>*)m);


		static_cast<ShadowPrimitivePipeline*>(static_cast<ShadowCamera*>(scene.cameras[0])->Pipelines[0])->Models.push_back((AbstractWorldModel<PrimitiveVertex>*)mode);
		static_cast<ShadowColorPipeline*>(static_cast<ShadowCamera*>(scene.cameras[0])->Pipelines[1])->
			Models.push_back((AbstractWorldModel<ColorVertex>*)model);
		//static_cast<MaterialPipelineWrapper*>(static_cast<ShadowCamera*>(scene.cameras[1])->Pipelines[2])->
		//	Models.push_back((AbstractWorldMaterialModel<TexturedVertex, PrimitiveVertexBufferWrapper<TexturedVertex>>*)m);

		static_cast<ShadowCamera*>(scene.cameras[0])->loadMatModel("C:/Users/SlyFox/Downloads/Box/BOX.obj", "C:/Users/SlyFox/Downloads/Box/", glm::vec3(-8, 8, 30));
		//static_cast<ShadowCamera*>(scene.cameras[0])->loadMatModel("C:/Users/SlyFox/Downloads/Trunk/tronco.obj", "C:/Users/SlyFox/Downloads/Trunk/", glm::vec3(-8, 12, 50));
			
		static_cast<LightCamera*>(scene.cameras[1])->loadMatModel("C:/Users/SlyFox/Downloads/Box/BOX.obj", "C:/Users/SlyFox/Downloads/Box/", glm::vec3(-8, 8, 30));
		//static_cast<LightCamera*>(scene.cameras[1])->loadMatModel("C:/Users/SlyFox/Downloads/Trunk/tronco.obj", "C:/Users/SlyFox/Downloads/Trunk/", glm::vec3(-8, 12, 50));



		scene.Update();
		
		
		logger.LogMessage("All prepare for draw");

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			else
			{
				logger.LogMessage("Start draw");
				
				//static_cast<LightCamera*>(scene.cameras[1])->MainMatrixes.RotateRight();
				//static_cast<ShadowCamera*>(scene.cameras[0])->TransformationMat.SetMatrix(static_cast<LightCamera*>(scene.cameras[1])->MainMatrixes);

				scene.Draw(ImageReady, CalculationReady, foundation.GetPhysDevices()[0].GetLogicDevices()[0].GetQueuesFamilies()[0].Queues[0]);
			}
		}

	}	
	catch (exception e)
	{
		logger.LogMessage((string) e.what());
	}
}