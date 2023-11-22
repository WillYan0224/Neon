#include "NeonTimer.h"

using namespace std::chrono;

NeonTimer::NeonTimer()
{
	last = steady_clock::now();
}

float NeonTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}

float NeonTimer::Mark() 
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}
