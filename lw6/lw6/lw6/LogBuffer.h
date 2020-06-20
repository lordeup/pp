#pragma once
#include "LinkedList.h"
#include "LogFileWriter.h"
#include <Windows.h>

class LogBuffer
{
public:
	LogBuffer(LogFileWriter* logFileWriter);
	~LogBuffer();
	void LogData(const std::string data);

private:
	static DWORD WINAPI LogSizeMonitoringThread(CONST LPVOID lpParam);
	void InitializationHandle();

	const size_t MAX_SIZE = 300;

	CRITICAL_SECTION m_criticalSection;
	LogFileWriter* m_logFileWriter;
	LinkedList<std::string> m_linkedList;
	HANDLE m_handle;
	HANDLE m_handleEvent;
};
