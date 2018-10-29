#pragma once

#include <Windows.h>
#include "format.hpp"
namespace helpers::opengl {
    namespace exception {
        //struct dc { };
    }

    HGLRC create(HDC dc, format f = format_default{})
    {
        PIXELFORMATDESCRIPTOR pfd = f;
        
        int pf = ChoosePixelFormat(dc, &pfd);
        SetPixelFormat(dc, pf, &pfd);
        DescribePixelFormat(dc, pf, sizeof(pfd), &pfd);
        HGLRC rc = wglCreateContext(dc);
        wglMakeCurrent(dc, rc);
        return rc;
    }

    void destroy(HDC dc, HGLRC rc)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(rc);
        ReleaseDC(WindowFromDC(dc), dc);
    }

    struct context {
        context(HDC dc, format f = format_default{}) : dc(dc), rc(create(dc, f)) { }
        ~context()
        {
            destroy(dc, rc);
            /*wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(*this);
            ReleaseDC(*this, *this);*/
        }
        operator HWND() { return WindowFromDC(dc); }
        operator HDC() { return dc; }
        operator HGLRC() { return rc; }
    private:
        HDC dc;
        HGLRC rc;
    };
}