// test 1000 function calls in 1 second
#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <array>

using timer = std::chrono::high_resolution_clock;

struct Call
{
    using f = void();
    int ms;
    f* func;
    Call(f* function_to_call, int every_milliseconds = 1) : func(function_to_call), ms(every_milliseconds) { }
};

template< typename T >
struct array_nano_duration
{
    using clock = std::chrono::high_resolution_clock;
    int index;
    std::array< T, 10 > data;
    array_nano_duration() : index(0)
    {
        for (int key = 0; key < data.size(); ++key) {
            auto start = clock::now();
            std::this_thread::sleep_for(std::chrono::nanoseconds(nano_progression(key)));
            data[key] = std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - start).count();
        }
    }

    T nano_progression(int key)
    {
        static constexpr T values[] = {
            0, 1, 5, 10, 50, 100, 200, 500, 750, 1'000, 5'000, 7'500, 10'000, 15'000, 20'000, 50'000, 100'000, 250'000, 500'000, 750'000
        };
        return values[key];
    }

    void cout()
    {
        int key = 0;
        std::cout << "Data nano duration key: value{" << std::endl;
        for (auto v : data) {
            std::cout << key++ << ": " << v << std::endl;
        }
        std::cout << "}" << std::endl;
    }

    T sleep_for(T initial_sleep)
    {
        T best = 0;
        auto best_diff = std::abs(data[0] - initial_sleep);
        decltype(best_diff) diff;
        for (auto key = 1; key < data.size(); ++key) {
            if (initial_sleep - data[key] < 0) continue;
            diff = initial_sleep - data[key];
            if (diff > best_diff) continue;
            best_diff = diff;
            best = key;
        }
        index = best;
        return nano_progression(best);
    }
    void update_index(long long value)
    {
        data[index] = value;
    }
};

void call()
{
    //std::this_thread::sleep_for(std::chrono::nanoseconds(33'000'000));
    //std::cout << " ";
    //std::cout << "call" << std::endl;
}

void test(Call::f* func = call)
{
    using clock = std::chrono::high_resolution_clock;
    // global while to check 1 second duration is correct, in <5% error [975-1025]ms is correct
    while (true) {
        auto global_start = clock::now();
        auto global_duration = 0LL;
        auto global_func_duration = 0LL;
        auto ticks = 0;
        auto rounding_cycles = 0;
        // local while, run function 1000 times and try sleep when it too fast
        while (true) {
            if (ticks == 1000 || global_func_duration > 975'000'000) break;
            auto func_start = clock::now();
            func();
            auto func_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - func_start).count();
            global_func_duration += func_duration;
            ticks++;
            auto expected_duration = (ticks * 1'000'000) - (ticks * 25'000);
            while (expected_duration > global_func_duration) {
                auto sleep_start = clock::now();
                rounding_cycles++;
                std::this_thread::sleep_for(
                    std::chrono::nanoseconds(
                        // the simpler and best way for now but not good for some cases
                        expected_duration - global_func_duration
                    )
                );
                auto sleep_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - sleep_start).count();
                global_func_duration += sleep_duration;
            }
        }
        auto global_end = clock::now();
        global_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(global_end - global_start).count();
        std::cout << "global duration is: " << global_duration / 1'000'000 << " milliseconds." << std::endl;
        std::cout << "func duration is: " << global_func_duration / 1'000'000 << " milliseconds." << std::endl;
        std::cout << "ticks: " << ticks << std::endl;
        std::cout << "avg rounding cycles: " << rounding_cycles / 1000.0 << std::endl;
        if (global_duration < 975'000'000 || global_duration > 1'025'000'000) throw std::runtime_error("duration check failed");
        //break;
    }
}

struct MilliTimer
{
    using fptr = void();
    MilliTimer(fptr* function_to_call, int call_every_millisconds) : call(function_to_call), ms(call_every_millisconds) { }
    void start()
    {
        call_thread = std::thread(&MilliTimer::call_process, this);
    }
    void join()
    {
        if (call_thread.joinable()) call_thread.join();
    }
private:
    fptr* call;
    int ms;
    int fps;
    std::thread call_thread;
    void call_process()
    {
        using clock = std::chrono::high_resolution_clock;
        constexpr auto min_dynamic_tick_error = 1;
        constexpr auto max_dynamic_tick_error = 65536;
        auto dynamic_tick_error = min_dynamic_tick_error;
        auto max_ticks = 1000 / ms;
        constexpr auto ns_in_ms = 1'000'000;
        constexpr auto ns_in_sec = 1'000'000'000;
        // TODO: find bug/problem with max_dynamic_tick_error go only to max when call function is too fast
        // problem happens in condion: empty function to call and timer ms set to > 8
        while (true) {
            auto check_start = clock::now();
            auto overall_call_duration = 0LL;
            auto ticks = 0;
            while (true) {
                if ((ticks == max_ticks) || (overall_call_duration > ns_in_sec)) break;
                auto call_start = clock::now();
                call();
                ticks++;
                auto tick_call_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - call_start).count();
                overall_call_duration += tick_call_duration;
                auto expected_duration = (ticks * ms * ns_in_ms) - (ticks * ms * dynamic_tick_error);
                while (expected_duration > overall_call_duration) {
                    auto sleep_start = clock::now();
                    std::this_thread::sleep_for(std::chrono::nanoseconds(expected_duration - overall_call_duration));
                    auto sleep_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - sleep_start).count();
                    overall_call_duration += sleep_duration;
                }
            }
            auto check_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - check_start).count();
            std::cout << "check duration: " << check_duration / ns_in_ms << "ms" << std::endl;
            std::cout << "overall call duration: " << overall_call_duration / ns_in_ms << "ms" << std::endl;
            std::cout << "ticks: " << ticks << std::endl;
            std::cout << "dynamic tick error: " << dynamic_tick_error << "ns" << std::endl;
            auto diff = dynamic_tick_error * 1'000;
            if (check_duration < ns_in_sec - diff || check_duration > ns_in_sec + diff) {
                if (dynamic_tick_error < max_dynamic_tick_error) dynamic_tick_error *= 2;
            }
            else {
                if (dynamic_tick_error > min_dynamic_tick_error) dynamic_tick_error /= 2;
            }
        }
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    MilliTimer mt(call, 33);
    try {
        mt.start();
        for (int i = 0; i < 4; ++i) {
            std::cout << "that my call" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    mt.join();
    
    std::cin.get();
}