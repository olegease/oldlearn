#include "../../helpers/windows/window.hpp"
#include "../../helpers/opengl/context.hpp"
//#define GL_GLEXT_PROTOTYPES
#include "../../../gl-headers/glext.h"
#include <iostream>
namespace win = helpers::windows;
namespace gl = helpers::opengl;

constexpr int PIXELBUFFERSIZE = 320 * 240 * 3;
GLfloat ParentPixelBuffer[PIXELBUFFERSIZE];
GLfloat ChildPixelBuffer[PIXELBUFFERSIZE];

PFNGLBINDBUFFERPROC                 glBindBuffer;
PFNGLBINDVERTEXARRAYPROC            glBindVertexArray;
PFNGLBUFFERDATAPROC                 glBufferData;
PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;
PFNGLGENBUFFERSPROC                 glGenBuffers;
PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays;
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer;

PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;


const GLchar* vertex_shader_chars =
"#version 330 core\n"
"layout(location = 0) in vec3 vp;\n"
"layout(location = 1) in vec3 vc;\n"
"out vec3 fc;\n"
"void main(){\n"
    "gl_Position.xyz = vp;\n"
    "gl_Position.w = 1.0;\n"
    "fc = vc;\n"
"}";

const GLchar* fragment_shader_chars =
"#version 330 core\n"
"in vec3 fc;\n"
"out vec3 color;\n"
"void main(){\n"
    "color = fc;\n"
"}";
static const GLdouble vertex_buffer_data[] = {
    0.0, 1.0,
    1.0, -1.0,
    -1.0, -1.0
};

static const GLdouble color_buffer_data[] = {
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
};

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
    GLuint VertexShaderID;
    GLuint FragmentShaderID;
    GLuint VertexArrayID;
    GLuint VertexBufferID;
    GLuint ColorArrayID;
    GLuint ColorBufferID;
    GLuint programID;
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
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShaderID, 1, &vertex_shader_chars, nullptr);
        glCompileShader(VertexShaderID);
        FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragmentShaderID, 1, &fragment_shader_chars, nullptr);
        glCompileShader(FragmentShaderID);
        programID = glCreateProgram();
        glAttachShader(programID, VertexShaderID);
        glAttachShader(programID, FragmentShaderID);
        glLinkProgram(programID);

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        glGenBuffers(1, &VertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

        glGenVertexArrays(1, &ColorArrayID);
        glBindVertexArray(ColorArrayID);
        glGenBuffers(1, &ColorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, ColorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
        glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, ColorBufferID);
        glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, 0);
        glUseProgram(programID);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glFlush();
        GetClientRect(event.hwnd, &rect);
        glReadPixels(0, rect.bottom - rect.top - 240, 320, 240, GL_RGB, GL_FLOAT, ParentPixelBuffer);
        SwapBuffers(GetDC(event.hwnd));
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
        glc.make_current();
        glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
        glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
        glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
        glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
        glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
        glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
        glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");

        glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
        glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
        glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
        glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
        glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
        glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
        glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
        gl::context_core glp(glc, parent, {gl::WGL_TYPE_RGBA_ARB, 3, 3});
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