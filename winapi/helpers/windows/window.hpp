#pragma once

#include <Windows.h>
#include "create.hpp"

namespace helpers::windows {
    struct window;
    struct window
    {
        window(styles style = style_default{}, display_rect rect = display_rect{0, 0, 640, 480}) : hwnd(create(style, rect)) { }
        ~window() { DestroyWindow(hwnd); }
        operator HWND() { return hwnd; }
        operator HDC() { return GetDC(hwnd); }

        void show() { ShowWindow(hwnd, SW_SHOW); }
    private:
        HWND hwnd;
    };
}