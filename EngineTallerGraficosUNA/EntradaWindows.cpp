#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	extern int main(int argc, char** argv);
	return main(0, nullptr);
}