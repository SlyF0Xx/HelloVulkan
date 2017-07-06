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

using namespace std;

template<class T>
class AbstractPipelineWrapper
{
	static_assert(std::is_base_of<AbstractVertex, T >::value, "T does not extend OtherClass");
protected:
	VkPipeline Pipeline;
public:
	vector<AbstractModelBase<T>*> Models;
	AbstractPipelineWrapper();
	virtual ~AbstractPipelineWrapper();
};

template<class T>
inline AbstractPipelineWrapper<T>::AbstractPipelineWrapper()
{
}

template<class T>
inline AbstractPipelineWrapper<T>::~AbstractPipelineWrapper()
{
}