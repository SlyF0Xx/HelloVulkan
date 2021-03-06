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
#include "MatrixBuffer.h"

#pragma comment (lib, "vulkan-1.lib")

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class TransformationBuffer
{
private:
	VkBuffer Buffer;
	VkDeviceMemory memory;
	vector<VkDescriptorSetLayout> Descriptors;
	VkDescriptorSet Descriptor;
	LogicDeviceWrapper Devaice;
public:
	struct data
	{
		glm::mat4 TransformationMatrix;
		glm::vec3 LightPos;
		glm::vec3 LightDir;
	}light;

	TransformationBuffer(LogicDeviceWrapper LogicDevaice, MatrixBuffer MainMatrixes);
	~TransformationBuffer();
	void TransformationBuffer::SetMatrix(MatrixBuffer MainMatrixes);
	VkBuffer GetBuffer();
};

