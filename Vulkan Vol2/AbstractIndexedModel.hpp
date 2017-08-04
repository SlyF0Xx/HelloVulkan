#pragma once
#include "AbstractWorldModel.hpp"
#include "AbstractModel.hpp"

template<class Vertex, class VertexBuffer, class IndexBuffer>
class AbstractIndexedModel :
	public AbstractModel<Vertex, VertexBuffer>,
	public AbstractWorldModel<Vertex>
{
protected:
	IndexBuffer IndBuffer;
public:
	~AbstractIndexedModel();
	AbstractIndexedModel(LogicDeviceWrapper device, vector<Vertex> vertexes, vector<uint32_t> indexes, glm::vec3 rotation);
};

template<class Vertex, class VertexBuffer, class IndexBuffer>
inline AbstractIndexedModel<Vertex, VertexBuffer, IndexBuffer>::AbstractIndexedModel(LogicDeviceWrapper device, vector<Vertex> vertexes, vector<uint32_t> indexes, glm::vec3 rotation) :
	AbstractModel(device, vertexes), AbstractWorldModel(device, rotation), IndBuffer(device, indexes)
{
	IndBuffer.WriteIndexes();
}

template<class Vertex, class VertexBuffer, class IndexBuffer>
inline AbstractIndexedModel<Vertex, VertexBuffer, IndexBuffer>::~AbstractIndexedModel()
{
}
