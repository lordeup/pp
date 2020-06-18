#include "LogBuffer.h"

LogBuffer::LogBuffer(LogFileWriter* logFileWriter)
	: m_criticalSection(new CRITICAL_SECTION)
	, m_logFileWriter(logFileWriter)
{
	if (!InitializeCriticalSectionAndSpinCount(m_criticalSection, 0x00000400))
	{
		throw std::exception("Error: InitializeCriticalSectionAndSpinCount");
	}
}

LogBuffer::~LogBuffer()
{
	if (m_criticalSection)
	{
		DeleteCriticalSection(m_criticalSection);
	}
}

void LogBuffer::LogData(const double data)
{
	if (m_criticalSection)
	{
		EnterCriticalSection(m_criticalSection);
	}

	if (m_linkedList.GetSize() >= MAX_SIZE)
	{
	}

	m_linkedList.Push(data);

	if (m_criticalSection)
	{
		LeaveCriticalSection(m_criticalSection);
	}
}

DWORD WINAPI LogBuffer::LogSizeMonitoringThread(const LPVOID lpParam)
{
	LogBuffer* data = (LogBuffer*)lpParam;

	ExitThread(0);
}
