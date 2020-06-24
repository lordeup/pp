#include "Utils.h"

double GetTimeDifference(const std::clock_t& start)
{
	std::clock_t end = std::clock();
	return std::difftime(end, start);
}
