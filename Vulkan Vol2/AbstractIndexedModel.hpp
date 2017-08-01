#pragma once
#include "AbstractModel.hpp"

template<class Vertex, class VertexBuffer, class IndexBuffer>
class AbstractIndexedModel :
	public AbstractModel<Vertex, VertexBuffer>
{
protected:
	IndexBuffer IndBuffer;
public:
	~AbstractIndexedModel();
	AbstractIndexedModel(LogicDeviceWrapper device, vector<Vertex> vertexes, vector<uint32_t> indexes);
};

template<class Vertex, class VertexBuffer, class IndexBuffer>
inline AbstractIndexedModel<Vertex, VertexBuffer, IndexBuffer>::AbstractIndexedModel(LogicDeviceWrapper device, vector<Vertex> vertexes, vector<uint32_t> indexes) :
	AbstractModel(device, vertexes), IndBuffer(device, indexes)
{
	IndBuffer.WriteIndexes();
}

template<class Vertex, class VertexBuffer, class IndexBuffer>
inline AbstractIndexedModel<Vertex, VertexBuffer, IndexBuffer>::~AbstractIndexedModel()
{
}
