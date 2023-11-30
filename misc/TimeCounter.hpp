

#ifndef LIBSUPERCPP4WIN_TIMECOUNTER_HPP
#define LIBSUPERCPP4WIN_TIMECOUNTER_HPP

#include <chrono>


template<typename ClockType = std::chrono::steady_clock>
class TimeCounter {
public:
    TimeCounter() noexcept {
        timePoint = ClockType::now();
    }

    void reset() noexcept {
        timePoint = ClockType::now();
    }

    template<typename TimeDuration = std::chrono::milliseconds>
    constexpr size_t getDiff() noexcept {
        auto diff = std::chrono::duration_cast<TimeDuration>(std::chrono::steady_clock::now() - timePoint).count();
        timePoint = std::chrono::steady_clock::now();
        return diff;
    }

private:
    decltype(ClockType::now()) timePoint;
};


#endif //LIBSUPERCPP4WIN_TIMECOUNTER_HPP
