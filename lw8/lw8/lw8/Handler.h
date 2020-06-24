#pragma once
#include "BlurBmp.h"
#include "ThreadPool.h"
#include <filesystem>
#include <iostream>

const std::string INFO_HINT = "To learn how to use the program, enter: lw8.exe /";

class Handler
{
public:
	void Run(int argc, char* argv[]);

private:
	int ParseNumber(const std::string str);
	OperationMode ParseOperationMode(const std::string str);
};