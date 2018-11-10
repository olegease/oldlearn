#include "../../helpers/windows/window.hpp"
#include "../../helpers/opengl/context.hpp"
#include <iostream>
namespace win = helpers::windows;
namespace gl = helpers::opengl;

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

void events()
{
    MSG event = win::run::message;
    switch (event.message) {
    case WM_SIZE:
        win::run::context[event.hwnd]->make_current();
        RECT rect;
        GetClientRect(event.hwnd, &rect);
        glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);
        PostMessage(event.hwnd, WM_PAINT, 0, 0);
    case WM_PAINT:
        win::run::context[event.hwnd]->make_current();
        glClear(GL_COLOR_BUFFER_BIT);
        draw_triangle();
        glFlush();
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
}

int main()
{
    try {
        win::window parent(win::style_graphic{});
        win::window_child child(parent, win::style_child{}, {0, 0, 320, 240});
        gl::context glp(parent);
        gl::context glc(child);
        win::run::assign(&glp);
        win::run::assign(&glc);
        win::run{}(events);
    }
    catch (const win::exception::get_last_error& e) {
        std::cout << e.what() << ", error code: " << e.code();
    }

    std::cin.get();
}