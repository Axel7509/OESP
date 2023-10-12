#pragma once

#define OnMenuClicked1		1
#define OnMenuClicked2		2
#define OnMenuClicked3		3
#define OnExitSoftware		4


#define OnClear				5
#define OnRead				6

#define OnSave				7
#define OnLoad				8

#define TextBufferSize		1024
char Buffer[TextBufferSize];

HWND hEditControl;
HWND hStaticControl;
HWND hErrorControl;
int readChars;

const char* filePath = "D:\\OSiSP\\libs\\output.txt";

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

void MainWndAddMenus(HWND hwnd);
void MainWndAddWidgets(HWND hwnd);
void SaveData(LPCSTR path);
void LoadData(LPCSTR path);