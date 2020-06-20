#pragma once
#include <fstream>
#include <string>

class LogFileWriter
{
public:
	LogFileWriter(const std::string fileName);

	void Log(const std::string str);

private:
	std::ofstream m_output;
};