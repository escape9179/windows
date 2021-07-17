#include <cstdio>
#include <Windows.h>

int CDECL MessageBoxPrintf(TCHAR const *szCaption, TCHAR const *szFormat, ...);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdshow) {
	int cxScreen, cyScreen;
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);
	MessageBoxPrintf(TEXT("Screen Size"), TEXT("The screen size is %i pixels wide by %i pixels high."), cxScreen, cyScreen);
	return 0;
}

int CDECL MessageBoxPrintf(TCHAR const *szCaption, TCHAR const *szFormat, ...) {
	va_list args;

	// args = (char *) &szFormat + sizeof(szFormat)
	va_start(args, szFormat);
	wchar_t buffer[128];

	// Last argument points to argument list
	_vsnwprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), szFormat, args);

	// Zeroes out args for no good reason
	va_end(args);
	return MessageBox(NULL, buffer, szCaption, MB_OK | MB_ICONEXCLAMATION);
}
