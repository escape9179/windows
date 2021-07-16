#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdshow) {
	int result = MessageBox(NULL, TEXT("Hello Windows 98!"), TEXT("HelloMsg"), MB_YESNOCANCEL | MB_ICONERROR);
	if (result == IDYES) {
		MessageBox(NULL, TEXT("You clicked 'Yes'."), TEXT("Result"), MB_OK);
	} else if (result == IDNO) {
		MessageBox(NULL, TEXT("You clicked 'No'."), TEXT("Result"), MB_OK);
	} else if (result == IDCANCEL) {
		MessageBox(NULL, TEXT("You clicked 'Cancel'."), TEXT("Result"), MB_OK);
	} else {
		MessageBox(NULL, TEXT("You clicked on a non-existing button?"), TEXT("Result"), MB_OK);
	}
	return 0;
}
