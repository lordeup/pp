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

OperationMode Handler::ParseOperationMode(const std::string str)
{
	int operatingMode = ParseNumber(str);

	if (operatingMode == 0)
	{
		return OperationMode::USUAL;
	}
	else if (operatingMode == 1)
	{
		return OperationMode::POOL;
	}
	else
	{
		throw std::invalid_argument("Incorrect operation mode: " + str);
	}
}

void Handler::Run(int argc, char* argv[])
{
	if (strcmp(argv[1], "/") == 0)
	{
		std::cout << "Input should look:\n"
					 "lw8.exe <operating mode> <number blocks> <input path> <output path> <blur radius> <number threads>\n"
					 "Operating mode:\n"
					 "0 - usual\n1 - using pool\n";
		return;
	}

	OperationMode operatingMode = ParseOperationMode(argv[1]);

	if (argc != 7)
	{
		throw std::invalid_argument("The number of arguments does not match the task condition\n" + INFO_HINT);
	}

	int blocksCount = ParseNumber(argv[2]);

	std::string inputPath = argv[3];
	std::string outputPath = argv[4];

	int blurRadius = ParseNumber(argv[5]);
	int threadCount = ParseNumber(argv[6]);

	if (!std::filesystem::exists(inputPath))
	{
		throw std::exception("This directory does not exist");
	}

	if (!std::filesystem::exists(outputPath))
	{
		std::filesystem::create_directories(outputPath);
	}

	std::vector<std::string> inputImages;
	std::vector<std::string> outputImages;

	for (const auto& file : std::filesystem::directory_iterator(inputPath))
	{
		auto path = file.path();

		if (path.extension() == ".bmp")
		{
			inputImages.push_back(path.string());
			outputImages.push_back(outputPath + "/" + path.stem().string() + "_blur.bmp");
		}
	}

	BlurBmp blurBmp(operatingMode, inputImages, outputImages, blurRadius, blocksCount, threadCount);
	blurBmp.Run();
}
