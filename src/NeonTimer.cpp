#include "NeonTimer.h"

using namespace std::chrono;

NeonTimer::NeonTimer() noexcept
{
	last = steady_clock::now();
}

float NeonTimer::Peek() const noexcept
{
	return duration<float>(steady_clock::now() - last).count();
}

float NeonTimer::Mark() noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}
