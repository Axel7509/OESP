#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "SoftwareDefinitions.h"
#include <string>


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_HAND), hInst, LoadIcon(NULL, IDI_ERROR),
		L"MainWndClass", SoftwareMainProcedure);
	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }

	CreateWindow(L"MainWndClass", L"Lab_1 window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 250, NULL, NULL, NULL, NULL);

	MSG msg = { };
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
  
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
	WNDCLASS NWC = { 0 };
	
	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_COMMAND:

		switch (wp) {
		case OnMenuClicked1:
			MessageBoxA(hWnd, "menu 1 was klicked", "Menu worked", MB_OK);
			break;
		case OnClear:
			SetWindowTextA(hEditControl, "");
			break;
		case OnRead:
			readChars = GetWindowTextA(hEditControl, Buffer, TextBufferSize);
			//SetWindowTextA(hStaticControl, Buffer);
			SetWindowTextA(hStaticControl, ("Symbols read: " + std::to_string(readChars)).c_str());
			break; 
		case OnExitSoftware:
			PostQuitMessage(0);
			break;
		}
		
		break;
	case WM_CREATE:
		MainWndAddMenus(hWnd);
		MainWndAddWidgets(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: 
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

void MainWndAddMenus(HWND hwnd) {
	HMENU RootMenu = CreateMenu();
	HMENU SubMenu = CreateMenu();
	//HMENU SubActionMenu = CreateMenu();

	//AppendMenu(SubActionMenu, MF_STRING, OnMenuClicked3, L"Menu 3");

	//AppendMenu(SubMenu, MF_POPUP, (UINT_PTR)SubActionMenu, L"Action");
	AppendMenu(SubMenu, MF_STRING, OnClear, L"Clear");
	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, OnExitSoftware, L"Exit");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");
	AppendMenu(RootMenu, MF_STRING, (UINT_PTR)SubMenu, L"Help");

	SetMenu(hwnd, RootMenu);
}

void MainWndAddWidgets(HWND hWnd) {

	hStaticControl = CreateWindowA("static", "Main Window!", WS_VISIBLE | WS_CHILD | ES_CENTER, 200, 5, 100, 30, hWnd, NULL, NULL, NULL);

	hEditControl = CreateWindowA("edit", "Enter text!", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, 40, 470, 120, hWnd, NULL, NULL, NULL);

	CreateWindowA("button", "Clear", WS_VISIBLE | WS_CHILD | ES_CENTER, 130, 5, 50, 30, hWnd, (HMENU)OnClear , NULL, NULL);
	CreateWindowA("button", "Read", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 5, 120, 30, hWnd, (HMENU)OnRead, NULL, NULL);
}