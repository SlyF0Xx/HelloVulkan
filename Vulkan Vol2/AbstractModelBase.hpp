#pragma once
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan\vk_platform.h>
#include <vulkan\vulkan.h>

#pragma comment (lib, "vulkan-1.lib")

template<class T>
class AbstractModelBase
{
public:
	AbstractModelBase();
	virtual void Draw(VkCommandBuffer CmdBuffer) = 0;
	virtual ~AbstractModelBase();
};


template<class T>
AbstractModelBase<T>::AbstractModelBase()
{
}

template<class T>
AbstractModelBase<T>::~AbstractModelBase()
{
}
