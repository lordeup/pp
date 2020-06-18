#pragma once
#include "LinkedList.h"
#include "LogFileWriter.h"
#include <Windows.h>

class LogBuffer
{
public:
	LogBuffer(LogFileWriter* logFileWriter);
	~LogBuffer();
	void LogData(const double data);

private:
	const size_t MAX_SIZE = 256;

	CRITICAL_SECTION* m_criticalSection;

	LogFileWriter* m_logFileWriter;
	LinkedList<double> m_linkedList;

	static DWORD WINAPI LogSizeMonitoringThread(CONST LPVOID lpParam);
};
