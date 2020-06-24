#include "BlurBmp.h"

BlurBmp::BlurBmp(const OperationMode operationMode, const std::vector<std::string>& inputImages, const std::vector<std::string>& outputImages, const int blurRadius, const int blocksCount, const int threadCount)
	: m_operationMode(operationMode)
	, m_inputImages(inputImages)
	, m_outputImages(outputImages)
	, m_blurRadius(blurRadius)
	, m_blocksCount(blocksCount)
	, m_threadCount(threadCount)
{
}

std::vector<ThreadData> BlurBmp::GetThreadsData(const bitmap_image& inputImage, bitmap_image* outputImage) const
{
	std::vector<ThreadData> threadsData;

	int width = inputImage.width();
	int height = inputImage.height();

	int partWidth = width / m_blocksCount;
	int partHeight = height / m_blocksCount;

	for (int i = 0; i < m_blocksCount; ++i)
	{
		ThreadData threadData;
		threadData.inputImage = inputImage;
		threadData.outputImage = outputImage;
		threadData.blurRadius = m_blurRadius;
		threadData.blocksCount = m_blocksCount;
		threadData.threadNumber = i;

		for (int j = 0; j < m_blocksCount; ++j)
		{
			int funishHeight = (i + 1) * partHeight;
			threadData.indexStartHeight = i * partHeight;
			threadData.indexFinishHeight = funishHeight + (i == m_blocksCount - 1 ? height - funishHeight : 0);

			int funishWidth = (j + 1) * partWidth;
			threadData.indexStartWidth = j * partWidth;
			threadData.indexFinishWidth = funishWidth + (j == m_blocksCount - 1 ? width - funishWidth : 0);

			threadsData.push_back(threadData);
		}
	}

	return threadsData;
}

void BlurBmp::Run()
{
	std::clock_t start = std::clock();

	for (size_t i = 0; i < m_inputImages.size(); ++i)
	{
		bitmap_image inputImage(m_inputImages[i]);

		if (!inputImage)
		{
			return;
		}

		bitmap_image outputImage(inputImage);

		std::vector<ThreadData> threadsData = GetThreadsData(inputImage, &outputImage);

		std::vector<ITask*> tasks;

		for (auto& threadData : threadsData)
		{
			tasks.push_back(new Blur(threadData));
		}

		if (m_operationMode == OperationMode::USUAL)
		{
			std::vector<HANDLE> handles(tasks.size());
			for (size_t i = 0; i < tasks.size(); ++i)
			{
				handles[i] = CreateThread(nullptr, 0, &ThreadProc, tasks[i], CREATE_SUSPENDED, nullptr);
			}

			for (const auto& handle : handles)
			{
				ResumeThread(handle);
			}

			WaitForMultipleObjects((DWORD)handles.size(), handles.data(), true, INFINITE);
		}
		else if (m_operationMode == OperationMode::POOL)
		{
			ThreadPool threadPool(m_threadCount, tasks);
			threadPool.Execute();
		}

		outputImage.save_image(m_outputImages[i]);
	}

	std::cout << "Runtime: " << GetTimeDifference(start) << " ms" << std::endl;
}
