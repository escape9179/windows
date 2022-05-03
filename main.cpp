#include <windows.h>
#include <algorithm>
#include "SystemMetrics.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void BetterTextOut(HDC hdc, PSTR text, int x, int y);

void MessageBoxPrintf(HWND hwnd, LPCSTR title, LPCSTR format, ...) {
    va_list argList;
    va_start(argList, format);
    char buffer[1024];
    wvsprintf(buffer, format, argList);
    MessageBox(hwnd, buffer, title, MB_OK);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR appName[] = TEXT("HelloWin");
    HWND windowHandle;
    MSG message;
    WNDCLASS windowClass;

    windowClass.style = CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = appName;

    if (!RegisterClass(&windowClass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), appName, MB_ICONERROR);
        return 0;
    }

    windowHandle = CreateWindow(
            appName,
            TEXT("The Hello Program"),
            WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            NULL,
            NULL,
            hInstance,
            NULL
    );

    ShowWindow(windowHandle, iCmdShow);
    UpdateWindow(windowHandle);

    while(GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC deviceContext;
    PAINTSTRUCT paintStruct;
    RECT rectangle;
    TCHAR charBuffer[10];
    TEXTMETRIC textMetric;
    static int charWidth, capsWidth, charHeight;
    static int clientWidth;
    static int clientHeight;
    static int vertScrollBarPos;

    switch (message) {
        case WM_CREATE:
            PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
            deviceContext = GetDC(hwnd);
            TEXTMETRIC tm;
            GetTextMetrics(deviceContext, &tm);
            charWidth = tm.tmAveCharWidth;
            capsWidth = (tm.tmPitchAndFamily & 1 ? 3 : 2) * charWidth / 2;
            charHeight = tm.tmHeight + tm.tmExternalLeading;
            ReleaseDC(hwnd, deviceContext);
            SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
            SetScrollPos(hwnd, SB_VERT, vertScrollBarPos, FALSE);
            return 0;
        case WM_PAINT:
            deviceContext = BeginPaint(hwnd, &paintStruct);
            for (int i = 0; i < NUMLINES; i++) {
                int y = charHeight * (i - vertScrollBarPos);
                TextOut(deviceContext, 0, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
                TextOut(deviceContext, 22 * capsWidth, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
                SetTextAlign(deviceContext, TA_RIGHT | TA_TOP);
                TextOut(deviceContext, 22 * capsWidth + 40 * charWidth, y, charBuffer, wsprintf(charBuffer, TEXT("%5d"),
                                                                                                             GetSystemMetrics(sysmetrics[i].iIndex)));
                SetTextAlign(deviceContext, TA_LEFT | TA_TOP);
            }
            EndPaint(hwnd, &paintStruct);
            return 0;
        case WM_VSCROLL:
            switch(LOWORD(wParam)) {
                case SB_LINEUP:
                    vertScrollBarPos -= 1;
                    break;
                case SB_LINEDOWN:
                    vertScrollBarPos += 1;
                    break;
                case SB_PAGEUP:
                    vertScrollBarPos -= clientHeight / charHeight;
                    break;
                case SB_PAGEDOWN:
                    vertScrollBarPos += clientHeight / charHeight;
                case SB_THUMBPOSITION:
                    vertScrollBarPos = HIWORD(wParam);
            }
            vertScrollBarPos = std::max(0, std::min(vertScrollBarPos, NUMLINES - 1));
            if(vertScrollBarPos != GetScrollPos(hwnd, SB_VERT)) {
                SetScrollPos(hwnd, SB_VERT, vertScrollBarPos, FALSE);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        case WM_SIZE:
            clientWidth = HIWORD(lParam);
            clientHeight = LOWORD(lParam);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}