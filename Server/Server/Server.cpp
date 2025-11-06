#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    WSADATA wsaData{};
    const int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (startupResult != 0) {
        cout << "Помилка ініціалізації Winsock!" << endl;
        return 0;
    }
    cout << "Winsock успішно ініціалізовано." << endl;

    SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listeningSocket == INVALID_SOCKET) {
        cout << "Помилка створення сокета!" << endl;
        WSACleanup();
        return 0;
    }
    cout << "Сокет успішно створено." << endl;

    sockaddr_in serverHint{};
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(5000);
    serverHint.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(listeningSocket, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
        cout << "Помилка прив'язки сокета до стурктури." << endl;
        closesocket(listeningSocket);
        WSACleanup();
        return 0;
    }
    cout << "Прив'язка сокета до структури успішна!" << endl;
    
    if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Помилка прослуховування!" << endl;
        closesocket(listeningSocket);
        WSACleanup();
        return 0;
    }
    cout << "Сервер прослуховує порт 5000. Очікування клієнта..." << endl;

    sockaddr_in client{};
    int clientSize = sizeof(client);
    SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);
    closesocket(listeningSocket);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Помилка прийняття з'єднання!" << endl;
        WSACleanup();
        return 0;
    }
    cout << "Клієнта підключено." << endl;

    char buf[4096];
    int bytesReceived = recv(clientSocket, buf, 4096 - 1, 0);
    if (bytesReceived > 0) {
        buf[bytesReceived] = '\0';
        cout << "Повідомлення від клієнта: " << buf << endl;
        string response = "Сервер отримав ваше повідомлення. Успіх!";
        send(clientSocket, response.c_str(), response.size() + 1, 0);
    }
    else if (bytesReceived == 0) {
        cout << "Клієнт відключився" << endl;
    }
    else {
        cout << "Помилка!" << endl;
    }
    closesocket(clientSocket);
    cout << "З'єднання закрито." << endl;

    const int cleanupResult = WSACleanup();
    if (cleanupResult != 0) {
        cout << "Помилка очищення ресурсів!" << endl;
    }
    return 0;
}