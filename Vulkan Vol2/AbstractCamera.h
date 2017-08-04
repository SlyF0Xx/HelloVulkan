#pragma once
#include "AbstractBasePipelineWrapper.h"

class AbstractCamera
{
public:
	vector<AbstractBasePipelineWrapper*> Pipelines;
	virtual void Draw(VkCommandBuffer CmdBuffer, VkImage image) = 0;
	virtual void keyPressed(uint32_t keyCode) = 0;
	AbstractCamera();
	~AbstractCamera();
};

