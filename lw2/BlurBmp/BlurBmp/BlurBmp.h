#pragma once
#define NOMINMAX
#include "bitmap_image.hpp"
#include <Windows.h>
#include <algorithm>
#include <string>
#include <vector>

struct ThreadData
{
	bitmap_image inputImage;
	bitmap_image* outputImage;
	int indexStartWidth;
	int indexFunisWidth;
	int indexStartHeight;
	int indexFunishHeight;
	int blurRadius;
	int threadCount;
	int threadNumber;
};

class BlurBmp
{
public:
	BlurBmp(const std::string input, const std::string output, const int threadCount, const int coreCount, const int blurRadius);

	void Run();

private:
	std::vector<ThreadData> GetThreadsData(const bitmap_image& inputImage, bitmap_image* outputImage) const;

	std::string m_input;
	std::string m_output;
	int m_threadCount;
	int m_coreCount;
	int m_blurRadius;
};