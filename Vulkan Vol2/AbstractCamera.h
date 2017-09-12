#pragma once
#include "AbstractBasePipelineWrapper.h"

#include <tiny_obj_loader.h>

class AbstractCamera
{
public:
	vector<AbstractBasePipelineWrapper*> Pipelines;
	virtual void Draw(VkCommandBuffer CmdBuffer, VkImage image) = 0;
	AbstractCamera();
	~AbstractCamera();
};

