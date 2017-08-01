#pragma once
#include "AbstractIndexBufferWrapper.h"

class PrimitiveIndexBuffer :
	public AbstractIndexBufferWrapper
{
public:
	void WriteIndexes();
	PrimitiveIndexBuffer(LogicDeviceWrapper device, vector<uint32_t> indexes);
	~PrimitiveIndexBuffer();

	PrimitiveIndexBuffer() {};
};