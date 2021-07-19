#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void initializeWindowClass(HINSTANCE &instance, WNDCLASS &windowClass, LPCTSTR windowClassName);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = "HelloWin";
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    initializeWindowClass(hInstance, wndclass, szAppName);

    // Register window class. If this function fails then it will return 0
    // indicating an error (on Windows 98 there is no RegisterClassW implemented so it could return 0
    // for that also.)
    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    // Creates the window internally in Windows (memory has been allocated for it)
    hwnd = CreateWindow(szAppName, // window class name
                        TEXT("The Hello Program"), // window caption
                        WS_OVERLAPPEDWINDOW, // window style
                        CW_USEDEFAULT, // initial x position
                        CW_USEDEFAULT, // initial y position
                        CW_USEDEFAULT, // initial x size
                        CW_USEDEFAULT, // initial y size
                        NULL, // parent window handle
                        NULL, // window menu handle
                        hInstance, // program instance handle
                        NULL); // creation parameters

    // Displays window on the desktop
    ShowWindow(hwnd, iCmdShow);

    // Causes window to get painted via WM_PAINT
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch(message) {
        case WM_CREATE:
            PlaySound(TEXT("hellowin .wav"), NULL, SND_FILENAME | SND_ASYNC);
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);
            DrawText(hdc, TEXT("Hello, Windows 98!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint(hwnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void initializeWindowClass(HINSTANCE &instance, WNDCLASS &windowClass, LPCTSTR windowClassName) {
    windowClass.style = CS_VREDRAW | CS_HREDRAW; // The window is to be repainted whenever the vertical or horizontal dimensions of the window change
    windowClass.lpfnWndProc = WndProc; // Pointer to window procedure function
    windowClass.cbClsExtra = 0; // Count in bytes of extra space for window class
    windowClass.cbWndExtra = 0; // Count in bytes of extra space for window
    windowClass.hInstance = instance; // Handle to program instance passed to WinMain
    windowClass.hIcon = LoadIcon(instance, IDI_APPLICATION); // Handle to icon
    windowClass.hCursor = LoadCursor(instance, IDC_APPSTARTING); // Handle to a cursor
    windowClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); // Handle to a brush
    windowClass.lpszMenuName = NULL; // The menu name (there is no menu so this is NULL)
    windowClass.lpszClassName = windowClassName; // The name of the window class (for single window applications it's usually the name of the program)
}