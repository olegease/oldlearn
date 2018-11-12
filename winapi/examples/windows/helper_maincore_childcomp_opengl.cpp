#include "../../helpers/windows/window.hpp"
#include "../../helpers/opengl/context.hpp"
#include <iostream>
namespace win = helpers::windows;
namespace gl = helpers::opengl;

constexpr int PIXELBUFFERSIZE = 320 * 240 * 3;
GLfloat ParentPixelBuffer[PIXELBUFFERSIZE];
GLfloat ChildPixelBuffer[PIXELBUFFERSIZE];

void draw_triangle()
{
    glBegin(GL_TRIANGLES);
    glColor3d(1.0, 0.0, 0.0);
    glVertex2d(0.0, 1.0);
    glColor3d(0.0, 1.0, 0.0);
    glVertex2d(1.0, -1.0);
    glColor3d(0.0, 0.0, 1.0);
    glVertex2d(-1.0, -1.0);
    glEnd();
}

void parent_events()
{
    RECT rect;
    MSG event = win::run::message;
    switch (event.message) {
    case WM_SIZE:
        win::run::context[event.hwnd]->make_current();
        GetClientRect(event.hwnd, &rect);
        glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);
        SendMessage(event.hwnd, WM_PAINT, 0, 0);
        break;
    case WM_PAINT:
        win::run::context[event.hwnd]->make_current();
        GetClientRect(event.hwnd, &rect);
        glClear(GL_COLOR_BUFFER_BIT);
        draw_triangle();
        glFlush();
        glReadPixels(0, rect.bottom - rect.top - 240, 320, 240, GL_RGB, GL_FLOAT, ParentPixelBuffer);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
}

void child_events()
{
    MSG event = win::run::message;
    switch (event.message) {
    case WM_SIZE:
        win::run::context[event.hwnd]->make_current();
        RECT rect;
        GetClientRect(event.hwnd, &rect);
        glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);
        SendMessage(event.hwnd, WM_PAINT, 0, 0);
        break;
    case WM_PAINT:
        win::run::context[event.hwnd]->make_current();
        glClear(GL_COLOR_BUFFER_BIT);
        draw_triangle();
        glFlush();
        glReadPixels(0, 0, 320, 240, GL_RGB, GL_FLOAT, ChildPixelBuffer);
        for (int rpos = 0, gpos = 1, bpos = 2; rpos < PIXELBUFFERSIZE; rpos += 3, gpos += 3, bpos += 3) {
            if (!ChildPixelBuffer[rpos] && !ChildPixelBuffer[gpos] && !ChildPixelBuffer[bpos]) {
                ChildPixelBuffer[rpos] = ParentPixelBuffer[rpos];
                ChildPixelBuffer[gpos] = ParentPixelBuffer[gpos];
                ChildPixelBuffer[bpos] = ParentPixelBuffer[bpos];
            }
        }
        glDrawPixels(320, 240, GL_RGB, GL_FLOAT, ChildPixelBuffer);
        glFlush();
        break;
    }
}

int main()
{
    try {
        win::window parent(win::style_graphic{});
        win::window_child child(parent, win::style_child{}, {0, 0, 320, 240});
        gl::context glc(child);
        gl::context_core glp(glc, parent, {gl::WGL_TYPE_RGBA_ARB, 4, 7});
        win::run::assign(&glp);
        win::run::assign(&glc);
        win::run::assign(parent_events, parent);
        win::run::assign(child_events, child);
        win::run{}();
    }
    catch (const win::exception::get_last_error& e) {
        std::cout << e.what() << ", error code: " << e.code();
    }

    std::cin.get();
}