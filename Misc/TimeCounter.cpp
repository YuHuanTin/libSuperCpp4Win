

#include "TimeCounter.h"

TimeCounter::TimeCounter() noexcept {
    timePoint = std::chrono::steady_clock::now();
}

void TimeCounter::reset() noexcept {
    timePoint = std::chrono::steady_clock::now();
}
