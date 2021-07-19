#include <Windows.h>

LRESULT CALLBACK windowProcedure(HWND windowHandle, UINT messageId, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand) {
    LPSTR appName = "Application Name";
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
        MessageBox(NULL, TEXT("Error registering window class"), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND windowHandle = CreateWindow(
            appName,
            TEXT("This is a window name"),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            NULL,
            NULL,
            instance,
            NULL
    );

    ShowWindow(windowHandle, showCommand);
    UpdateWindow(windowHandle);

    MSG message;
    while (GetMessage(&message, windowHandle, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return message.wParam;
}

LRESULT CALLBACK windowProcedure(HWND windowHandle, UINT messageId, WPARAM wParam, LPARAM lParam) {
    HDC deviceContext;
    PAINTSTRUCT paintStruct;
    RECT clientRect;
    switch (messageId) {
        case WM_CREATE:
            // Do nothing
            return 0;
        case WM_PAINT:
            deviceContext = BeginPaint(windowHandle, &paintStruct);
            GetClientRect(windowHandle, &clientRect);
            DrawText(deviceContext, TEXT("This is in the center of the screen"), -1, &clientRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint(windowHandle, &paintStruct);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(windowHandle, messageId, wParam, lParam);
    }
}