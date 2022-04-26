#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void BetterTextOut(HDC hdc, PSTR text, int x, int y);

void MessageBoxPrintf(HWND hwnd, LPCSTR title, LPCSTR format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    char buffer[1024];
    wvsprintf(buffer, format, arg_list);
    MessageBox(hwnd, buffer, title, MB_OK);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = TEXT("HelloWin");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_VREDRAW | CS_HREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(
            szAppName,
            TEXT("The Hello Program"),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            NULL,
            NULL,
            hInstance,
            NULL
    );

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    TEXTMETRIC tm;

    switch (message) {
        case WM_CREATE:
            PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
            hdc = GetDC(hwnd);
            GetTextMetrics(hdc, &tm);
            MessageBoxPrintf(hwnd, "Text Metric Info", "Some text metrics... ascent = %li, descent: %li, height: %li, ave width: %li, max width: %li",
                             tm.tmAscent, tm.tmDescent, tm.tmHeight, tm.tmAveCharWidth, tm.tmMaxCharWidth);
            ReleaseDC(hwnd, hdc);
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

void BetterTextOut(HDC hdc, PSTR text, int x, int y) {
    int count = 0;
    for (int i = 0; ; i++) {
        if (text[i] != '\0') count++;
        else break;
    }
    TextOut(hdc, x, y, text, sizeof(char) * count);
}