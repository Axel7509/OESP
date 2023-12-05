#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "SoftwareDefinitions.h"
#include <string>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
//#include <iphlpapi.h>



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_HAND), hInst, LoadIcon(NULL, IDI_ERROR),
		L"MainWndClass", SoftwareMainProcedure);
	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }

	CreateWindow(L"MainWndClass", L"Lab_3 window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 750, 500, NULL, NULL, NULL, NULL);

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
		case OnSave:
			SaveData("D:\\OSiSP\\libs\\output.txt");
			break;
		case OnLoad:
			LoadData("D:\\OSiSP\\Lab_3.docx");
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

	AppendMenu(SubMenu, MF_STRING, OnClear, L"Clear");
	AppendMenu(SubMenu, MF_STRING, OnSave, L"Save");
	AppendMenu(SubMenu, MF_STRING, OnLoad, L"Load");
	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, OnExitSoftware, L"Exit");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");
	AppendMenu(RootMenu, MF_STRING, (UINT_PTR)SubMenu, L"Help");

	SetMenu(hwnd, RootMenu);
}

void MainWndAddWidgets(HWND hWnd) {

	hStaticControl = CreateWindowA("static", "Main Window!", WS_VISIBLE | WS_CHILD | ES_CENTER, 200, 5, 100, 30, hWnd, NULL, NULL, NULL);

	hEditControl = CreateWindowA("edit", "Enter text!", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, 40, 470, 320, hWnd, NULL, NULL, NULL);
	hErrorControl = CreateWindowA("static", "", WS_VISIBLE | WS_CHILD | ES_CENTER, 500, 50, 100, 30, hWnd, NULL, NULL, NULL);
	CreateWindowA("button", "Clear", WS_VISIBLE | WS_CHILD | ES_CENTER, 130, 5, 50, 30, hWnd, (HMENU)OnClear, NULL, NULL);
	CreateWindowA("button", "Read", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 5, 120, 30, hWnd, (HMENU)OnRead, NULL, NULL);
}

void SaveData(LPCSTR path) {
	HANDLE SourceFile = CreateFileA(
		path,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (SourceFile == INVALID_HANDLE_VALUE)
	{
		SetWindowTextA(hErrorControl, ("Error with open file"));
		return;
	}


	DWORD saveLenth = GetWindowTextLength(hEditControl) + 1;
	char* data = new char[saveLenth];

	saveLenth = GetWindowTextA(hEditControl, data, saveLenth);

	DWORD bytesIterated;
	WriteFile(SourceFile, data, saveLenth, &bytesIterated, NULL);

	CloseHandle(SourceFile);
	delete[] data;
	SetWindowTextA(hErrorControl, ("File save"));
}


void LoadData(LPCSTR path) {
	HANDLE hFile = CreateFileA(
		path,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	DWORD bytesIterated;
	ReadFile(hFile, Buffer, TextBufferSize, &bytesIterated, NULL);

	SetWindowTextA(hEditControl, Buffer);
	CloseHandle(hFile);
	SetWindowTextA(hErrorControl, ("File open"));
}