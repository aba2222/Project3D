#pragma once
#include <chrono>

class ProTimer {
public:
	ProTimer();
	float Mark();
	float Peek() const;

private:
	std::chrono::steady_clock::time_point last;
};

