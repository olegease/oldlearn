#pragma once

#include <Windows.h>

namespace helpers::windows {
    struct run
    {
        using f = void();
        static f* global;
        static MSG message;

        static LRESULT WINAPI event_handler(HWND h, UINT m, WPARAM wp, LPARAM lp)
        {
            message.hwnd = h;
            message.message = m;
            message.wParam = wp;
            message.lParam = lp;
            if (global != nullptr) { global(); }
            return DefWindowProc(h, m, wp, lp);
        }

        static void assign(f* f, HWND window_id = nullptr)
        {
            if (window_id == nullptr) {
                global = f;
            }
            else {
                // TODO assign to child or whatever map of windows ids => [funcs, events|messages]
            }
        }

        void operator()(f* fnc = nullptr)
        {
            if (fnc != nullptr) global = fnc;
            MSG msg;
            while (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    };

    run::f* run::global = nullptr;
    MSG run::message;
}
