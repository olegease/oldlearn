#include "defines.hpp"
#include "string_proposal_rtti/string.hpp"
#include <string>

void print_test_utf8string(const utf8::string& str)
{
    std::cout
        << str
        << " -> mode: "
        << str.mode()
        << ", size: " << str.size()
        << ", length: " << str.length()
        << ", first char: " << str[0]
        << ", middle char: " << str[str.length() / 2]
        << ", last char: " << str[str.length() - 1]
        << std::endl;
}

int main()
{
    main_options();
    {
        try {
            // test creating for 1, 2, 3, 4 byte string with 1 char, and cout it
            utf8::string str1_1("\x24");
            print_test_utf8string(str1_1);
            utf8::string str2_1 = "\xC6\x92";
            print_test_utf8string(str2_1);
            utf8::string str3_1 = "\xE2\x82\xAC";
            print_test_utf8string(str3_1);
            utf8::string str4_1 = "\xF0\x9F\x82\xA0";
            print_test_utf8string(str4_1);


            // test creating for 1, 2, 3, 4 byte string with 3 chars, and cout it
            utf8::string str1_3("\x24\x25\x26");
            print_test_utf8string(str1_3);
            utf8::string str2_3 = "\xC6\x92\xC6\x93\xC6\x94";
            print_test_utf8string(str2_3);
            utf8::string str3_3 = "\xE2\x82\xAC\xE2\x82\xAD\xE2\x82\xAE";
            print_test_utf8string(str3_3);
            utf8::string str4_3 = "\xF0\x9F\x82\xA0\xF0\x9F\x82\xA1\xF0\x9F\x82\xA2";
            print_test_utf8string(str4_3);

            // test changing first char for 1, 2, 3, 4 byte string with 3 chars, and cout it


        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    std::cin.get();
}
