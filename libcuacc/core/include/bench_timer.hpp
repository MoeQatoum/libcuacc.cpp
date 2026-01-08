#include <assert.h>
#include <chrono>
#include <functional>
#include <numeric>

using time_point = typename std::chrono::high_resolution_clock::time_point;

template<typename TimeUnit, std::size_t N, class Clock = std::chrono::high_resolution_clock>
class Timer {
    using time_point   = typename Clock::time_point;
    time_point m_start = Clock::now();
    time_point m_end   = {};

    std::chrono::duration<TimeUnit> duration_buff[N] = {};

    std::size_t n = 0;

  public:
    void tick() {
        if (N - 1 == n) {
            // raise 1;
            abort();
        }
        m_end   = time_point{};
        m_start = Clock::now();
    }

    void tock() {
        m_end            = Clock::now();
        duration_buff[n] = m_start - m_end;
    }

    std::chrono::duration<TimeUnit> duration_last() const {
        assert(m_end != time_point{});
        return std::chrono::duration_cast<TimeUnit>(duration_buff[n]);
    }

    std::chrono::duration<TimeUnit> duration_at(int at) {
        return std::chrono::duration<TimeUnit>(duration_buff[at]);
    }

    std::chrono::duration<TimeUnit> duration_max() const {
        time_point max = duration_buff[0];
        for (int i = 0; i < N; ++i) {
            if (max < duration_buff[i]) max = duration_buff[i];
        }
        return max;
    }

    std::chrono::duration<TimeUnit> duration_min() const {
        time_point min = duration_buff[0];
        for (int i = 0; i < N; ++i) {
            if (min > duration_buff[i]) min = duration_buff[i];
        }
        return min;
    }

    std::chrono::duration<TimeUnit> duration_mean() const {
        time_point acc = std::accumulate(duration_buff[0], duration_buff[N - 1]);
        return acc / N;
    }
};
