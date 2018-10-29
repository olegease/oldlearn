#include "../../helpers/windows/window.hpp"
#include "../../helpers/opengl/context.hpp"

#include <iostream>
#include <gl/GL.h>
#include <gl/GLU.h>

namespace win = helpers::windows;
namespace gl = helpers::opengl;

void events()
{
    MSG m = win::run::message;
    switch (m.message) {
    case WM_CREATE:
        std::cout << "created" << std::endl;
        break;
    case WM_SIZE:
        std::cout << "sized" << std::endl;
        RECT rect;
        GetClientRect(m.hwnd, &rect);
        glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);
        PostMessage(m.hwnd, WM_PAINT, 0, 0);
        break;
    case WM_PAINT:
        std::cout << "painted" << std::endl;
        glClearColor(1.f, 0.8f, 0.6f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        glPointSize(32);
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2i(0, 0);
        glEnd();
        glFlush();
        break;
    case WM_DESTROY:
        std::cout << "destroyed" << std::endl;
        PostQuitMessage(0);
        break;
    }
}

int main()
{
    win::run::assign(events);
    win::single sng(L"helper_simple_opengl.cpp");
    win::window wnd(win::style_graphic{});
    gl::context context(wnd, gl::format_default{});
    wnd.show();
    win::run{}();
}