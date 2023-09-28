#pragma once

#define OnMenuClicked1		1
#define OnMenuClicked2		2
#define OnMenuClicked3		3
#define OnExitSoftware		4
#define OnClear				5
#define OnRead				6

#define TextBufferSize		256
char Buffer[TextBufferSize];

HWND hEditControl;
HWND hStaticControl;
int readChars;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

void MainWndAddMenus(HWND hwnd);
void MainWndAddWidgets(HWND hwnd);