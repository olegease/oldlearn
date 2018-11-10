#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "format.hpp"
#include "../windows/exception.hpp"
#include <stdexcept>
namespace helpers::opengl {
    namespace exception {
        using namespace helpers::windows::exception;
    }

    HGLRC create(HDC dc, format f = format_default{})
    {
        PIXELFORMATDESCRIPTOR pfd = f;
        int pf = ChoosePixelFormat(dc, &pfd);
        if (!pf) throw exception::get_last_error("ChoosePixelFormat return zero");
        if (SetPixelFormat(dc, pf, &pfd) == FALSE) throw exception::get_last_error("SetPixelFormat return FALSE");
        if (!DescribePixelFormat(dc, pf, sizeof(pfd), &pfd)) throw exception::get_last_error("DescribePixelFormat return zero");
        HGLRC rc = wglCreateContext(dc);
        if (rc == nullptr) throw exception::get_last_error("wglCreateContext return nullptr");

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

        void make_current()
        {
            if (wglMakeCurrent(*this, *this) == FALSE) throw exception::get_last_error("wglMakeCurrent return false");
        }
    private:
        HDC dc;
        HGLRC rc;
    };
}
