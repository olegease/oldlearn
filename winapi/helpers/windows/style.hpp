#pragma once
#include <Windows.h>

namespace helpers::windows {
    struct styles;
    struct style_base;
    /*struct window_styles;
    struct class_styles;
    struct extended_styles;*/

    struct styles
    {
        DWORD sclass;
        DWORD window;
        DWORD extended;
        styles(DWORD sclass = 0L, DWORD window = 0L, DWORD extended = 0L) : sclass(sclass), window(window), extended(extended) { }
    };

    struct style_base
    {
        DWORD sclass;
        DWORD window;
        DWORD extended;
        style_base(DWORD sclass = 0L, DWORD window = 0L, DWORD extended = 0L) : sclass(sclass), window(window), extended(extended) { }
        operator styles() { return {sclass, window, extended}; }
    };

    struct style_default : public style_base
    {
        style_default() : style_base(
            CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
            WS_TILEDWINDOW | WS_VISIBLE,
            WS_EX_LEFT
        )
        { }
    };

    struct style_graphic : public style_base
    {
        style_graphic() : style_base(
            CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
            WS_TILEDWINDOW | WS_CLIPCHILDREN,
            WS_EX_LEFT
        )
        { }
    };

    struct style_child : public style_base
    {
        style_child() : style_base(
            CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
            WS_CHILD,
            WS_EX_LEFT
        )
        { }

    };
}
