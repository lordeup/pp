#include "LogFileWriter.h"

LogFileWriter::LogFileWriter(const std::string fileName)
	: m_output(std::ofstream(fileName))
{
}
