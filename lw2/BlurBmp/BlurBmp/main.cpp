#include "BlurBmp.h"
#include <ctime>
#include <iostream>

int main(int argc, char* argv[])
{
	std::clock_t start = std::clock();
	if (argc != 6)
	{
		std::cerr << "The number of arguments does not match the task condition\n"
					 "Input should look: BlurBmp.exe <input image> <output image> <number threads> <number core> <blur radius>\n";
		return 1;
	}

	std::string inputImage;
	std::string outputImage;
	int threadCount;
	int coreCount;
	int blurRadius;

	try
	{
		inputImage = argv[1];
		outputImage = argv[2];
		threadCount = std::stoi(argv[3]);
		coreCount = std::stoi(argv[4]);
		blurRadius = std::stoi(argv[5]);
	}
	catch (const std::invalid_argument)
	{
		return 1;
	}

	BlurBmp blurBmp(inputImage, outputImage, threadCount, coreCount, blurRadius);
	blurBmp.Run();

	std::clock_t end = std::clock();
	std::cout << "Runtime: " << std::difftime(end, start) << " ms" << std::endl;

	return 0;
}
