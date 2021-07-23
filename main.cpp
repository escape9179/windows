#include <Windows.h>
#include "SystemMetrics.h"
#include <cstdio>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = TEXT("SysMets1");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
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
            TEXT("Get System Metrics No. 1"),
            WS_OVERLAPPEDWINDOW | WS_VSCROLL,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, hwnd, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static int cxChar, cxCaps, cyChar, iVscrollPos;
    HDC hdc;
    int i, strLength;
    PAINTSTRUCT ps;
    TCHAR szBuffer[10];
    TEXTMETRIC tm;
    USHORT cxClient, cyClient;

    switch (message) {
        case WM_CREATE:
            hdc = GetDC(hwnd);

            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
            cyChar = tm.tmHeight + tm.tmExternalLeading;

            ReleaseDC(hwnd, hdc);

            SetScrollRange(hwnd, SB_VERT, 0, NUMLINES, FALSE);
            SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
            break;
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            break;
        case WM_VSCROLL:
            switch (LOWORD(wParam)) {
                case SB_LINEUP:
                    iVscrollPos -= 1;
                    break;
                case SB_LINEDOWN:
                    iVscrollPos += 1;
                    break;
                case SB_PAGEUP:
                    iVscrollPos -= cyClient / cyChar;
                    break;
                case SB_PAGEDOWN:
                    iVscrollPos += cyClient / cyChar;
                    break;
                case SB_THUMBPOSITION:
                    iVscrollPos = HIWORD(wParam);
                    break;
                default:
                    break;
            }
            iVscrollPos = max(0, min(iVscrollPos, NUMLINES - 1));
            if (iVscrollPos != GetScrollPos(hwnd, SB_VERT)) {
                SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        case WM_HSCROLL:
            //TODO Implement horizontal scroll
            break;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            for (i = 0; i < NUMLINES; i++) {
                TextOut(hdc, 0, cyChar * i, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
                TextOut(hdc, 22 * cxCaps, cyChar * i, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
                SetTextAlign(hdc, TA_RIGHT | TA_TOP);
                TextOut(hdc, 22 * cxCaps + 40 * cxChar, cyChar * i, szBuffer,
                        wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));
                SetTextAlign(hdc, TA_LEFT | TA_TOP);
            }
            EndPaint(hwnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}