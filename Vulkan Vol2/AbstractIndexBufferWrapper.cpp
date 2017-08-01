#include "AbstractIndexBufferWrapper.h"


VkBuffer AbstractIndexBufferWrapper::GetBuffer()
{
	return Buffer;
}

vector<uint32_t> AbstractIndexBufferWrapper::GetIndexes()
{
	return Indexes;
}

AbstractIndexBufferWrapper::AbstractIndexBufferWrapper(LogicDeviceWrapper device, vector<uint32_t> indexes) :Device(device), Indexes(indexes)
{
}

AbstractIndexBufferWrapper::~AbstractIndexBufferWrapper()
{
}