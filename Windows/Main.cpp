#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdshow) {
	MessageBox(NULL, TEXT("Hello Windows 98!"), TEXT("HelloMsg"), MB_YESNOCANCEL | MB_DEFBUTTON2 | MB_ICONINFORMATION);
	return 0;
}
