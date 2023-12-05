#include <Windows.h>
#include <iostream>

#define BUFFER_SIZE 1024
#define PIPE_NAME L"\\\\.\\pipe\\MyNamedPipe"

HWND hwndEdit;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
        {

            hwndEdit = CreateWindowEx(
                0,                             
                L"EDIT",                       
                NULL,                          
                WS_CHILD | WS_VISIBLE |        
                WS_BORDER | ES_AUTOHSCROLL,
                10, 10,                        
                300, 25,                       
                hwnd,                           
                NULL,                           
                NULL,                         
                NULL                            
            );

            CreateWindowEx(
                0,                          
                L"BUTTON",                      
                L"Send",                        
                WS_CHILD | WS_VISIBLE,
                10, 45,                         
                70, 25,                        
                hwnd,                           
                (HMENU)1,                       
                NULL,                          
                NULL                       
            );

            break;
        }
        case WM_COMMAND:
        {
            if (LOWORD(wParam) == 1)
            {
        
                int textLength = GetWindowTextLength(hwndEdit);
                if (textLength > 0)
                {
               
                    int bufferSize = (textLength + 1) * sizeof(wchar_t);

                 
                    wchar_t* buffer = new wchar_t[bufferSize];

                    GetWindowText(hwndEdit, buffer, textLength + 1);

                    // Преобразование текста в формат const char*
                    int messageSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
                    char* message = new char[messageSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, message, messageSize, NULL, NULL);

                    // Создание и открытие именованного канала
                    HANDLE hPipe = CreateFile(
                        PIPE_NAME,                          // Имя канала
                        GENERIC_READ | GENERIC_WRITE,       // Режим доступа
                        0,                                  // Нет совместного доступа
                        NULL,                               // Атрибуты безопасности по умолчанию
                        OPEN_EXISTING,                      // Открытие существующего канала
                        0,                                  // Дополнительные флаги и атрибуты
                        NULL                                // Использование шаблона файла
                    );

                    if (hPipe != INVALID_HANDLE_VALUE)
                    {
                        // Отправка данных на сервер
                        DWORD bytesWritten;
                        BOOL success = WriteFile(hPipe, message, messageSize, &bytesWritten, NULL);
                        if (success)
                        {
                            MessageBox(hwnd, L"Data sent to server.", L"Client", MB_OK | MB_ICONINFORMATION);
                        }
                        else
                        {
                            MessageBox(hwnd, L"Failed to send data to server.", L"Error", MB_OK | MB_ICONERROR);
                        }

                        // Закрытие канала
                        CloseHandle(hPipe);
                    }
                    else
                    {
                        MessageBox(hwnd, L"Failed to connect to server.", L"Error", MB_OK | MB_ICONERROR);
                    }

                    // Очистка буферов
                    delete[] buffer;
                    delete[] message;
                }
                else
                {
                    MessageBox(hwnd, L"Please enter some text.", L"Client", MB_OK | MB_ICONWARNING);
                }
            }
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
        L"Named Pipe Client",          
        WS_OVERLAPPEDWINDOW,            
        CW_USEDEFAULT, CW_USEDEFAULT,  
        350, 120,                     
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

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}