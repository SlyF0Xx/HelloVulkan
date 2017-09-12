#pragma once
#include "AbstractCamera.h"

class AbstractScene
{
public:

	vector<AbstractCamera*> cameras;
	virtual void Draw() = 0;
	AbstractScene();
	~AbstractScene();
};

