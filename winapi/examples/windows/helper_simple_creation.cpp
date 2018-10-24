#include "../../helpers/windows/create.hpp"

#include <iostream>

namespace hlpwin = helpers::windows;

void f()
{
    MSG event = hlpwin::run::message;
    switch (event.message) {
    case WM_CREATE:
        std::cout << "created" << std::endl;
        break;
    case WM_DESTROY:
        std::cout << "destroyed" << std::endl;
        PostQuitMessage(0);
        break;
    }
}

int main()
{
    hlpwin::run::assign(f);
    HWND id = hlpwin::create();
    hlpwin::run{}();
    std::cin.get();
}
