#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

void main_options()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
}
