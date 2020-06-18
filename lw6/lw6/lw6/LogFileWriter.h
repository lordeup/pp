#pragma once
#include <fstream>
#include <string>

class LogFileWriter
{
public:
	LogFileWriter(const std::string fileName);
	
	void LogFile();

private:
	std::ofstream m_output;
};