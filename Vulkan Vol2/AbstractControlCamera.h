#pragma once
#include "AbstractCamera.h"
class AbstractControlCamera :
	public AbstractCamera
{
public:
	virtual void keyPressed(uint32_t keyCode) = 0;
	AbstractControlCamera();
	~AbstractControlCamera();
};

