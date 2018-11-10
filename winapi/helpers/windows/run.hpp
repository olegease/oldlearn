#pragma once

#include <Windows.h>
#include <map>
#include "../opengl/context.hpp"

namespace helpers::windows {
    struct run
    {
        using f = void();
        static f* global;
        static MSG message;
        static std::map< HWND, f* > window;
        static std::map< HWND, helpers::opengl::context* > context;

        static LRESULT WINAPI event_handler(HWND h, UINT m, WPARAM wp, LPARAM lp)
        {
            message.hwnd = h;
            message.message = m;
            message.wParam = wp;
            message.lParam = lp;
            if (window.find(h) != window.end()) { window[h](); }
            else if (global != nullptr) { global(); }
            else {
                switch (m) {
                case WM_DESTROY:
                    PostQuitMessage(0);
                    break;
                }
            }

            return DefWindowProc(h, m, wp, lp);
        }

        static void assign(f* f, HWND window_id = nullptr)
        {
            if (window_id == nullptr) {
                global = f;
            }
            else {
                // TODO assign to child or whatever map of windows ids => [funcs, events|messages]
                window[window_id] = f;
            }
        }

        static void assign(helpers::opengl::context* c)
        {
            context[*c] = c;
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
    std::map< HWND, run::f* > run::window;
    std::map< HWND, helpers::opengl::context* > run::context;
}
