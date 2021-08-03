#include <Windows.h>
#include <cstdio>
#include "SystemMetrics.h"

LRESULT CALLBACK windowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int commandShow) {
    LPSTR appName = "App Name";

    WNDCLASS windowClass;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = windowProcedure;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = instance;
    windowClass.hIcon = LoadIcon(instance, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(instance, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = appName;

    if (!RegisterClass(&windowClass)) {
        MessageBox(NULL, TEXT("Error registering class"), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND windowHandle = CreateWindow(
            appName, TEXT("Window Name"), WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, instance, NULL
    );

    ShowWindow(windowHandle, commandShow);
    UpdateWindow(windowHandle);

    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return message.wParam;
}

LRESULT CALLBACK windowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC deviceContext;
    PAINTSTRUCT paintStruct;
    RECT clientRect;
    TEXTMETRIC textMetrics;
    SCROLLINFO scrollInfo;
    TCHAR buffer[10];
    static int charWidth;
    static int charWidthCaps;
    static int charHeight;
    static int maxWidth;
    static int clientWidth;
    static int clientHeight;
    int vertScrollbarPos;
    int horzScrollbarPos;
    int paintBegin;
    int paintEnd;
    int i;
    int x;
    int y;
    switch (message) {
        case WM_CREATE:
            deviceContext = GetDC(windowHandle);
            GetTextMetrics(deviceContext, &textMetrics);
            charWidth = textMetrics.tmAveCharWidth;
            charWidthCaps = (textMetrics.tmPitchAndFamily & 1 ? 3 : 2) * charWidth / 2;
            charHeight = textMetrics.tmHeight + textMetrics.tmExternalLeading;
            ReleaseDC(windowHandle, deviceContext);
            maxWidth = 40 * charWidth + 22 * charWidthCaps; //TODO Why are we saving the width of the three columns?
            break;
        case WM_SIZE:
            clientWidth = LOWORD(lParam);
            clientHeight = HIWORD(lParam);

            // Set vertical scrollbar range and page size
            scrollInfo.cbSize = sizeof(SCROLLINFO);
            scrollInfo.fMask = SIF_RANGE | SIF_PAGE;
            scrollInfo.nMin = 0;
            scrollInfo.nMax = NUMLINES - 1;
            scrollInfo.nPage = clientHeight / charHeight;
            SetScrollInfo(windowHandle, SB_VERT, &scrollInfo, TRUE);

            // Set horizontal scrollbar range and page size
            scrollInfo.cbSize = sizeof(SCROLLINFO);
            scrollInfo.fMask = SIF_RANGE | SIF_PAGE;
            scrollInfo.nMin = 0;
            scrollInfo.nMax = 2 + maxWidth / charWidth; //TODO What is this doing?
            scrollInfo.nPage = clientWidth / charWidth;
            SetScrollInfo(windowHandle, SB_HORZ, &scrollInfo, TRUE);
            break;
        case WM_VSCROLL:
            // Get all the vertical scrollbar information
            scrollInfo.cbSize = sizeof(SCROLLBARINFO);
            scrollInfo.fMask = SIF_ALL;
            GetScrollInfo(windowHandle, SB_VERT, &scrollInfo);

            vertScrollbarPos = scrollInfo.nPos;

            switch (LOWORD(wParam)) {
                case SB_TOP:
                    scrollInfo.nPos = scrollInfo.nMin;
                    break;
                case SB_BOTTOM:
                    scrollInfo.nPos = scrollInfo.nMax;
                    break;
                case SB_LINEUP:
                    scrollInfo.nPos -= 1;
                    break;
                case SB_LINEDOWN:
                    scrollInfo.nPos += 1;
                    break;
                case SB_PAGEUP:
                    scrollInfo.nPos -= scrollInfo.nPage;
                    break;
                case SB_PAGEDOWN:
                    scrollInfo.nPos += scrollInfo.nPage;
                    break;
                case SB_THUMBTRACK:
                    scrollInfo.nPos = scrollInfo.nTrackPos;
                    break;
            }

            // Set the position and then retrieve it. Due to adjustments
            // by Windows it may not be the same as the value set.
            scrollInfo.fMask = SIF_POS;
            SetScrollInfo(windowHandle, SB_VERT, &scrollInfo, TRUE);
            GetScrollInfo(windowHandle, SB_VERT, &scrollInfo);

            // If the position has changed, scroll the window and update it
            if (scrollInfo.nPos != vertScrollbarPos) {
                ScrollWindow(windowHandle, 0, charHeight * (vertScrollbarPos - scrollInfo.nPos), NULL, NULL);
                UpdateWindow(windowHandle);
            }
            break;
        case WM_HSCROLL:
            // Get all the horizontal scrollbar information.
            scrollInfo.cbSize = sizeof(scrollInfo);
            scrollInfo.fMask = SIF_ALL;

            GetScrollInfo(windowHandle, SB_HORZ, &scrollInfo);
            // Save the position for comparison later on
            horzScrollbarPos = scrollInfo.nPos;

            switch (LOWORD(wParam)) {
                case SB_LINELEFT:
                    scrollInfo.nPos -= 1;
                    break;
                case SB_LINERIGHT:
                    scrollInfo.nPos += 1;
                    break;
                case SB_PAGELEFT:
                    scrollInfo.nPos -= scrollInfo.nPage;
                    break;
                case SB_PAGERIGHT:
                    scrollInfo.nPos += scrollInfo.nPage;
                    break;
                case SB_THUMBPOSITION:
                    scrollInfo.nPos = scrollInfo.nTrackPos;
                    break;
                default:
                    break;
            }

            scrollInfo.fMask = SIF_POS;
            SetScrollInfo(windowHandle, SB_HORZ, &scrollInfo, TRUE);
            GetScrollInfo(windowHandle, SB_HORZ, &scrollInfo);

            if (scrollInfo.nPos != horzScrollbarPos) {
                ScrollWindow(windowHandle, charWidth * (horzScrollbarPos - scrollInfo.nPos), 0, NULL, NULL);
            }
            break;
        case WM_PAINT: {
            deviceContext = BeginPaint(windowHandle, &paintStruct);

            // Get vertical scrollbar position
            scrollInfo.cbSize = sizeof(scrollInfo);
            scrollInfo.fMask = SIF_POS;
            GetScrollInfo(windowHandle, SB_VERT, &scrollInfo);
            vertScrollbarPos = scrollInfo.nPos;

            // Get horizontal scrollbar position
            GetScrollInfo(windowHandle, SB_HORZ, &scrollInfo);
            horzScrollbarPos = scrollInfo.nPos;

            // Find painting limits
            paintBegin = max(0, vertScrollbarPos + paintStruct.rcPaint.top / charHeight);
            paintEnd = min(NUMLINES - 1, vertScrollbarPos + paintStruct.rcPaint.bottom / charHeight);

            for (i = paintBegin; i <= paintEnd; i++) {
                x = charWidth * (1 - horzScrollbarPos);
                y = charHeight * (i - vertScrollbarPos);

                TextOut(deviceContext, x, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
                TextOut(deviceContext, x + 22 * charWidthCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
                SetTextAlign(deviceContext, TA_RIGHT | TA_TOP);
                TextOut(deviceContext, x + 22 * charWidthCaps + 40 * charWidth, y, buffer, wsprintf(buffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));
                SetTextAlign(deviceContext, TA_LEFT | TA_TOP);
            }
            EndPaint(windowHandle, &paintStruct);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(windowHandle, message, wParam, lParam);
    }
    return 0;
}