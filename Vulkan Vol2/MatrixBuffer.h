#pragma once
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif // _WIN32

#include <vulkan\vulkan.h>
#include <vulkan\vk_platform.h>
#include <vector>
#include "Logger.h"
#include <string>
#include "Foundation.h"
#include "AbstractModel.hpp"

#pragma comment (lib, "vulkan-1.lib")

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MatrixBuffer
{
private:
	VkBuffer Buffer;
	VkDeviceMemory memory;
	vector<VkDescriptorSetLayout> Descriptors;
	VkDescriptorSet Descriptor;
	glm::vec3 dirView = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 dirUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, -2.5f);
	const float scale = glm::pi<double>() / 360.0;
	LogicDeviceWrapper Devaice;
public:
	struct Matrixes
	{
		//glm::mat4 WorldMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
	}Matrixs;
	MatrixBuffer(LogicDeviceWrapper LogicDevaice, int ScreenX, int ScreenY, float zoom);
	~MatrixBuffer();


	glm::vec3 GetDirView();
	glm::vec3 GetPosition();
	VkBuffer GetBuffer();
	void Update();
	void GoForward();
	void GoBackward();
	void StrafeLeft();
	void StrafeRight();
	void Up();
	void Down();
	void RotateUp();
	void RotateDown();
	void RotateLeft();
	void RotateRight();
	void RotateZUp();
	void RotateZDown();
};

