#pragma once

#define OnMenuClicked1		1
#define OnMenuClicked2		2
#define OnMenuClicked3		3
#define OnExitSoftware		4
#define OnClear				5
#define OnReadColor			6

#define TextBufferSize		256
#define IndexColorR			200
#define IndexColorG			201
#define IndexColorB			202
char Buffer[TextBufferSize];

PAINTSTRUCT ps;
RECT windowRectangle;
HBRUSH brushRectangle;

HWND hEditControl;
HWND hStaticControl;

HHOOK g_hHook = NULL;

LRESULT CALLBACK KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

void MainWndAddMenus(HWND hwnd);
void MainWndAddWidgets(HWND hwnd);
void MainWndRect(HWND hwnd);