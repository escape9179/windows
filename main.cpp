#include <windows.h>
#include "SystemMetrics.h"

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
    TCHAR sz_buffer[10];
    TEXTMETRIC tm;
    static int cx_char, cx_caps, cy_char;
    static int clientWidth;
    static int clientHeight;

    switch (message) {
        case WM_CREATE:
            PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
            hdc = GetDC(hwnd);
            TEXTMETRIC tm;
            GetTextMetrics(hdc, &tm);
            cx_char = tm.tmAveCharWidth;
            cx_caps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cx_char / 2;
            cy_char = tm.tmHeight + tm.tmExternalLeading;
            ReleaseDC(hwnd, hdc);
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            for (int i = 0; i < NUMLINES; i++) {
                TextOut(hdc, 0, cy_char * i, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
                TextOut(hdc, 22 * cx_caps, cy_char * i, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
                SetTextAlign(hdc, TA_RIGHT | TA_TOP);
                TextOut(hdc, 22 * cx_caps + 40 * cx_char, cy_char * i, sz_buffer, wsprintf(sz_buffer, TEXT("%5d"),
                                                                                           GetSystemMetrics(sysmetrics[i].iIndex)));
                SetTextAlign(hdc, TA_LEFT | TA_TOP);
            }
            EndPaint(hwnd, &ps);
            return 0;
        case WM_SIZE:
            clientWidth = HIWORD(lParam);
            clientHeight = LOWORD(lParam);
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