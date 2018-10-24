#include <iostream>
#include <string>
#include <Windows.h>

LRESULT WINAPI event_handler(HWND hWnd, UINT messageId, WPARAM wParam, LPARAM lParam);
void call_on_fail();
int return_on_fail();
void display_error_line(const std::string& str);

int main()
{
    std::wstring wclassname = L"application";
    WNDCLASSEX wclass;
    wclass.cbSize = sizeof(wclass);
    wclass.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
    wclass.lpfnWndProc = event_handler;
    wclass.cbClsExtra = 0;
    wclass.cbWndExtra = 0;
    wclass.hInstance = GetModuleHandle(NULL);
    wclass.hIcon = NULL;
    wclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wclass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
    wclass.lpszMenuName = NULL;
    wclass.lpszClassName = wclassname.c_str();
    wclass.hIconSm = NULL;
    ATOM rclassAtom = RegisterClassEx(&wclass);
    if (!rclassAtom) {
        display_error_line("RegisterClassEx fail.");
        return return_on_fail();
    }
    HWND wid = CreateWindowEx(
        WS_EX_LEFT,
        wclassname.c_str(),
        L"Window Title",
        WS_TILEDWINDOW,
        0,
        0,
        640,
        480,
        NULL,
        NULL,
        wclass.hInstance,
        NULL
    );
    bool prevIsVisible = ShowWindow(wid, SW_SHOW);
    if (prevIsVisible) display_error_line("Window was already showing");
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    std::cin.get();
}

LRESULT WINAPI event_handler(HWND hWnd, UINT messageId, WPARAM wParam, LPARAM lParam)
{
    switch (messageId) {
    case WM_CREATE:
        std::cout << "created" << std::endl;
        break;
    case WM_DESTROY:
        std::cout << "destroyed" << std::endl;
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, messageId, wParam, lParam);
}

void call_on_fail()
{

}

int return_on_fail()
{
    call_on_fail();
    return 0;
}

void display_error_line(const std::string& str)
{
    std::cout << "Error: " << str << std::endl;
}
