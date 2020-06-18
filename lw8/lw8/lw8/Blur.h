#pragma once
#define NOMINMAX
#include "ITask.h"
#include "bitmap_image.hpp"
#include <ctime>

struct ThreadData
{
	bitmap_image inputImage;
	bitmap_image* outputImage;
	std::ofstream* logFile;
	std::clock_t startTime;
	int indexStartWidth;
	int indexFinishWidth;
	int indexStartHeight;
	int indexFinishHeight;
	int blurRadius;
	int threadCount;
	int threadNumber;
};

class Blur : public ITask
{
public:
	Blur(ThreadData& thread);

	void Execute() override;

private:
	double GetTimeDifference(const std::clock_t& start);
	std::vector<rgb_t> GetPixels(const int height, const int width, const ThreadData& thread);

	ThreadData& m_thread;
};
