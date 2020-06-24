#include "Blur.h"

Blur::Blur(ThreadData& thread)
	: m_thread(thread)
{
}

void Blur::Execute()
{
	for (int height = m_thread.indexStartHeight; height < m_thread.indexFinishHeight; ++height)
	{
		for (int width = m_thread.indexStartWidth; width < m_thread.indexFinishWidth; ++width)
		{
			std::vector<rgb_t> pixels = GetPixels(height, width, m_thread);

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

			m_thread.outputImage->set_pixel(width, height, colour);
		}
	}
}

std::vector<rgb_t> Blur::GetPixels(const int height, const int width, const ThreadData& thread)
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
