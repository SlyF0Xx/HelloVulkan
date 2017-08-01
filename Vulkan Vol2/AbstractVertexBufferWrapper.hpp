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
#include "AbstractVertex.h"

#pragma comment (lib, "vulkan-1.lib")

using namespace std;

template<class T>
class AbstractVertexBufferWrapper
{
	static_assert(std::is_base_of<AbstractVertex, T >::value, "T does not extend OtherClass");
protected:
	VkBuffer Buffer;
	VkDeviceMemory Memory;
	LogicDeviceWrapper Device;
	vector<T> Vertexes;
public:
	vector<T> GetVertexes();
	VkBuffer GetBuffer();
	//void SetVertexes();
	virtual void WriteVertexes() = 0;
	AbstractVertexBufferWrapper(LogicDeviceWrapper device, vector<T> vertexes);
	~AbstractVertexBufferWrapper();
};

template<class T>
inline vector<T> AbstractVertexBufferWrapper<T>::GetVertexes()
{
	return Vertexes;
}

template<class T>
inline VkBuffer AbstractVertexBufferWrapper<T>::GetBuffer()
{
	return Buffer;
}

template<class T>
inline AbstractVertexBufferWrapper<T>::AbstractVertexBufferWrapper(LogicDeviceWrapper device, vector<T> vertexes) : Device(device), Vertexes(vertexes)
{
}

/*template<class T>
inline void AbstractVertexBufferWrapper<T>::SetVertexes(vector<T> vertexes)
{
	Vertexes = vertexes;
}*/

template<class T>
inline AbstractVertexBufferWrapper<T>::~AbstractVertexBufferWrapper()
{
}