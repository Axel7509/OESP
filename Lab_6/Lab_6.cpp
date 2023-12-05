#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


HANDLE hCalculationMutex; // Мьютекс для синхронизации доступа к переменной-результату
HANDLE hCalculationComplete; // Событие для сигнализации о завершении вычислений
DWORD calculationResult; // Переменная для хранения результата вычислений

DWORD WINAPI FactorialThread(LPVOID param)
{
    int n = *((int*)param);

    DWORD result = 1;
    for (int i = 1; i <= n; i++)
    {
        result *= i;
    }

    // Захват мьютекса перед записью результата
    WaitForSingleObject(hCalculationMutex, INFINITE);

    calculationResult = result;

    // Освобождение мьютекса и сигнализация о завершении вычислений
    ReleaseMutex(hCalculationMutex);
    SetEvent(hCalculationComplete);

    return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        
        CreateWindow(TEXT("BUTTON"), TEXT("Calculate"), WS_CHILD | WS_VISIBLE, 10, 10, 100, 30, hwnd, (HMENU)1, NULL, NULL);

      
        CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER, 10, 50, 100, 20, hwnd, (HMENU)2, NULL, NULL);

    
        CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY, 10, 80, 100, 20, hwnd, (HMENU)3, NULL, NULL);

       
        hCalculationMutex = CreateMutex(NULL, FALSE, NULL);

      
        hCalculationComplete = CreateEvent(NULL, FALSE, FALSE, NULL);
    }
    return 0;

    case WM_COMMAND:
    {
        if (LOWORD(wParam) == 1)
        {
            
            HWND hInput = GetDlgItem(hwnd, 2);
            char input[10];
            GetWindowTextA(hInput, input, sizeof(input));
            int n = atoi(input);

            // Запуск вычисления в отдельном потоке
            DWORD threadId;
            HANDLE hThread = CreateThread(NULL, 0, FactorialThread, &n, 0, &threadId);

            // Ожидание завершения вычислений
            WaitForSingleObject(hCalculationComplete, INFINITE);

            // Захват мьютекса перед чтением результата
            WaitForSingleObject(hCalculationMutex, INFINITE);

            // Получение результата
            DWORD result = calculationResult;

            // Освобождение мьютекса
            ReleaseMutex(hCalculationMutex);

            // Вывод результата в текстовое поле
            HWND hOutput = GetDlgItem(hwnd, 3);
            char output[20];
            snprintf(output, sizeof(output), "%lu", result);
            SetWindowTextA(hOutput, output);

            // Закрытие дескриптора потока
            CloseHandle(hThread);
        }
    }
    return 0;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        // Закрытие мьютекса и события
        CloseHandle(hCalculationMutex);
        CloseHandle(hCalculationComplete);

        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("MyWindowClass");
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(0, TEXT("MyWindowClass"), TEXT("Multi-Threaded App"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}