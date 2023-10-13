#pragma once

#define OnMenuClicked1		1
#define OnMenuClicked2		2
#define OnMenuClicked3		3
#define OnExitSoftware		4
#define OnCalculate			5

FILETIME lastIdleTime = { 0 };
FILETIME lastKernelTime = { 0 };
FILETIME lastUserTime = { 0 };

HWND hEditControl;
HWND hStaticControl;
double cpuLoad;
double cpuusage;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

void MainWndAddMenus(HWND hwnd);
void MainWndAddWidgets(HWND hwnd);

ULONGLONG SubtractTimes(const FILETIME& ftA, const FILETIME& ftB);
double GetCPULoadPercentage();