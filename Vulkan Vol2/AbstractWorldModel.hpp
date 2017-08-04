#pragma once
#include "AbstractModelBase.hpp"
#include "WorldBuffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

template<class T>
class AbstractWorldModel:
	public AbstractModelBase<T>
{
protected:
	WorldBuffer World;
public:
	AbstractWorldModel(LogicDeviceWrapper Device, glm::vec3 rotation);
	~AbstractWorldModel();
	WorldBuffer GetWorld();
};

template<class T>
inline AbstractWorldModel<T>::AbstractWorldModel(LogicDeviceWrapper Device, glm::vec3 rotation):AbstractModelBase<T>(), World(Device, rotation)
{

}

template<class T>
inline AbstractWorldModel<T>::~AbstractWorldModel()
{
}

template<class T>
inline WorldBuffer AbstractWorldModel<T>::GetWorld()
{
	return World;
}
