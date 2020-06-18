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
					 "lw8.exe <operating mode> <input image> <output image> <number threads> <number core> <blur radius> <threads priorities>\n"
					 "Operating mode:\n"
					 "0 - usual\n1 - using pool\n"
					 "Options priority:\n"
					 "-1 - below normal\n 0 - normal\n 1 - above normal\n";
		return;
	}

	if (argc < MIN_NUMBER_ARGUMENTS)
	{
		throw std::invalid_argument("The number of arguments does not match the task condition\n" + INFO_HINT);
	}

	int operatingMode = ParseNumber(argv[1]);
	std::string inputImage = argv[2];
	std::string outputImage = argv[3];

	int threadCount = ParseNumber(argv[4]);
	int coreCount = ParseNumber(argv[5]);
	int blurRadius = ParseNumber(argv[6]);

	if (argc != MIN_NUMBER_ARGUMENTS + threadCount)
	{
		throw std::invalid_argument("The number of priorities should be the same as the number of threads\n" + INFO_HINT);
	}

	std::vector<std::string> inputImages;
	std::vector<std::string> outputImages;

	std::vector<int> threadPriorities(threadCount);

	for (size_t i = 0; i < threadPriorities.size(); ++i)
	{
		threadPriorities[i] = ParseNumber(argv[MIN_NUMBER_ARGUMENTS + i]);
	}

	if (operatingMode == OperationMode::USUAL)
	{
		for (size_t i = 0; i < inputImages.size(); ++i)
		{
			BlurBmp blurBmp(inputImage, outputImage, threadCount, coreCount, blurRadius, threadPriorities);
			blurBmp.Run();
		}
	}
	else if (operatingMode == OperationMode::POOL)
	{
		int f = 0;
	}
	else
	{
		throw std::invalid_argument("Incorrect operation mode");
	}
}
