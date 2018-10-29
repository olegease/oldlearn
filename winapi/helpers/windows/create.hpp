#pragma once

#include <Windows.h>
#include "style.hpp"
#include "run.hpp"
#include "single.hpp"
#include <stdexcept>
#include <string>
namespace helpers::windows {
    namespace exception {
        struct register_class : std::runtime_error { register_class(const char* msg) : std::runtime_error(msg) { } };
        struct create_window : std::runtime_error { create_window(const char* msg) : std::runtime_error(msg) { } };
    }

    struct display_rect;
    struct display_rect
    {
        int leftXpos;
        int topYpos;
        int width;
        int height;
        display_rect(int x, int y, int width, int height) : leftXpos(x), topYpos(y), width(width), height(height) { }
    };

    HWND create(styles style = style_default{}, display_rect rect = display_rect{0, 0, 640, 480})
    {
        static int n = 0; n++;
        std::wstring wclassname = L"create";
        wclassname += n;
        WNDCLASSEX wclass;
        wclass.cbSize = sizeof(wclass);
        wclass.style = style.sclass;
        wclass.lpfnWndProc = run::event_handler;
        wclass.cbClsExtra = 0;
        wclass.cbWndExtra = 0;
        wclass.hInstance = GetModuleHandle(NULL);
        wclass.hIcon = NULL;
        wclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wclass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
        wclass.lpszMenuName = NULL;
        wclass.lpszClassName = wclassname.c_str();
        wclass.hIconSm = NULL;
        ATOM rclassAtom = RegisterClassEx(&wclass);
        if (!rclassAtom) throw exception::register_class((std::string{"RegisterClassEx ATOM return false, errorcode: "} + std::to_string(GetLastError())).c_str());
        HWND wid = CreateWindowEx(
            style.extended,
            wclass.lpszClassName,
            L"Window Title",
            style.window,
            rect.leftXpos,
            rect.topYpos,
            rect.width,
            rect.height,
            NULL,
            NULL,
            wclass.hInstance,
            NULL
        );
        if (!wid) throw exception::create_window((std::string{"CreateWindowEx return nullptr, errorcode: "} + std::to_string(GetLastError())).c_str());
        if (run::global == nullptr) ShowWindow(wid, SW_SHOW);
        return wid;
    }
}
