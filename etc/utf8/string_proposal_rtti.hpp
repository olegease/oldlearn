// utf-8 PROTECTION FROM MICROSOFT: 🖕( ͡° ͜ʖ ͡° )🖕

#include <iostream>
#include <cstring>
#include <exception>

namespace utf8 {

    constexpr uint8_t DIFF_BYTES = 0;
    constexpr uint8_t ALL_1_BYTE = 1;
    constexpr uint8_t ALL_2_BYTE = 2;
    constexpr uint8_t ALL_3_BYTE = 3;
    constexpr uint8_t ALL_4_BYTE = 4;

    struct proxy_code_point
    {
        using pchar = char*;
        pchar code_unit[2];
        proxy_code_point(char* cstr)
        {
            code_unit[0] = cstr;
            code_unit[1] = &cstr[1];
        }
        proxy_code_point& operator=(const char* cstr)
        {
            *code_unit[0] = *cstr;
            *code_unit[1] = cstr[1];
            return *this;
        }

        proxy_code_point& operator=(const proxy_code_point& pcp)
        {
            *code_unit[0] = *pcp.code_unit[0];
            *code_unit[1] = *pcp.code_unit[1];
            return *this;
        }
    };

    struct virtual_string_data
    {
        char* data;
        virtual_string_data() : data(nullptr) { }
        ~virtual_string_data() { delete[] data; std::cout << "delete[] data" << std::endl; }
    };

    struct base_string_data : public virtual_string_data
    {
        std::size_t sz;
        std::size_t len;
        base_string_data() : virtual_string_data() { }
        base_string_data(std::size_t size) : sz(size), len(sz / 2) { data = new char[sz]; }

        proxy_code_point operator[](int key)
        {
            return &data[key * 2];
        }
    };

    struct base_string
    {
        base_string_data* pdata;
        base_string() : pdata(nullptr) { }
        base_string(const char* cstr) : pdata(new base_string_data(strlen(cstr))) { }
        ~base_string() { delete pdata; std::cout << "delete pdata" << std::endl; }
    };

    struct string : public base_string
    {
        string(const char* cstr) : base_string(cstr) { for (int i = 0; i < pdata->sz; ++cstr, ++i) pdata->data[i] = *cstr; }
        friend std::ostream& operator<<(std::ostream& os, const string& str)
        {
            os.write(str.pdata->data, str.pdata->sz);
            return os;
        }

        proxy_code_point operator[](int k)
        {
            return pdata->operator[](k);
        }
    };


}
