#include <Windows.h>

constexpr LPTSTR WINDOW_CLASS_NAME = "Hello World Application";

LRESULT CALLBACK windowProcedure(HWND windowHandle, UINT messageCode, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int windowShowMode) {
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
    windowClass.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClass(&windowClass)) {
        MessageBox(NULL, TEXT("There was an error registering the window class"), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND windowHandle;
    windowHandle = CreateWindow(WINDOW_CLASS_NAME, TEXT(WINDOW_CLASS_NAME), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, instance, NULL);

    ShowWindow(windowHandle, windowShowMode);
    UpdateWindow(windowHandle);

    MSG message;
    while (GetMessage(&message, windowHandle, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return message.wParam;
}

LRESULT CALLBACK windowProcedure(HWND windowHandle, UINT messageCode, WPARAM wParam, LPARAM lParam) {
    HDC deviceContext;
    PAINTSTRUCT paintStruct;
    RECT rectangle;
    switch (messageCode) {
        case WM_CREATE:
            // Do nothing
            return 0;
        case WM_PAINT:
            deviceContext = BeginPaint(windowHandle, &paintStruct);
            GetClientRect(windowHandle, &rectangle);
            DrawText(deviceContext, TEXT("Hello, World!"), -1, &rectangle, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint(windowHandle, &paintStruct);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            //TODO Implement default case for windowProcedure
            break;
    }
    return DefWindowProc(windowHandle, messageCode, wParam, lParam);
}