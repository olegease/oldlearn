#pragma once

#include "core.hpp"
namespace utf8 {

    constexpr int PROXY_CODE_POINT_SIZE = ALL_4_BYTE;


    struct virtual_string_data
    {
        struct proxy_code_point // use only as nested because it highly relate to virtual_string_data
        {
            using pchar = char*;
            pchar code_unit[PROXY_CODE_POINT_SIZE];
            proxy_code_point(char* cstr);
            proxy_code_point(virtual_string_data* pdata, std::size_t start_index);
            proxy_code_point& operator=(const char* cstr);
            proxy_code_point& operator=(const proxy_code_point& pcp);
            char& operator[](std::size_t key) { return *code_unit[key]; }
        };

        char* data;
        std::size_t sz;
        std::size_t len;
        virtual_string_data() : data(nullptr) { }
        virtual ~virtual_string_data() { delete[] data; }
        virtual proxy_code_point operator[](std::size_t key) = 0;
        virtual int mode() const = 0;
    };

    template< int MODE >
    struct base_string_data : public virtual_string_data
    {
        base_string_data() : virtual_string_data() { }
        base_string_data(std::size_t length) { len = length; sz = len * MODE; data = new char[sz]; }
        virtual proxy_code_point operator[](std::size_t key) { return {this, key * MODE}; }
        virtual int mode() const { return MODE; }
    };

    std::ostream& operator<<(std::ostream& os, const virtual_string_data::proxy_code_point& rhs)
    {
        int i = 0;
        while (rhs.code_unit[i] != EMPTY_POINTER && i < PROXY_CODE_POINT_SIZE) ++i;
        os.write(*rhs.code_unit, i);
        return os;
    }

    virtual_string_data::proxy_code_point::proxy_code_point(virtual_string_data* pdata, std::size_t start_index)
    {
        int i = 0;
        while (i < pdata->mode()) {
            code_unit[i] = &(pdata->data[start_index + i]);
            ++i;
        }
        while (i < PROXY_CODE_POINT_SIZE) {
            code_unit[i] = EMPTY_POINTER;
            ++i;
        }
    }

    virtual_string_data::proxy_code_point::proxy_code_point(char* cstr)
    {
        int i = 0;
        while (*cstr != '\0' && i < PROXY_CODE_POINT_SIZE) {
            code_unit[i] = cstr;
            ++cstr;
            ++i;
        }
        while (i < PROXY_CODE_POINT_SIZE) {
            code_unit[i] = EMPTY_POINTER;
            ++i;
        }
    }

    virtual_string_data::proxy_code_point& virtual_string_data::proxy_code_point::operator=(const char* cstr)
    {
        for (int i = 0; i < PROXY_CODE_POINT_SIZE && *cstr != '\0'; ++cstr, ++i) *code_unit[i] = *cstr;
        return *this;
    }

    virtual_string_data::proxy_code_point& virtual_string_data::proxy_code_point::operator=(const virtual_string_data::proxy_code_point& pcp)
    {
        for (int i = 0; i < PROXY_CODE_POINT_SIZE && *pcp.code_unit[i] != EMPTY; ++i) *code_unit[i] = *pcp.code_unit[i];
        return *this;
    }
}
