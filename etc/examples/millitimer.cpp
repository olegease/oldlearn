#include "../mtimer.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <random>

std::random_device rd;
std::mt19937 eng(rd());
std::uniform_int_distribution<> ldistr(5, 33);
std::uniform_int_distribution<> hdistr(2000, 3000);
std::uniform_int_distribution<> idistr(0, 20);

void test()
{
    int next_call = (!idistr(eng)) ? hdistr(eng) : ldistr(eng);
    std::cout << "call - next after: " << next_call << "mls" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(next_call));
}

int main()
{
    etc::milli_timer timer(test, 1);
    timer.start();
    while (true) {
        timer.get_stats().display();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cin.get();
}