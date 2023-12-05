#include <windows.h>
#include "SoftwareDefinitions.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <Pdh.h>

#include <cstdio>


#define ID_TIMER 1
#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 200

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_HAND), hInst, LoadIcon(NULL, IDI_ERROR),
		L"MainWndClass", SoftwareMainProcedure);
	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }

	CreateWindow(L"MainWndClass", L"Lab_4 window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 550, 450, NULL, NULL, NULL, NULL);

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
		case OnCalculate:

			break;
		case OnExitSoftware:
			PostQuitMessage(0);
			break;
		}

		break;
	case WM_CREATE:
		MainWndAddMenus(hWnd);
		MainWndAddWidgets(hWnd);

		
		AuditRegistryChanges(hWnd);

		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

void MainWndAddMenus(HWND hwnd) {
	HMENU RootMenu = CreateMenu();
	HMENU SubMenu = CreateMenu();
	//HMENU SubActionMenu = CreateMenu();

	//AppendMenu(SubActionMenu, MF_STRING, OnMenuClicked3, L"Menu 3");

	//AppendMenu(SubMenu, MF_POPUP, (UINT_PTR)SubActionMenu, L"Action");

	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, OnExitSoftware, L"Exit");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");
	AppendMenu(RootMenu, MF_STRING, (UINT_PTR)SubMenu, L"Help");

	SetMenu(hwnd, RootMenu);
}

void MainWndAddWidgets(HWND hWnd) {

	hStaticControl = CreateWindowA("static", "Main Window!", WS_VISIBLE | WS_CHILD | ES_CENTER, 200, 5, 100, 30, hWnd, NULL, NULL, NULL);

	hEditControl = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, 40, 470, 60, hWnd, NULL, NULL, NULL);

	//CreateWindowA("button", "Calculate", WS_VISIBLE | WS_CHILD | ES_CENTER, 130, 200, 100, 50, hWnd, (HMENU)OnCalculate, NULL, NULL);
	
	/*g_hTextBox = CreateWindowA("edit", "EDIT",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		5, 110, 470, 280, hWnd, NULL, NULL, NULL);*/
}

void AuditRegistryChanges(HWND hwnd)
{
	// Open the registry key to monitor changes
	HKEY hKey;
	DWORD result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion", 0, KEY_NOTIFY | KEY_READ, &hKey);
	if (result != ERROR_SUCCESS)
	{
		std::cout << "Error opening registry key" << std::endl;
		return;
	}

	// Create an event for waiting for registry changes
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hEvent == NULL)
	{
		std::cout << "Error creating event" << std::endl;
		RegCloseKey(hKey);
		return;
	}

	// Set the event to interrupt the waiting
	result = RegNotifyChangeKeyValue(hKey, TRUE, REG_NOTIFY_CHANGE_LAST_SET, hEvent, TRUE);
	if (result != ERROR_SUCCESS)
	{
		std::cout << "Error setting event for interruption" << std::endl;
		CloseHandle(hEvent);
		RegCloseKey(hKey);
		return;
	}

	// Registry change processing
	while (true)
	{
		// Wait for registry changes
		DWORD dwWait = WaitForSingleObject(hEvent, INFINITE);
		if (dwWait != WAIT_OBJECT_0)
		{
			std::cout << "Error waiting for registry changes" << std::endl;
			continue;
		}

		// Get information about the changes
		DWORD valueType;
		BYTE buffer[1024];
		DWORD bufferSize = sizeof(buffer);
		result = RegQueryValueEx(hKey, L"ProductName", NULL, &valueType, buffer, &bufferSize);
		if (result == ERROR_SUCCESS)
		{
			// Convert the buffer to a string
			std::wstring productName(reinterpret_cast<wchar_t*>(buffer), bufferSize / sizeof(wchar_t));

			// Output the change information
			std::wcout << L"Registry change: " << productName << std::endl;

			// Display the information in a message box
			MessageBox(hwnd, productName.c_str(), L"Registry Change", MB_OK);
		}

		// Reset the event for the next change
		result = RegNotifyChangeKeyValue(hKey, TRUE, REG_NOTIFY_CHANGE_LAST_SET, hEvent, TRUE);
		if (result != ERROR_SUCCESS)
		{
			std::cout << "Error resetting event for next change" << std::endl;
			break;
		}
	}

	// Close the registry key and free resources
	RegCloseKey(hKey);
	CloseHandle(hEvent);
}

// Функция для добавления информации в реестр
void WriteToRegistry()
{
	while (true)
	{
		std::wstring info = L"Info";

		HKEY hKey;
		DWORD result = RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_KEY_PATH, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
		if (result == ERROR_SUCCESS)
		{
			result = RegSetValueEx(hKey, REGISTRY_VALUE_NAME, 0, REG_SZ, reinterpret_cast<const BYTE*>(info.c_str()), (info.length() + 1) * sizeof(wchar_t));
			RegCloseKey(hKey);
		}

		// Пауза в 10 секунд
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}
