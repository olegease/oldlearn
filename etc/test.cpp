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
    std::array< T, 2000 > data;
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
        return 500 * key;
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
    std::this_thread::sleep_for(std::chrono::nanoseconds(5));
    //std::cout << " ";
}

void test(Call::f* func = call)
{
    using clock = std::chrono::high_resolution_clock;
    array_nano_duration< long long > sleep_helper;
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
                // TODO find best way to sleep, now it's too heavy
                auto sleep_start = clock::now();
                rounding_cycles++;
                std::this_thread::sleep_for(
                    std::chrono::nanoseconds(
                        sleep_helper.sleep_for(1'000'000 - func_duration)
                    )
                );
                auto sleep_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - sleep_start).count();
                global_func_duration += sleep_duration;
                sleep_helper.update_index(sleep_duration);
            }
        }
        auto global_end = clock::now();
        global_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(global_end - global_start).count();
        std::cout << "global duration is: " << global_duration / 1'000'000 << " milliseconds." << std::endl;
        std::cout << "func duration is: " << global_func_duration / 1'000'000 << " milliseconds." << std::endl;
        std::cout << "ticks: " << ticks << std::endl;
        std::cout << "avg rounding cycles: " << rounding_cycles / 1000 << std::endl;
        if (global_duration < 975'000'000 || global_duration > 1'025'000'000) throw std::runtime_error("duration check failed");
        //break;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    try {
        test();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cin.get();
}