#pragma once

#include <chrono>

class NeonTimer
{
public:
	NeonTimer() noexcept;
	float Peek() const noexcept;
	float Mark() noexcept;
private:
	std::chrono::steady_clock::time_point last;
};