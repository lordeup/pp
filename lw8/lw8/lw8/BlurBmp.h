#pragma once
#include "Blur.h"
#include "ThreadPool.h"
#include <algorithm>
#include <fstream>
#include <string>

enum class OperationMode
{
	USUAL,
	POOL,
};

class BlurBmp
{
public:
	BlurBmp(const OperationMode operationMode, const std::vector<std::string>& inputImages, const std::vector<std::string>& outputImages, const int blurRadius, const int blocksCount, const int threadCount);

	void Run();

private:
	std::vector<ThreadData> GetThreadsData(const bitmap_image& inputImage, bitmap_image* outputImage) const;

	OperationMode m_operationMode;
	std::vector<std::string> m_inputImages;
	std::vector<std::string> m_outputImages;
	int m_blurRadius;
	int m_blocksCount;
	int m_threadCount;
};