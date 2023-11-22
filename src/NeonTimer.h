#pragma once

#include <chrono>

class NeonTimer
{
public:
	NeonTimer();
	float Peek() const;
	float Mark();
private:
	std::chrono::steady_clock::time_point last;
};