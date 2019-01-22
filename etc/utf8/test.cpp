// utf-8 PROTECTION FROM MICROSOFT: 🖕( ͡° ͜ʖ ͡° )🖕
#include "string_proposal_rtti.hpp"
#include "windows.h"
int main()
{
    SetConsoleOutputCP(65001);
    {
        try {
            utf8::string str("€ƒ");
            std::cout << str << std::endl;
            str[0] = "ƒ";
            str[1] = "@";
            std::cout << str << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    std::cin.get();
}
