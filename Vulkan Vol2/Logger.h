#pragma once
#include <fstream>
#include <vulkan\vulkan.h>
#include <string>
#include <vulkan\vulkan.hpp>

#pragma comment (lib, "vulkan-1.lib")

using namespace std;


class Logger
{
private:
	ofstream flog;
public:
	Logger();
	void checkResults(VkResult result);

	template <class T>
	void LogMessage(T message);
	~Logger();
};

template<class T>
inline void Logger::LogMessage(T message)
{
#ifdef _DEBUG
	flog << message << endl;
#endif // _DEBUG
}
