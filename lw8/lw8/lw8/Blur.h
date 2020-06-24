#pragma once
#define NOMINMAX
#include "ITask.h"
#include "Utils.h"
#include "bitmap_image.hpp"

struct ThreadData
{
	bitmap_image inputImage;
	bitmap_image* outputImage;
	int indexStartWidth;
	int indexFinishWidth;
	int indexStartHeight;
	int indexFinishHeight;
	int blurRadius;
	int blocksCount;
	int threadNumber;
};

class Blur : public ITask
{
public:
	Blur(ThreadData& thread);

	void Execute() override;

private:
	std::vector<rgb_t> GetPixels(const int height, const int width, const ThreadData& thread);

	ThreadData& m_thread;
};
