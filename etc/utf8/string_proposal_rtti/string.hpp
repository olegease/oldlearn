#pragma once

#include "core.hpp"
#include "data.hpp"
namespace utf8 {
    struct base_string
    {
        virtual_string_data* pdata;
        base_string() : pdata(nullptr) { }
        base_string(const char* cstr);
        ~base_string() { delete pdata; }
    };

    struct string
    {
        base_string str; // for to correct delete on exception use it as copmosition
        string(const char* cstr) : str(cstr) { for (int i = 0; i < str.pdata->sz; ++cstr, ++i) str.pdata->data[i] = *cstr; }
        virtual_string_data::proxy_code_point operator[](std::size_t k) { return str.pdata->operator[](k); }
        virtual_string_data::proxy_code_point operator[](std::size_t k) const { return str.pdata->operator[](k); }
        int mode() const { return str.pdata->mode(); }
        std::size_t size() const { return str.pdata->sz; }
        std::size_t length() const { return str.pdata->len; }
    };

    std::ostream& operator<<(std::ostream& os, const string& rhs)
    {
        os.write(rhs.str.pdata->data, rhs.str.pdata->sz);
        return os;
    }

    base_string::base_string(const char* cstr)
    {
        // TODO do it correctly work with empty strings "" and \0 terminationg characters
        int len = 0;
        const char* s = cstr;
        int all_bytes = process_codepoint(s);

        if (!all_bytes) throw std::runtime_error("utf8: error code point");
        s += all_bytes;
        len++;
        while (*s != '\0') {
            len++;
            int bytes = process_codepoint(s);
            if (bytes != all_bytes) throw std::runtime_error("utf8: error code point");
            s += bytes;
        }

        // TODO make it constexpr?
        //if (!all_bytes) pdata = new base_string_data< UTF8_DIFF_BYTES >(len);
        if (all_bytes == 1) pdata = new base_string_data< ALL_1_BYTE >(len);
        else if (all_bytes == 2) pdata = new base_string_data< ALL_2_BYTE >(len);
        else if (all_bytes == 3) pdata = new base_string_data< ALL_3_BYTE >(len);
        else if (all_bytes == 4) pdata = new base_string_data< ALL_4_BYTE >(len);
    }
}
