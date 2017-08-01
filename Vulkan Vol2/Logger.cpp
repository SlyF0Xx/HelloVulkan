#include "Logger.h"


Logger::Logger()
{
#ifdef _DEBUG
	flog.open("log.txt");
#endif // _DEBUG

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
