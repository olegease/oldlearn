#include "../../helpers/windows/window.hpp"
#include <iostream>
#include <sstream>
namespace win = helpers::windows;

int main()
{
    try {
        win::single sng(L"helper_single_creation.cpp");
        win::window wnd;
        win::run{}();
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::wstringstream s;
        s << e.what();
        MessageBox(nullptr, s.str().c_str(), L"Error", MB_OK);
    }

    std::cin.get();
}
