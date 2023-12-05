#include <Windows.h>
#include <iostream>

#define BUFFER_SIZE 1024
#define PIPE_NAME L"\\\\.\\pipe\\MyNamedPipe"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
    {
        // Создание именованного канала (в серверном окне)
        HANDLE hPipe = CreateNamedPipe(
            PIPE_NAME,              // Имя канала
            PIPE_ACCESS_DUPLEX,     // Дуплексный доступ
            PIPE_TYPE_MESSAGE |     // Режим сообщений
            PIPE_READMODE_MESSAGE | // Режим чтения сообщений
            PIPE_WAIT,              // Режим блокировки
            PIPE_UNLIMITED_INSTANCES,   // Количество экземпляров
            BUFFER_SIZE,            // Размер входного/выходного буфера
            BUFFER_SIZE,            // Размер выходного/входного буфера
            0,                       // Время ожидания соединения (0 - неограниченное время)
            NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            MessageBox(hwnd, L"Failed to create named pipe.", L"Error", MB_OK | MB_ICONERROR);
            return -1;
        }

        MessageBox(hwnd, L"Waiting for client connection...", L"Server", MB_OK | MB_ICONINFORMATION);

        // Ожидание подключения клиента
        BOOL isConnected = ConnectNamedPipe(hPipe, NULL);
        if (!isConnected) {
            MessageBox(hwnd, L"Failed to connect to client.", L"Error", MB_OK | MB_ICONERROR);
            CloseHandle(hPipe);
            return -1;
        }

        MessageBox(hwnd, L"Client connected.", L"Server", MB_OK | MB_ICONINFORMATION);

        // Чтение данных от клиента
        char buffer[BUFFER_SIZE];
        DWORD bytesRead;
        BOOL success = ReadFile(hPipe, buffer, BUFFER_SIZE, &bytesRead, NULL);
        if (!success) {
            MessageBox(hwnd, L"Failed to read data from client.", L"Error", MB_OK | MB_ICONERROR);
        }
        else {
            buffer[bytesRead] = '\0';
            std::string receivedData(buffer);
            std::wstring message = L"Received data from client:\n\n" + std::wstring(receivedData.begin(), receivedData.end());
            MessageBox(hwnd, message.c_str(), L"Server", MB_OK | MB_ICONINFORMATION);
        }

        // Закрытие канала
        FlushFileBuffers(hPipe);
        DisconnectNamedPipe(hPipe);
        CloseHandle(hPipe);

        DestroyWindow(hwnd);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                             
        CLASS_NAME,                    
        L"Named Pipe Server",           
        WS_OVERLAPPEDWINDOW,            
        CW_USEDEFAULT, CW_USEDEFAULT,   
        500, 300,                       
        NULL,                          
        NULL,                          
        hInstance,                     
        NULL                      
    );

    if (hwnd == NULL) {
        MessageBox(NULL, L"Failed to create window.", L"Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}