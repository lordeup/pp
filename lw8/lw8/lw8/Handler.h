#pragma once
#include "BlurBmp.h"
#include <iostream>

const int MIN_NUMBER_ARGUMENTS = 7;
const std::string INFO_HINT = "To learn how to use the program, enter: lw8.exe /";

enum OperationMode
{
	USUAL,
	POOL
};

class Handler
{
public:
	void Run(int argc, char* argv[]);

private:
	int ParseNumber(const std::string str);
};