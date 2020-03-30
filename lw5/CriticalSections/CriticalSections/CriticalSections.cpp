#include <Windows.h>
#include <iostream>

CRITICAL_SECTION CriticalSection;

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	EnterCriticalSection(&CriticalSection);

	int* workingVariable = (int*)lpParam;

	for (int i = 0; i < 100000; ++i)
	{
		int j = *workingVariable;
		*workingVariable = j + 1;
	}

	LeaveCriticalSection(&CriticalSection);

	ExitThread(0);
}

int main(int argc, char* argv[])
{
	int threadCount = 2;
	int coreCount = 2;
	int workingVariable = 0;

	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400))
	{
		return 1;
	}

	HANDLE* handles = new HANDLE[threadCount];

	for (int i = 0; i < threadCount; ++i)
	{
		handles[i] = CreateThread(NULL, 0, &ThreadProc, &workingVariable, CREATE_SUSPENDED, NULL);
		SetThreadAffinityMask(handles[i], coreCount);
	}

	for (int i = 0; i < threadCount; ++i)
	{
		ResumeThread(handles[i]);
	}

	WaitForMultipleObjects(threadCount, handles, true, INFINITE);

	std::cout << "WorkingVariable: " << workingVariable << std::endl;

	DeleteCriticalSection(&CriticalSection);

	return 0;
}