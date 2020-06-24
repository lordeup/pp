#include "ThreadPool.h"

ThreadPool::ThreadPool(const int threadCount, const std::vector<ITask*>& tasks)
	: m_threadCount(threadCount)
	, m_tasks(tasks)
{
	InitializationHandles();
}

void ThreadPool::InitializationHandles()
{
	m_handles.resize(m_tasks.size());
	for (size_t i = 0; i < m_handles.size(); ++i)
	{
		m_handles[i] = CreateThread(nullptr, 0, &ThreadProc, m_tasks[i], CREATE_SUSPENDED, nullptr);
	}
}

void ThreadPool::Execute()
{
	for (size_t i = 0, j = 0; i < m_handles.size(); ++i, ++j)
	{
		ResumeThread(m_handles[i]);

		if (m_threadCount == j)
		{
			j = 0;
			WaitForMultipleObjects((DWORD)i + 1, m_handles.data(), true, INFINITE);
		}
	}

	WaitForMultipleObjects((DWORD)m_handles.size(), m_handles.data(), true, INFINITE);
}
