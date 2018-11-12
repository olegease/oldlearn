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

    typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext,
        const int *attribList);
    wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;
    typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList,
        const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
    wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;

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

    struct context_base
    {
        context_base(HDC dc, HGLRC rc) : dc(dc), rc(rc) { }
        operator HWND() { return WindowFromDC(dc); }
        operator HDC() { return dc; }
        operator HGLRC() { return rc; }
        void make_current()
        {
            if (wglMakeCurrent(*this, *this) == FALSE) throw exception::get_last_error("wglMakeCurrent return false");
        }
    protected:
        HDC dc;
        HGLRC rc;
    };

    struct context : context_base {
        context(HDC dc, format f = format_default{}) : context_base(dc, create(dc, f)) { }
        ~context()
        {
            destroy(*this, *this);
        }
    };

    HGLRC create_core(context& dummy, HDC dc, format_core f)
    {
        //PIXELFORMATDESCRIPTOR opfd = format{format_default{}};
        wglMakeCurrent(dummy, dummy);
        wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
        if (!wglCreateContextAttribsARB) throw exception::get_last_error("wglCreateContextAttribsARB is nullptr");
        wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");
        if (!wglChoosePixelFormatARB) throw exception::get_last_error("wglChoosePixelFormatARB is nullptr");
        // modern
        int* pixelformatattribs = pixelattribs< 17 >(f);
        int pixel_format;
        UINT num_formats;
        wglChoosePixelFormatARB(dc, pixelformatattribs, 0, 1, &pixel_format, &num_formats);
        if (!num_formats) throw std::runtime_error("wglChoosePixelFormatARB num formats set to zero");
        PIXELFORMATDESCRIPTOR pfd;
        if (!DescribePixelFormat(dc, pixel_format, sizeof(pfd), &pfd)) throw exception::get_last_error("DescribePixelFormat for core return zero");
        if (!SetPixelFormat(dc, pixel_format, &pfd)) throw exception::get_last_error("SetPixelFormat for core return FALSE");
        int* a = attribs< 5 >(f);
        HGLRC rc = wglCreateContextAttribsARB(dc, 0, a);
        if (!rc) throw exception::get_last_error("wglCreateContexAttribsARB return false");
        wglMakeCurrent(nullptr, nullptr);
        return rc;
    }

    struct context_core : context_base
    {
        context_core(context& dummy, HDC dc, format_core f) : context_base(dc, dummy) { rc = create_core(dummy, dc, f); }
    };
}
