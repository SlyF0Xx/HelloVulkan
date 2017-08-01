#pragma once
#include "AbstractBasePipelineWrapper.h"

using namespace std;

//TODO параметризовать по Descriptor Set
template<class T, int CountOfOutImages>
class AbstractPipelineWrapper:
	AbstractBasePipelineWrapper
{
	static_assert(std::is_base_of<AbstractVertex, T >::value, "T does not extend OtherClass");
protected:
	VkPipeline Pipeline;
public:
	vector<AbstractModelBase<T>*> Models;
	vector<VkCommandBuffer> CmdBuffers;
	AbstractPipelineWrapper();
	void Draw(VkCommandBuffer CmdBuffer, array<VkImageView, CountOfOutImages> ImageViews);
	virtual ~AbstractPipelineWrapper();
	VkPipeline GetPipeline();
};

template<class T, int CountOfOutImages>
inline AbstractPipelineWrapper<T, CountOfOutImages>::AbstractPipelineWrapper():AbstractBasePipelineWrapper()
{
}

template<class T, int CountOfOutImages>
inline void AbstractPipelineWrapper<T, CountOfOutImages>::Draw(VkCommandBuffer CmdBuffer, array<VkImageView, CountOfOutImages> ImageViews)
{
	_Draw(CmdBuffer, vector<VkImageView>(ImageViews.begin(), ImageViews.end()));
}

template<class T, int CountOfOutImages>
inline AbstractPipelineWrapper<T, CountOfOutImages>::~AbstractPipelineWrapper()
{
}

template<class T, int CountOfOutImages>
inline VkPipeline AbstractPipelineWrapper<T, CountOfOutImages>::GetPipeline()
{
	return Pipeline;
}
