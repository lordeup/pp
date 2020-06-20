#include "LogFileWriter.h"

LogFileWriter::LogFileWriter(const std::string fileName)
	: m_output(std::ofstream(fileName))
{
}

void LogFileWriter::Log(const std::string str)
{
	m_output << str;
}
