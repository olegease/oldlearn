#pragma once

#include <Windows.h>

namespace helpers::opengl {

    struct format
    {
        DWORD dwFlags;
        BYTE iPixelType;
        format(DWORD dwFlags, BYTE iPixelType) : dwFlags(dwFlags), iPixelType(iPixelType) { }
        operator PIXELFORMATDESCRIPTOR()
        {
            PIXELFORMATDESCRIPTOR pfd;
            pfd.nSize = sizeof(pfd);
            pfd.nVersion = 1;
            pfd.dwFlags = dwFlags | PFD_SUPPORT_OPENGL;
            pfd.iPixelType = iPixelType;
            pfd.cColorBits = 32;
            // TODO: need properly testing to add below properties
            /*pfd.cRedBits = 8;
            pfd.cRedShift = 8;
            pfd.cGreenBits = 8;
            pfd.cGreenShift = 0;
            pfd.cBlueBits = 8;
            pfd.cBlueShift = 18;
            pfd.cAlphaBits = 8;
            pfd.cAlphaShift = 24;
            pfd.cAccumBits = 16;
            pfd.cAccumRedBits = 4;
            pfd.cAccumGreenBits = 4;
            pfd.cAccumBlueBits = 4;
            pfd.cAccumAlphaBits = 4;
            pfd.cDepthBits = 24;
            pfd.cStencilBits = 24;*/
            // pfd.cAuxBuffers = not supported;
            // pfd.iLayerType = ignored;
            // TODO: pfd.bReserved = need more info;
            // pfd.dwLayerMask = ignored;
            // pfd.dwVisibleMask = need more info;
            // pfd.dwDamageMask = ignored;

            return pfd;
        }
    };

    struct format_base
    {
        DWORD dwFlags;
        BYTE iPixelType;
        format_base(DWORD dwFlags, BYTE iPixelType) : dwFlags(dwFlags), iPixelType(iPixelType) { }
        operator format() { return {dwFlags, iPixelType}; }
    };

    struct format_default : public format_base
    {
        format_default() : format_base(
            PFD_DRAW_TO_WINDOW,
            PFD_TYPE_RGBA)
        { }
    };

    struct format_animation : public format_base
    {
        format_animation() : format_base(
            PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA)
        { }
    };
}