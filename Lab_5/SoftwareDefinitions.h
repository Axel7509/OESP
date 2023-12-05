#pragma once

#define OnMenuClicked1		1
#define OnMenuClicked2		2
#define OnMenuClicked3		3
#define OnExitSoftware		4
#define OnCalculate			5
#define IDC_STATUS_LABEL	101

// Путь к ключу реестра для записи
#define REGISTRY_KEY_PATH L"Software\\MyApp"
// Имя значения реестра для записи
#define REGISTRY_VALUE_NAME L"Info"

HWND hEditControl;
HWND hStaticControl;
HWND g_hTextBox;
HWND hStatusLabel;

double cpuLoad;
double cpuusage;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

void MainWndAddMenus(HWND hwnd);
void MainWndAddWidgets(HWND hwnd);

void AuditRegistryChanges(HWND hwnd);
void WriteToRegistry();