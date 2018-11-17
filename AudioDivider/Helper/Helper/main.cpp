#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	::MessageBox(NULL, lpCmdLine, "LoadLibrary Name", MB_OK | MB_ICONINFORMATION);
	return (int)&LoadLibraryA;
}