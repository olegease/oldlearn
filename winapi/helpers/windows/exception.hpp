#pragma once

#include <Windows.h>
#include <stdexcept>

namespace helpers::windows::exception {
    struct get_last_error : std::runtime_error
    {
        get_last_error(const char* msg) : std::runtime_error(msg), error_code(GetLastError()) { }
        int code() const { return error_code; }
    private:
        int error_code;
    };
}
