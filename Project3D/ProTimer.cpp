#include "ProTimer.h"
using namespace std::chrono;

ProTimer::ProTimer() {
	last = steady_clock::now();
}

float ProTimer::Mark() {
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float ProTimer::Peek() const {
	return duration<float>(steady_clock::now() - last).count();
}
