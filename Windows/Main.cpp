#include <cstdio>
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdshow) {
	wchar_t buffer[100];
	wsprintf(buffer, L"The result of %i + %i is %i", 5, 3, 5 + 3);
	MessageBox(NULL, buffer, TEXT("MessageBox"), MB_OK | MB_ICONEXCLAMATION);
	return 0;
}
