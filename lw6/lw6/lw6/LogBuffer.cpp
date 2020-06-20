#include "LogBuffer.h"

LogBuffer::LogBuffer(LogFileWriter* logFileWriter)
	: m_criticalSection(CRITICAL_SECTION())
	, m_logFileWriter(logFileWriter)
{
	if (!InitializeCriticalSectionAndSpinCount(&m_criticalSection, 0x00000400))
	{
		throw std::exception("Error. Critical section not created");
	}

	m_handleEvent = CreateEvent(nullptr, true, false, L"Event");

	if (!m_handleEvent)
	{
		throw std::exception("Error. Event not created");
	}

	InitializationHandle();
}

LogBuffer::~LogBuffer()
{
	CloseHandle(m_handle);
	CloseHandle(m_handleEvent);

	if (&m_criticalSection)
	{
		DeleteCriticalSection(&m_criticalSection);
	}
}

void LogBuffer::InitializationHandle()
{
	m_handle = CreateThread(nullptr, 0, &LogSizeMonitoringThread, (void*)this, 0, nullptr);
}

void LogBuffer::LogData(const std::string data)
{
	if (&m_criticalSection)
	{
		EnterCriticalSection(&m_criticalSection);
	}

	if (m_linkedList.GetSize() >= MAX_SIZE)
	{
		SetEvent(m_handleEvent);

		if (WaitForSingleObject(m_handle, INFINITE) == WAIT_OBJECT_0)
		{
			ResetEvent(m_handleEvent);
			InitializationHandle();
		}
	}

	m_linkedList.Push(data);

	if (&m_criticalSection)
	{
		LeaveCriticalSection(&m_criticalSection);
	}
}

DWORD WINAPI LogBuffer::LogSizeMonitoringThread(const LPVOID lpParam)
{
	LogBuffer* data = (LogBuffer*)lpParam;

	if (WaitForSingleObject(data->m_handleEvent, INFINITE) == WAIT_OBJECT_0)
	{
		size_t size = data->m_linkedList.GetSize();

		for (size_t i = 0; i < size; ++i)
		{
			data->m_logFileWriter->Log(data->m_linkedList.GetHeadData());
			data->m_linkedList.Pop();
		}
	}

	ExitThread(0);
}