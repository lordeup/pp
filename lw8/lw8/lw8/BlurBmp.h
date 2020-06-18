#pragma once
#define NOMINMAX
#include "Blur.h"
#include <Windows.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

class BlurBmp
{
public:
	BlurBmp(const std::string input, const std::string output, const int threadCount, const int coreCount, const int blurRadius, const std::vector<int>& threadPriorities);

	void Run();

private:
	std::vector<ThreadData> GetThreadsData(const bitmap_image& inputImage, bitmap_image* outputImage, std::clock_t& start) const;

	std::string m_input;
	std::string m_output;
	int m_threadCount;
	int m_coreCount;
	int m_blurRadius;
	std::vector<int> m_threadPriorities;
};