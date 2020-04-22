#include "BlurBmp.h"

BlurBmp::BlurBmp(const std::string input, const std::string output, const int threadCount, const int coreCount, const int blurRadius)
	: m_input(input)
	, m_output(output)
	, m_threadCount(threadCount)
	, m_coreCount(coreCount)
	, m_blurRadius(blurRadius)
{
}

void Blur(ThreadData& thread)
{
	int blurRadius = thread.blurRadius;

	for (int height = thread.indexStartHeight; height < thread.indexFunishHeight; ++height)
	{
		for (int width = thread.indexStartWidth; width < thread.indexFunisWidth; ++width)
		{
			std::vector<rgb_t> pixels;

			for (int i = height - blurRadius; i <= height + blurRadius; ++i)
			{
				for (int j = width - blurRadius; j <= width + blurRadius; ++j)
				{
					int minHeight = std::min(thread.indexFunishHeight - 1, std::max(i, 0));
					int minWidth = std::min(thread.indexFunisWidth - 1, std::max(j, 0));

					rgb_t pixel = thread.inputImage.get_pixel(minWidth, minHeight);
					pixels.push_back(pixel);
				}
			}

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
		int funishHeight = (threadNumber + 1) * partHeight;
		thread->indexStartHeight = threadNumber * partHeight;
		thread->indexFunishHeight = funishHeight + (threadNumber == threadCount - 1 ? height - funishHeight : 0);

		int funishWidth = (i + 1) * partWidth;
		thread->indexStartWidth = i * partWidth;
		thread->indexFunisWidth = funishWidth + (i == threadCount - 1 ? width - funishWidth : 0);

		Blur(*thread);
	}
	ExitThread(0);
}

std::vector<ThreadData> BlurBmp::GetThreadsData(const bitmap_image& inputImage, bitmap_image* outputImage) const
{
	std::vector<ThreadData> threadsData;

	for (int i = 0; i < m_threadCount; ++i)
	{
		ThreadData threadData;
		threadData.inputImage = inputImage;
		threadData.outputImage = outputImage;
		threadData.blurRadius = m_blurRadius;
		threadData.threadCount = m_threadCount;
		threadData.threadNumber = i;

		threadsData.push_back(threadData);
	}

	return threadsData;
}

void BlurBmp::Run()
{
	bitmap_image inputImage(m_input);

	if (!inputImage)
	{
		return;
	}

	bitmap_image outputImage(inputImage);

	std::vector<ThreadData> threadsData = GetThreadsData(inputImage, &outputImage);

	std::vector<HANDLE> handles(threadsData.size());
	int affinityMask = (1 << m_coreCount) - 1;
	for (size_t i = 0; i < threadsData.size(); ++i)
	{
		handles[i] = CreateThread(NULL, 0, &ThreadProc, &threadsData[i], CREATE_SUSPENDED, NULL);
		SetThreadAffinityMask(handles[i], affinityMask);
	}

	for (const auto& handle : handles)
	{
		ResumeThread(handle);
	}

	WaitForMultipleObjects((DWORD)handles.size(), handles.data(), true, INFINITE);
	outputImage.save_image(m_output);
}
