#include <Windows.h>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

std::clock_t start = std::clock();

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	int thread = *(int*)lpParam;
	std::cout << "Поток № " << thread << std::endl;
	std::string outputFileName = "t" + std::to_string(thread) + ".txt";
	std::ofstream fileOutput(outputFileName);

	for (int i = 0; i < 125; ++i)
	{
		std::clock_t end = std::clock();
		fileOutput << std::difftime(end, start) << std::endl;
		Sleep(10);
	}

	ExitThread(0);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	int threadCount = 2;

	HANDLE* handles = new HANDLE[threadCount];
	int* indexes = new int[threadCount];

	for (int i = 0; i < threadCount; ++i)
	{
		indexes[i] = i + 1;
		handles[i] = CreateThread(NULL, 0, &ThreadProc, (LPVOID)&indexes[i], CREATE_SUSPENDED, NULL);
		ResumeThread(handles[i]);
	}

	WaitForMultipleObjects(threadCount, handles, true, INFINITE);

	return 0;
}