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

namespace exception {
    struct mutex_create { };
    struct mutex_exists { };
}

int main()
{
    HANDLE single_mutex = CreateMutex(NULL, true, L"single_creation.cpp");
    if (single_mutex == NULL) {
        std::cout << "Error: Cannot create mutex." << std::endl;
        MessageBox(NULL, L"Cannot create mutex.", L"Error", MB_ICONERROR);
        return 0;
    }
    else if (GetLastError() == ERROR_ALREADY_EXISTS) {
        std::cout << "Warning: Application already running." << std::endl;
        MessageBox(NULL, L"Application already running.", L"Warning", MB_ICONWARNING);
        return 0;
    }
    hlpwin::run::assign(f);
    HWND id = hlpwin::create();
    hlpwin::run{}();
    std::cin.get();
    ReleaseMutex(single_mutex);
    CloseHandle(single_mutex);
}
