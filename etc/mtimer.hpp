#pragma once

#include <thread>
#include <chrono>
#include <iostream>

namespace etc {
    struct milli_timer;
    struct timer_stats
    {
        using clock = std::chrono::high_resolution_clock;
        using clock_type = decltype(clock::now() - clock::now());
        friend class milli_timer;
        void display()
        {
            std::cout << "timer (mtimer.hpp milli_timer) stats:" << std::endl;
            std::cout << "\tcheck duration: " << check_duration / 1'000'000 << "mls" << std::endl;
            std::cout << "\toverall call duration: " << overall_call_duration / 1'000'000 << "mls" << std::endl;
            std::cout << "\tticks: " << ticks << std::endl;
            std::cout << "\ttotal diff: " << total_diff / 1'000'000 << "mls" << std::endl;

        }
    private:
        std::size_t check_duration;
        std::size_t overall_call_duration;
        unsigned int ticks;
        double fps;
        long long total_diff;
    };

    struct milli_timer
    {
        using function_type = void();
        milli_timer(function_type* function_to_call, int call_every_milliseconds) : call(function_to_call), mls(call_every_milliseconds) { }
        void start()
        {
            call_thread = std::thread(&milli_timer::call_handler, this);
            call_thread.detach();
        }
        timer_stats& get_stats() { return stats; }
        //void stop() { }
    private:
        function_type* call;
        int mls;
        std::thread call_thread;
        //bool stop_flag = false;
        timer_stats stats;
        void call_handler()
        {
            using clock = std::chrono::high_resolution_clock;
            constexpr auto max_possible_ticks = 1'000ULL;
            constexpr auto nns_in_mcs = 1'000ULL;
            constexpr auto nns_in_mls = 1'000'000ULL;
            constexpr auto nns_in_sec = 1'000'000'000ULL;
            auto total_diff = 0LL;
            int max_ticks = mls ? max_possible_ticks / mls : -1;
            // function must be called at least one time (need in cases with high ms is set)
            if (!max_ticks) max_ticks = 1;
            while (true) {
                auto check_start = clock::now();
                auto overall_call_duration = 0ULL;
                auto ticks = 0;
                while (true) {
                    if (ticks == max_ticks || overall_call_duration > nns_in_sec - total_diff) break;
                    auto call_start = clock::now();
                    call();
                    ticks++;
                    overall_call_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - call_start).count();
                    unsigned long long expected_duration = (ticks * mls * nns_in_mls) - total_diff / (max_ticks - ticks + 1);
                    while (expected_duration > overall_call_duration) {
                        auto sleep_start = clock::now();
                        std::this_thread::sleep_for(std::chrono::nanoseconds(expected_duration - overall_call_duration));
                        overall_call_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - sleep_start).count();
                    }
                }
                auto check_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - check_start).count();
                if (mls > 1'000) total_diff += check_duration - mls * nns_in_mls;
                else total_diff += check_duration - nns_in_sec;
                if (total_diff > nns_in_sec) total_diff = 0;
                stats.check_duration = check_duration;
                stats.overall_call_duration = overall_call_duration;
                stats.ticks = ticks;
                stats.total_diff = total_diff;
            }
        }
    };
}