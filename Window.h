//
// Created by tr3lo on 4/11/2022.
//

#ifndef WINDOWSPROGRAMMING_WINDOW_H
#define WINDOWSPROGRAMMING_WINDOW_H

#include <windows.h>

class Window {
    char const *caption;
    int x, y, width, height;
    HWND hwnd;
public:
    Window(HINSTANCE hInstance, char const *caption, int x, int y, int width, int height);
    void show();
    static LRESULT defaultWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif //WINDOWSPROGRAMMING_WINDOW_H
