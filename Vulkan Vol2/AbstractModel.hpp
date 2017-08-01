#pragma once
#include "AbstractVertexBufferWrapper.hpp"
#include "AbstractModelBase.hpp"

template<class T, class VertexBuffer>
class AbstractModel : AbstractModelBase<T>
{
	static_assert(std::is_base_of<AbstractVertexBufferWrapper<T>, VertexBuffer>::value, "T does not extend OtherClass");
protected:
	VertexBuffer VertBuffer;
public:
	//void Init() {};
//	AbstractModel(LogicDeviceWrapper Device);
	virtual ~AbstractModel();
	AbstractModel(LogicDeviceWrapper Device, vector<T> vertexes);
};


template<class T, class VertexBuffer>
inline AbstractModel<T, VertexBuffer>::~AbstractModel()
{
}

template<class T, class VertexBuffer>
inline AbstractModel<T, VertexBuffer>::AbstractModel(LogicDeviceWrapper Device, vector<T> vertexes) : AbstractModelBase<T>(), VertBuffer(Device, vertexes)
{
	//static_cast<Self*>(this)->Init();
	VertBuffer.WriteVertexes();
}