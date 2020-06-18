#pragma once
#include "IWorker.h"

class Worker : public IWorker
{
public:
	Worker();
	~Worker();

	bool ExecuteTask(ITask& task) override;
	bool IsBusy() const override;

private:
	HANDLE m_handle;
};