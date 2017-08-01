#pragma once
#include "AbstractBasePipelineWrapper.h"

class AbstractCamera
{
public:
	vector<AbstractBasePipelineWrapper> Pipelines;
	virtual void Draw(VkCommandBuffer CmdBuffer) = 0;
	AbstractCamera();
	~AbstractCamera();
};

