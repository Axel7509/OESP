#include <windows.h>
#include "SoftwareDefinitions.h"
#include <string>
#include <thread>
#include <chrono>
#include <Pdh.h>

#define ID_TIMER 1
#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 200



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_HAND), hInst, LoadIcon(NULL, IDI_ERROR),
		L"MainWndClass", SoftwareMainProcedure);
	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }

	CreateWindow(L"MainWndClass", L"Lab_4 window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 550, 250, NULL, NULL, NULL, NULL);

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

			cpuLoad = GetCPULoadPercentage();

			SetWindowTextA(hEditControl, (" cpuload: " + std::to_string(cpuLoad)).c_str());

			break;
		case OnExitSoftware:
			PostQuitMessage(0);
			break;
		}

		break;
	case WM_CREATE:
		MainWndAddMenus(hWnd);
		MainWndAddWidgets(hWnd);
		SetTimer(hWnd, ID_TIMER, 1000, NULL); // Устанавливаем таймер на 1 секунду
		break;
	case WM_TIMER:
		if (wp == ID_TIMER)
		{
			double cpuLoadPercentage = GetCPULoadPercentage() + 10;

			// Обновляем заголовок окна с текущей загрузкой процессора
			char title[50];
			sprintf_s(title, "CPU Load: %.2f%%", cpuLoadPercentage);
			SetWindowTextA(hEditControl, title);
		}
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

	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, OnExitSoftware, L"Exit");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");
	AppendMenu(RootMenu, MF_STRING, (UINT_PTR)SubMenu, L"Help");

	SetMenu(hwnd, RootMenu);
}

void MainWndAddWidgets(HWND hWnd) {

	hStaticControl = CreateWindowA("static", "Main Window!", WS_VISIBLE | WS_CHILD | ES_CENTER, 200, 5, 100, 30, hWnd, NULL, NULL, NULL);

	hEditControl = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, 40, 470, 120, hWnd, NULL, NULL, NULL);

	//CreateWindowA("button", "Calculate", WS_VISIBLE | WS_CHILD | ES_CENTER, 130, 200, 100, 50, hWnd, (HMENU)OnCalculate, NULL, NULL);

}


double GetCPULoadPercentage()
{
	FILETIME idleTime, kernelTime, userTime;
	
	if (GetSystemTimes(&idleTime, &kernelTime, &userTime))
	{
		ULONGLONG idleTimeDiff = SubtractTimes(idleTime, lastIdleTime);
		ULONGLONG kernelTimeDiff = SubtractTimes(kernelTime, lastKernelTime);
		ULONGLONG userTimeDiff = SubtractTimes(userTime, lastUserTime);

		ULONGLONG totalTimeDiff = kernelTimeDiff + userTimeDiff;
		ULONGLONG idleTimePercentage = (idleTimeDiff * 100) / totalTimeDiff;
		ULONGLONG cpuLoadPercentage = 100 - idleTimePercentage;

		lastIdleTime = idleTime;
		lastKernelTime = kernelTime;
		lastUserTime = userTime;

		return static_cast<double>(cpuLoadPercentage);
	}

	return 0.0;
}

// Вспомогательная функция для вычисления разницы между двумя FILETIME значениями
ULONGLONG SubtractTimes(const FILETIME& ftA, const FILETIME& ftB)
{
	LARGE_INTEGER a, b;
	a.LowPart = ftA.dwLowDateTime;
	a.HighPart = ftA.dwHighDateTime;
	b.LowPart = ftB.dwLowDateTime;
	b.HighPart = ftB.dwHighDateTime;
	return a.QuadPart - b.QuadPart;
}