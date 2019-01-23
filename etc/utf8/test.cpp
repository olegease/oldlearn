#include "string_proposal_rtti.hpp"
#include "defines.hpp"
int main()
{
    main_options();
    {
        try {
            // test creating for 1, 2, 3, 4 byte string with 1 char, and cout it
            utf8::string str("\x24\x24\x24\x24");
            std::cout << str << " -> size: " << str.size() << ", length: " << str.length() << ", first char:" << str[0] << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    std::cin.get();
}
