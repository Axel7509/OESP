#include <windows.h>
#include <iostream>

#define BUFFER_SIZE 1024
#define PIPE_NAME L"\\\\.\\pipe\\MyNamedPipe"

int main() {
    std::cout << "Connecting to server..." << std::endl;

    // Подключение к именованному каналу
    HANDLE hPipe = CreateFile(
        PIPE_NAME,                      // Имя канала
        GENERIC_READ | GENERIC_WRITE,   // Доступ для чтения/записи
        0,                              // Нет разделения совместного доступа
        NULL,                           // Без атрибутов защиты
        OPEN_EXISTING,                  // Открыть существующий канал
        0,                              // Без атрибутов шаблона файла
        NULL                            // Без дополнительных атрибутов
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to connect to server: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    // Запись данных на сервер
    const char* message = "Hello, server!";
    DWORD bytesWritten;
    BOOL success = WriteFile(hPipe, message, strlen(message) + 1, &bytesWritten, NULL);
    if (!success) {
        std::cout << "Failed to send data to server: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "Data sent to server." << std::endl;
    }

    // Закрытие канала
    FlushFileBuffers(hPipe);
    CloseHandle(hPipe);

    return 0;
}