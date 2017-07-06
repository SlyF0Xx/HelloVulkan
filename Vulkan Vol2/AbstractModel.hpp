#pragma once
#include "VertexBufferWrapper.hpp"
#include "AbstractModelBase.hpp"

template<class T, class Self>
class AbstractModel : AbstractModelBase<T>
{
protected:
	VertexBufferWrapper<T> VertBuffer;
public:
	AbstractModel(LogicDeviceWrapper Device);
	virtual ~AbstractModel();
};


template<class T, class Self>
inline AbstractModel<T, Self>::AbstractModel(LogicDeviceWrapper Device) : AbstractModelBase<T>(), VertBuffer(Device)
{
	static_cast<Self*>(this)->Init();
	VertBuffer.WriteVertexes();
}

template<class T, class Self>
inline AbstractModel<T, Self>::~AbstractModel()
{
}