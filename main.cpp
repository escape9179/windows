/*--------------------------------------------------
 * SYSMETS.C -- System Metrics Display Program No. 2
 */

#include <windows.h>
#include "SystemMetrics.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = TEXT("SysMets2");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    // Class styles. These styles instruct the window to be repainted
    // whenever the horizontal or vertical size of the window changes.
    wndclass.style = CS_HREDRAW | CS_VREDRAW;

    // The address of a windows procedure used for any window using this class.
    wndclass.lpfnWndProc = WndProc;

    // Used to reserve extra space in the class and window structure.
    // Can be used for own purposes.
    wndclass.cbClsExtra = 0; // ?
    wndclass.cbWndExtra = 0; // ?

    // Instance handle to the program.
    wndclass.hInstance = hInstance;

    // Sets an icon for all windows created based on this windows class.
    // To use a predefined icon set the first parameter to NULL.
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ?

    // Load a cursor used for windows based on this windows class.
    // To use a predefined cursor, set the first parameter to NULL.
    // When the cursor moves over the client area, it will become a small arrow.
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

    // Sets the background color of the client area.
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    // Specifies the window class menu. This program has no application menu
    // so it is set to NULL.
    wndclass.lpszMenuName = NULL;

    // The name of the window class. This can simply be the name of the program
    // if the program is small.
    wndclass.lpszClassName = szAppName;

    // Registers the window class. Returns false if there was a problem registering the class, such as
    // a NULL lpfnWndProc field.
    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    // Create a window. You can specify more detailed information here.
    hwnd = CreateWindow(
            szAppName, // window class name
            TEXT("The Hello Program"), // window caption (text that appears in title bar of window)
            WS_OVERLAPPEDWINDOW, // window style
            CW_USEDEFAULT, // initial x position
            CW_USEDEFAULT, // initial y position
            CW_USEDEFAULT, // initial x size (width)
            CW_USEDEFAULT, // initial y size (height)
            NULL, // parent window handle (When set to NULL
            // this window will always appear on the surface of the desktop.
            // A child window will appear on the surface of its parent window).
            NULL, // window menu handle (is set to NULL because the program has no menu.)
            hInstance, // program instance handle
            NULL // creation parameters (Set to NULL. Could point to data you may want to reference later in
            // the program
    );

    // Displays the window
    // hwnd is the handle to the window just created.
    // iCmdShow specifies how the window should be displayed (normal, minimized, maximized)
    // Possible values (SW_SHOWNORMAL, SW_SHOWMAXIMIZED, SW_SHOWMINNOACTIVE)
    ShowWindow(hwnd, iCmdShow);

    // Causes the client area to be painted.
    // It sends the windows procedure a WM_PAINT message to do this.
    UpdateWindow(hwnd);

    // Processes messages windows sends to the programs message queue.
    // GetMessage gets a message from the queue. The params are set to NULL or 0 so that
    // all messages from all windows are processed and none are filtered out.
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}