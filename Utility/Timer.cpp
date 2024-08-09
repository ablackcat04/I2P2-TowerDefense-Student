#include "Timer.hpp"

void Timer::Proceed(float delta_time) {
    time += delta_time;
}

bool Timer::ReachLimit() {
    return time >= limit;
}

void Timer::Reset() {
    time = 0.f;
}

void Timer::Initialize(float limit_time) {
    Reset();
    limit = limit_time;
}