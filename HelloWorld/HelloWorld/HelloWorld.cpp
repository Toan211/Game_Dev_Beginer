#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	MessageBox(NULL,"Welcome!","Hello World",MB_OK | MB_ICONEXCLAMATION);
	return 0;
}