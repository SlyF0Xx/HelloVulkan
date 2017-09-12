#pragma once
#include "AbstractWorldModel.hpp"
#include "AbstractMaterialModel.hpp"
#include "AbstractModel.hpp"

template<class Vertex, class VertexBuffer/*, class Material*/>
class AbstractWorldMaterialModel :
	public AbstractWorldModel<Vertex>, 
	//public AbstractMaterialModel<Vertex, Material>,
	public AbstractModel<Vertex, VertexBuffer>
{
public:
	void Draw(VkCommandBuffer CmdBuffer);
	AbstractWorldMaterialModel(LogicDeviceWrapper Device, vector<Vertex> vertexes, glm::vec3 rotation, glm::vec3 translation/*, Material material*/);
	~AbstractWorldMaterialModel();
};



template<class Vertex, class VertexBuffer/*, class Material*/>
inline void AbstractWorldMaterialModel<Vertex, VertexBuffer/*, Material*/>::Draw(VkCommandBuffer CmdBuffer)
{
	VkBuffer temp = VertBuffer.GetBuffer();
	VkDeviceSize offsets[1] = { 0 };
	vkCmdBindVertexBuffers(CmdBuffer, 0, 1, &temp, offsets);

	vkCmdDraw(CmdBuffer, VertBuffer.GetVertexes().size(), 1, 0, 0);
}

template<class Vertex, class VertexBuffer/*, class Material*/>
inline AbstractWorldMaterialModel<Vertex, VertexBuffer/*, Material*/>::AbstractWorldMaterialModel(LogicDeviceWrapper Device, vector<Vertex> vertexes,
	glm::vec3 rotation, glm::vec3 translation/*, Material material*/)
	:AbstractWorldModel<Vertex>(rotation, translation), 
	//AbstractMaterialModel<Vertex, Material>(material),
	AbstractModel<Vertex, VertexBuffer>(Device, vertexes)
{
}

template<class Vertex, class VertexBuffer/*, class Material*/>
inline AbstractWorldMaterialModel<Vertex, VertexBuffer/*, Material*/>::~AbstractWorldMaterialModel()
{
}
