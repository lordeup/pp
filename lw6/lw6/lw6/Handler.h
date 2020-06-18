#pragma once
#include "BlurBmp.h"
#include <iostream>

const int MIN_NUMBER_ARGUMENTS = 6;
const std::string INFO_HINT = "To learn how to use the program, enter: SchedulerBlurBmp.exe /";

class Handler
{
public:
	void Run(int argc, char* argv[]);

private:
	int ParseNumber(const std::string str);
};