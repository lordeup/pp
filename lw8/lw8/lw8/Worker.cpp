#include "Worker.h"

Worker::Worker()
{
	m_handle = new HANDLE;
}

Worker::~Worker()
{
	WaitForSingleObject(m_handle, INFINITE);
}

bool Worker::ExecuteTask(ITask& task)
{
	if (IsBusy())
	{
		return false;
	}

	m_handle = CreateThread(NULL, 0, &ThreadProc, &task, CREATE_SUSPENDED, NULL);
	ResumeThread(m_handle);

	return true;
}

bool Worker::IsBusy() const
{
	LPDWORD code = new DWORD;

	if (GetExitCodeThread(m_handle, code))
	{
		return true;
	}

	return false;
}
