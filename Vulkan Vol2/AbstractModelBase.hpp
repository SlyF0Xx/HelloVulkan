#pragma once

template<class T>
class AbstractModelBase
{
public:
	AbstractModelBase();
	virtual void Draw(VkCommandBuffer CmdBuffer) = 0;
	virtual ~AbstractModelBase();
};


template<class T>
AbstractModelBase<T>::AbstractModelBase()
{
}

template<class T>
AbstractModelBase<T>::~AbstractModelBase()
{
}
