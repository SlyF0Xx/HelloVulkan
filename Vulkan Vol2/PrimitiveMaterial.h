#pragma once
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif // _WIN32

#include <vulkan\vulkan.h>
#include <vulkan\vk_platform.h>
#include <vector>
#include "Logger.h"
#include <string>
#include "AbstractMaterial.h"
#include "LogicDeviceWrapper.h"

#pragma comment (lib, "vulkan-1.lib")

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

class PrimitiveMaterial :
	public AbstractMaterial
{
protected:
	Logger *logger;
	glm::vec3 diffuse, specular, ambient;
	VkImage NormalImage;
	VkImageView NormalMap;
	LogicDeviceWrapper Device;
	static const VkFormat NormalFormat = VK_FORMAT_R8G8B8A8_UNORM;

	VkImage TextureImage;
	VkImageView TextureView;
public:
	PrimitiveMaterial(Logger *logger, LogicDeviceWrapper device, string normalMapName, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, string imageName);
	VkImageView GetNormalMap();
	VkImageView GetTexture();
	VkImage CreateImage(string normalMapName);
	VkImageView CreateImageView(VkImage Image);
	~PrimitiveMaterial();
};

