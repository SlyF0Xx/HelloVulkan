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
	glm::vec3 rotation;
	glm::vec3 translation;
public:
	AbstractWorldModel(glm::vec3 rotation, glm::vec3 translation = glm::vec3(0));
	~AbstractWorldModel();
	glm::vec3 GetRotation();
	glm::vec3 GetTranslation();
};

template<class T>
inline AbstractWorldModel<T>::AbstractWorldModel(glm::vec3 rotation, glm::vec3 translation)
	:AbstractModelBase<T>(), rotation(rotation), translation(translation)
{

}

template<class T>
inline AbstractWorldModel<T>::~AbstractWorldModel()
{
}

template<class T>
inline glm::vec3 AbstractWorldModel<T>::GetRotation()
{
	return rotation;
}

template<class T>
inline glm::vec3 AbstractWorldModel<T>::GetTranslation()
{
	return translation;
}
