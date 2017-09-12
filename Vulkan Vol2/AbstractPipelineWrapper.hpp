#pragma once
#include "AbstractBasePipelineWrapper.h"

using namespace std;

//TODO параметризовать по Descriptor Set
template<class T, class Model, int CountOfOutImages>
class AbstractPipelineWrapper:
	public AbstractBasePipelineWrapper
{
	static_assert(std::is_base_of<AbstractVertex, T >::value, "T does not extend OtherClass");
protected:
	VkShaderModule loadSPIRVShader(std::string filename);
	VkPipeline Pipeline;
	LogicDeviceWrapper Device;
	Logger *logger;
public:
	vector<Model*> Models;
	AbstractPipelineWrapper(Logger *logger, LogicDeviceWrapper Device);
	void Draw(VkCommandBuffer CmdBuffer, array<VkImageView, CountOfOutImages> ImageViews);
	virtual ~AbstractPipelineWrapper();
	VkPipeline GetPipeline();
};


template<class Vertex, class Model, int CountOfOutImages>
inline VkShaderModule AbstractPipelineWrapper<Vertex, Model, CountOfOutImages>::loadSPIRVShader(std::string filename)
{
	size_t shaderSize;
	char* shaderCode;

#if defined(__ANDROID__)
	// Load shader from compressed asset
	AAsset* asset = AAssetManager_open(androidApp->activity->assetManager, filename.c_str(), AASSET_MODE_STREAMING);
	assert(asset);
	shaderSize = AAsset_getLength(asset);
	assert(shaderSize > 0);

	shaderCode = new char[shaderSize];
	AAsset_read(asset, shaderCode, shaderSize);
	AAsset_close(asset);
#else
	std::ifstream is(filename, std::ios::binary | std::ios::in | std::ios::ate);

	if (is.is_open())
	{
		shaderSize = is.tellg();
		is.seekg(0, std::ios::beg);
		// Copy file contents into a buffer
		shaderCode = new char[shaderSize];
		is.read(shaderCode, shaderSize);
		is.close();
		assert(shaderSize > 0);
	}
#endif
	if (shaderCode)
	{
		// Create a new shader module that will be used for pipeline creation
		VkShaderModuleCreateInfo moduleCreateInfo{};
		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleCreateInfo.codeSize = shaderSize;
		moduleCreateInfo.pCode = (uint32_t*)shaderCode;

		VkShaderModule shaderModule;
		logger->checkResults(vkCreateShaderModule(Device.GetLogicDevice(), &moduleCreateInfo, NULL, &shaderModule));

		delete[] shaderCode;

		return shaderModule;
	}
	else
	{
		logger->LogMessage("Error: Could not open shader file \"" + filename + "\"");
		return VK_NULL_HANDLE;
	}
}

template<class T, class Model, int CountOfOutImages>
inline AbstractPipelineWrapper<T, Model, CountOfOutImages>::AbstractPipelineWrapper(Logger *logger, LogicDeviceWrapper device):
	AbstractBasePipelineWrapper(), Device(device)
{
		this->logger = logger;
}

template<class T, class Model,  int CountOfOutImages>
inline void AbstractPipelineWrapper<T, Model, CountOfOutImages>::Draw(VkCommandBuffer CmdBuffer, array<VkImageView, CountOfOutImages> ImageViews)
{
	_Draw(CmdBuffer, vector<VkImageView>(ImageViews.begin(), ImageViews.end()));
}

template<class T, class Model, int CountOfOutImages>
inline AbstractPipelineWrapper<T, Model, CountOfOutImages>::~AbstractPipelineWrapper()
{
}

template<class T, class Model, int CountOfOutImages>
inline VkPipeline AbstractPipelineWrapper<T, Model, CountOfOutImages>::GetPipeline()
{
	return Pipeline;
}
