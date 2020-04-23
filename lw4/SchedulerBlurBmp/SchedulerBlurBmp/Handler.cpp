#include "Handler.h"

int Handler::ParseNumber(const std::string str)
{
	try
	{
		return std::stoi(str);
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("Invalid argument: " + str);
	}
}

void Handler::Run(int argc, char* argv[])
{
	if (strcmp(argv[1], "/") == 0)
	{
		std::cout << "Input should look:\n"
					 "SchedulerBlurBmp.exe <input image> <output image> <number threads> <number core> <blur radius> <threads priorities>\n"
					 "Options priority:\n"
					 "-1 - below normal\n 0 - normal\n 1 - above normal\n";
		return;
	}

	if (argc < MIN_NUMBER_ARGUMENTS)
	{
		throw std::invalid_argument("The number of arguments does not match the task condition\n" + INFO_HINT);
	}

	std::string inputImage = argv[1];
	std::string outputImage = argv[2];

	int threadCount = ParseNumber(argv[3]);
	int coreCount = ParseNumber(argv[4]);
	int blurRadius = ParseNumber(argv[5]);

	if (argc != MIN_NUMBER_ARGUMENTS + threadCount)
	{
		throw std::invalid_argument("The number of priorities should be the same as the number of threads\n" + INFO_HINT);
	}

	std::vector<int> threadPriorities(threadCount);

	for (size_t i = 0; i < threadPriorities.size(); ++i)
	{
		threadPriorities[i] = ParseNumber(argv[MIN_NUMBER_ARGUMENTS + i]);
	}

	BlurBmp blurBmp(inputImage, outputImage, threadCount, coreCount, blurRadius, threadPriorities);
	blurBmp.Run();
}
