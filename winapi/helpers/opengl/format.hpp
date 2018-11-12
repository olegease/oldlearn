#pragma once

#include <Windows.h>

namespace helpers::opengl {
    constexpr GLenum WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091;
    constexpr GLenum WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092;
    constexpr GLenum WGL_CONTEXT_PROFILE_MASK_ARB = 0x9126;

    constexpr GLenum WGL_CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001;

    constexpr GLenum WGL_DRAW_TO_WINDOW_ARB = 0x2001;
    constexpr GLenum WGL_ACCELERATION_ARB = 0x2003;
    constexpr GLenum WGL_SUPPORT_OPENGL_ARB = 0x2010;
    constexpr GLenum WGL_DOUBLE_BUFFER_ARB = 0x2011;
    constexpr GLenum WGL_PIXEL_TYPE_ARB = 0x2013;
    constexpr GLenum WGL_COLOR_BITS_ARB = 0x2014;
    constexpr GLenum WGL_DEPTH_BITS_ARB = 0x2022;
    constexpr GLenum WGL_STENCIL_BITS_ARB = 0x2023;

    constexpr GLenum WGL_FULL_ACCELERATION_ARB = 0x2027;
    constexpr GLenum WGL_TYPE_RGBA_ARB = 0x202B;


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

    template< int size >
    struct pixelattribs
    {
        int data[size];
        pixelattribs(int* d) 
        {
            for (int i = 0; i < size; ++i) data[i] = d[i];
            data[size - 1] = 0;

        }
        operator int*() { return data; }
    };

    template< int size >
    struct attribs
    {
        int data[size];
        attribs(int* d)
        {
            for (int i = 0; i < size; ++i) data[i] = d[i];
            data[size - 1] = 0;
        }
        operator int*() { return data; }
    };

    struct format_core
    {
        int pixel_type;
        int major_version;
        int minor_version;
        
        format_core(GLenum pixel_type, int major, int minor) : pixel_type(pixel_type), major_version(major), minor_version(minor) { }
        operator pixelattribs< 17 >()
        {
            int temp[] = {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                WGL_PIXEL_TYPE_ARB, pixel_type,
                WGL_COLOR_BITS_ARB, 32,
                WGL_DEPTH_BITS_ARB, 24,
                WGL_STENCIL_BITS_ARB, 8
            };
            pixelattribs< 17 > pa(temp);
            return pa;
        }

        operator attribs< 5 >()
        {
            int temp[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, major_version,
                WGL_CONTEXT_MINOR_VERSION_ARB, minor_version
            };
            attribs< 5 > a(temp);
            return a;
        }
    };
}
