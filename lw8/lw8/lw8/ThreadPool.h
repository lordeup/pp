#pragma once
#include "ITask.h"
#include <vector>

class ThreadPool
{
public:
	ThreadPool(const int threadCount, const std::vector<ITask*>& tasks);

	void Execute();

private:
	void InitializationHandles();

	std::vector<HANDLE> m_handles;

	std::vector<ITask*> m_tasks;
	int m_threadCount;
};
