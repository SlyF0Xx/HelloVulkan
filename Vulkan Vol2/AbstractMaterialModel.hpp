#pragma once
#include "AbstractWorldModel.hpp"
#include "AbstractMaterial.h"

template<class Vertex, class Material>
class AbstractMaterialModel :
	public AbstractModelBase<Vertex>
{
protected:
	Material material;
public:
	AbstractMaterialModel(Material material);
	Material GetMaterial();
	~AbstractMaterialModel();
};

template<class Vertex, class Material>
inline AbstractMaterialModel<Vertex, Material>::AbstractMaterialModel(Material material)
	:material(material)
{
}

template<class Vertex, class Material>
inline Material AbstractMaterialModel<Vertex, Material>::GetMaterial()
{
	return material;
}

template<class Vertex, class Material>
inline AbstractMaterialModel<Vertex, Material>::~AbstractMaterialModel()
{
}
