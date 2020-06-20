#include "BlurBmp.h"

BlurBmp::BlurBmp(const std::string input, const std::string output, const int threadCount, const int coreCount, const int blurRadius, const std::vector<int>& threadPriorities)
	: m_input(input)
	, m_output(output)
	, m_threadCount(threadCount)
	, m_coreCount(coreCount)
	, m_blurRadius(blurRadius)
	, m_threadPriorities(threadPriorities)
{
}

double GetTimeDifference(const std::clock_t& start)
{
	std::clock_t end = std::clock();
	return std::difftime(end, start);
}

std::vector<rgb_t> GetPixels(const int height, const int width, const ThreadData& thread)
{
	std::vector<rgb_t> pixels;

	int blurRadius = thread.blurRadius;

	for (int i = height - blurRadius; i <= height + blurRadius; ++i)
	{
		for (int j = width - blurRadius; j <= width + blurRadius; ++j)
		{
			int minHeight = std::min(thread.indexFinishHeight - 1, std::max(i, 0));
			int minWidth = std::min(thread.indexFinishWidth - 1, std::max(j, 0));

			rgb_t pixel = thread.inputImage.get_pixel(minWidth, minHeight);
			pixels.push_back(pixel);
		}
	}

	return pixels;
}

void Blur(ThreadData& thread)
{
	for (int height = thread.indexStartHeight; height < thread.indexFinishHeight; ++height)
	{
		for (int width = thread.indexStartWidth; width < thread.indexFinishWidth; ++width)
		{
			std::vector<rgb_t> pixels = GetPixels(height, width, thread);

			int totalRed = 0, totalGreen = 0, totalBlue = 0;

			for (const auto& pixel : pixels)
			{
				totalRed += pixel.red;
				totalGreen += pixel.green;
				totalBlue += pixel.blue;
			}

			rgb_t colour;
			colour.red = (unsigned char)(totalRed / pixels.size());
			colour.green = (unsigned char)(totalGreen / pixels.size());
			colour.blue = (unsigned char)(totalBlue / pixels.size());

			thread.outputImage->set_pixel(width, height, colour);

			std::string str = std::to_string(thread.threadNumber) + "\t" + std::to_string((int)GetTimeDifference(thread.startTime)) + "\n";

			*thread.logFile << str;
			thread.logBuffer->LogData(str);
		}
	}
}

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	ThreadData* thread = (ThreadData*)lpParam;

	int width = thread->inputImage.width();
	int height = thread->inputImage.height();
	int threadCount = thread->threadCount;
	int threadNumber = thread->threadNumber;

	int partWidth = width / threadCount;
	int partHeight = height / threadCount;

	for (int i = 0; i < threadCount; ++i)
	{
		int finishHeight = (threadNumber + 1) * partHeight;
		thread->indexStartHeight = threadNumber * partHeight;
		thread->indexFinishHeight = finishHeight + (threadNumber == threadCount - 1 ? height - finishHeight : 0);

		int finishWidth = (i + 1) * partWidth;
		thread->indexStartWidth = i * partWidth;
		thread->indexFinishWidth = finishWidth + (i == threadCount - 1 ? width - finishWidth : 0);

		Blur(*thread);
	}
	ExitThread(0);
}

std::vector<ThreadData> BlurBmp::GetThreadsData(const bitmap_image& inputImage, bitmap_image* outputImage, std::clock_t& start) const
{
	std::vector<ThreadData> threadsData;

	LogBuffer* logBuffer = new LogBuffer(new LogFileWriter("logFileWriter.txt"));

	for (int i = 0; i < m_threadCount; ++i)
	{
		ThreadData threadData;
		threadData.inputImage = inputImage;
		threadData.outputImage = outputImage;
		threadData.blurRadius = m_blurRadius;
		threadData.threadCount = m_threadCount;
		threadData.threadNumber = i;
		threadData.logBuffer = logBuffer;
		threadData.logFile = new std::ofstream("log" + std::to_string(i) + ".txt");
		threadData.startTime = start;

		threadsData.push_back(threadData);
	}

	return threadsData;
}

void BlurBmp::Run()
{
	std::clock_t start = std::clock();
	bitmap_image inputImage(m_input);

	if (!inputImage)
	{
		return;
	}

	bitmap_image outputImage(inputImage);

	std::vector<ThreadData> threadsData = GetThreadsData(inputImage, &outputImage, start);

	std::vector<HANDLE> handles(threadsData.size());
	int affinityMask = (1 << m_coreCount) - 1;
	for (size_t i = 0; i < threadsData.size(); ++i)
	{
		handles[i] = CreateThread(nullptr, 0, &ThreadProc, &threadsData[i], CREATE_SUSPENDED, nullptr);
		SetThreadAffinityMask(handles[i], affinityMask);
		SetThreadPriority(handles[i], m_threadPriorities[i]);
	}

	for (const auto& handle : handles)
	{
		ResumeThread(handle);
	}

	WaitForMultipleObjects((DWORD)handles.size(), handles.data(), true, INFINITE);
	outputImage.save_image(m_output);

	std::cout << "Runtime: " << GetTimeDifference(start) << " ms" << std::endl;
}
