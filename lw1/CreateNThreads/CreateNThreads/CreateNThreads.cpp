#include <Windows.h>
#include <iostream>
#include <string>

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	std::cout << "Поток № " << *(int*)lpParam << std::endl;
	ExitThread(0);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	if (argc != 2)
	{
		std::cerr << "The number of arguments does not match the task condition\n"
					 "Input should look: CreateNThreads.exe <number threads>\n";
		return 1;
	}

	std::string line = argv[1];

	int thread;

	try
	{
		thread = std::stoi(line);
	}
	catch (const std::invalid_argument)
	{
		return 1;
	}

	if (thread <= 0)
	{
		std::cout << "The number of threads must be greater than 0" << std::endl;
		return 1;
	}

	HANDLE* handles = new HANDLE[thread];
	int* indexes = new int[thread];
	for (int i = 0; i < thread; ++i)
	{
		indexes[i] = i + 1;
		handles[i] = CreateThread(NULL, 0, &ThreadProc, &indexes[i], CREATE_SUSPENDED, NULL);
		ResumeThread(handles[i]);
	}

	WaitForMultipleObjects(thread, handles, true, INFINITE);

	return 0;
}
