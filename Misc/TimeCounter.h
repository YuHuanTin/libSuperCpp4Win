#ifndef LIBSUPERCPP4WIN_TIMECOUNTER_H
#define LIBSUPERCPP4WIN_TIMECOUNTER_H

#include <chrono>

class TimeCounter {
public:
    TimeCounter() {
        timePoint = std::chrono::steady_clock::now();
    }
    void reset() {
        timePoint = std::chrono::steady_clock::now();
    }
    template<typename TimeDuration>
    long long getDiff() {
        auto diff = std::chrono::duration_cast<TimeDuration>(std::chrono::steady_clock::now() - timePoint).count();
        timePoint = std::chrono::steady_clock::now();
        return diff;
    }
private:
    std::chrono::time_point<std::chrono::steady_clock> timePoint;
};


#endif //LIBSUPERCPP4WIN_TIMECOUNTER_H
