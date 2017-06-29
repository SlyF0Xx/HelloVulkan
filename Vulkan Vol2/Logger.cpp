#include "Logger.h"


Logger::Logger()
{
	flog.open("log.txt");
}

void Logger::checkResults(VkResult result)
{
	switch (result)
	{
	case VK_SUCCESS:
	{
		break;
	}
	default:
		LogMessage(result);
		LogMessage(vk::to_string((vk::Result) result));
		LogMessage("Wtf");
		abort();
		break;
	}
}


Logger::~Logger()
{
}
