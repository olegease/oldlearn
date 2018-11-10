#pragma once

#include <Windows.h>
#include "style.hpp"
#include "run.hpp"
#include "single.hpp"
#include "exception.hpp"
#include <stdexcept>
#include <string>
namespace helpers::windows {
    namespace exception {

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
        if (!rclassAtom) throw exception::get_last_error("RegisterClassEx ATOM return false");
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
        if (!wid) throw exception::get_last_error("CreateWindowEx return nullptr");
        if (run::global == nullptr) ShowWindow(wid, SW_SHOW);
        return wid;
    }

    HWND create_child(HWND parent, styles style = style_child{}, display_rect rect = display_rect{0, 0, 640, 480})
    {
        static int n = 0; n++;
        std::wstring wclassname = L"create_child";
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
        wclass.hbrBackground = HBRUSH(COLOR_WINDOW + 2);
        wclass.lpszMenuName = NULL;
        wclass.lpszClassName = wclassname.c_str();
        wclass.hIconSm = NULL;
        ATOM rclassAtom = RegisterClassEx(&wclass);
        if (!rclassAtom) throw exception::get_last_error("RegisterClassEx ATOM return false");
        HWND wid = CreateWindowEx(
            style.extended,
            wclass.lpszClassName,
            L"Child Window Title",
            style.window,
            rect.leftXpos,
            rect.topYpos,
            rect.width,
            rect.height,
            parent,
            NULL,
            wclass.hInstance,
            NULL
        );
        if (!wid) throw exception::get_last_error("CreateWindowEx return nullptr");
        if (run::global == nullptr) ShowWindow(wid, SW_SHOW);
        return wid;
    }
}
