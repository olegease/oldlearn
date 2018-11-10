#pragma once

#include <Windows.h>
#include "create.hpp"

namespace helpers::windows {
    struct window_base;
    struct window;
    struct window_child;

    struct window_base
    {
        window_base(styles style, display_rect rect) : hwnd(create(style, rect)) { }
        window_base(HWND parent, styles style = style_default{}, display_rect rect = display_rect{0, 0, 640, 480}) : hwnd(create_child(parent, style, rect)) { }
        ~window_base() { DestroyWindow(hwnd); }
        operator HWND() { return hwnd; }
        operator HDC() { return GetDC(hwnd); }
        void show() { ShowWindow(hwnd, SW_SHOW); }
        void hide() { ShowWindow(hwnd, SW_HIDE); }
    private:
        HWND hwnd;
    };

    struct window : window_base
    {
        window(styles style = style_default{}, display_rect rect = display_rect{0, 0, 640, 480}) : window_base(style, rect) { }
    };

    struct window_child : window_base
    {
        window_child(HWND parent, styles style = style_child{}, display_rect rect = display_rect{0, 0, 640, 480}) : window_base(parent, style, rect) { }
    };
}
