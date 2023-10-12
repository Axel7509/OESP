#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "SoftwareDefinitions.h"
#include <string>


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	HINSTANCE hInstance = GetModuleHandle(NULL);
	g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookCallback, hInstance, 0);

	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_HAND), hInst, LoadIcon(NULL, IDI_ERROR),
		L"MainWndClass", SoftwareMainProcedure);
	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }

	CreateWindow(L"MainWndClass", L"Lab_1 window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 250, NULL, NULL, NULL, NULL);

	if (g_hHook == NULL)
	{
		return 1;
	}

	MSG msg = { };
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(g_hHook);

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
		case OnReadColor:
			brushRectangle = CreateSolidBrush(
			RGB(GetDlgItemInt(hWnd, IndexColorR, NULL, false),
				GetDlgItemInt(hWnd, IndexColorG, NULL, false),
				GetDlgItemInt(hWnd, IndexColorB, NULL, false)
				));
			RGB(10,255,0);

			RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);

			break;  
		case OnExitSoftware:
			PostQuitMessage(0);
			break;
		}
		
		break;
	case WM_PAINT:

		BeginPaint(hWnd, &ps);

		FillRect(ps.hdc, &windowRectangle, brushRectangle);

		EndPaint(hWnd, &ps);

		break;
	case WM_CREATE:
		MainWndAddMenus(hWnd);
		MainWndAddWidgets(hWnd);
		MainWndRect(hWnd);
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

	//hEditControl = CreateWindowA("edit", "Enter text!", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, 40, 470, 120, hWnd, NULL, NULL, NULL);
	//windowRectangle = { 5 + 470, 40, 5, 40 + 120 };

	CreateWindowA("edit", "0", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_NUMBER, 5, 170, 100, 30, hWnd, (HMENU)IndexColorR, NULL, NULL);
	CreateWindowA("edit", "0", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_NUMBER, 110, 170, 100, 30, hWnd, (HMENU)IndexColorG, NULL, NULL);
	CreateWindowA("edit", "0", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_NUMBER, 215, 170, 100, 30, hWnd, (HMENU)IndexColorB, NULL, NULL);

	//CreateWindowA("button", "Clear", WS_VISIBLE | WS_CHILD | ES_CENTER, 130, 5, 50, 30, hWnd, (HMENU)OnClear , NULL, NULL);
	CreateWindowA("button", "Set color", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 5, 120, 30, hWnd, (HMENU)OnReadColor, NULL, NULL);
}

LRESULT CALLBACK KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
	{

		KBDLLHOOKSTRUCT* pKeyboardData = (KBDLLHOOKSTRUCT*)lParam;
		DWORD keyCode = pKeyboardData->vkCode;

		// Пример: Вывод кода нажатой клавиши в консоль
		if (keyCode == VK_TAB)
		{
			// Обработка нажатия клавиши Tab
			MessageBox(NULL, L"Tab key pressed!", L"Key Pressed", MB_OK);
		}
	}

	// Передача управления следующему хуку в цепочке
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

void MainWndRect(HWND hWnd) {

	//hEditControl = CreateWindowA("edit", "Enter text!", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, 40, 470, 120, hWnd, NULL, NULL, NULL);
	windowRectangle = { 5 + 470, 40, 5, 40 + 120 };
	brushRectangle = CreateSolidBrush(RGB(10, 255, 0));
	FillRect(ps.hdc, &windowRectangle, brushRectangle);
}