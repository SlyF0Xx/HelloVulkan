#pragma once

template<class T>
class AbstractModelBase
{
public:
	AbstractModelBase();
	virtual void Draw() = 0;
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
