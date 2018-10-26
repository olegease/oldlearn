#pragma once

#include <stdexcept>
#include <Windows.h>
#include <string>
namespace helpers::windows {
    namespace exception {
        struct single_create : public std::runtime_error { single_create(const char* msg) : runtime_error(msg) { } };
        struct single_exists : public std::runtime_error { single_exists(const char* msg) : runtime_error(msg) { } };
    }

    struct single
    {
        single() = delete;
        single(const std::wstring& name) : handle(CreateMutex(nullptr, true, name.c_str()))
        {
            if (handle == nullptr) throw exception::single_create("CreateMutex return nullptr");
            if (GetLastError() == ERROR_ALREADY_EXISTS) throw exception::single_exists("GetLastError for CreateMutex equal to ERROR_ALREADY_EXISTS");
        }
        ~single()
        {
            ReleaseMutex(handle);
            CloseHandle(handle);
        }
    private:
        HANDLE handle;
    };
}
