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
	VkPipeline Pipeline;
public:
	vector<Model*> Models;
	vector<VkCommandBuffer> CmdBuffers;
	AbstractPipelineWrapper();
	void Draw(VkCommandBuffer CmdBuffer, array<VkImageView, CountOfOutImages> ImageViews);
	virtual ~AbstractPipelineWrapper();
	VkPipeline GetPipeline();
};

template<class T, class Model, int CountOfOutImages>
inline AbstractPipelineWrapper<T, Model, CountOfOutImages>::AbstractPipelineWrapper():AbstractBasePipelineWrapper()
{
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
