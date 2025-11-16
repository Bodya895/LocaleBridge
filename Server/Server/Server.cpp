#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include<vector>
#include<mutex>
#include<string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

vector<SOCKET> clientSockets;
mutex clientMutex;

DWORD WINAPI ClientHandler(LPVOID lpParam);

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
    cout << "Сервер прослуховує порт 5000. Очікування клієнтів..." << endl;

    while (true) {
        sockaddr_in client{};
        int clientSize = sizeof(client);
        SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);

        if (clientSocket == INVALID_SOCKET) {
            cout << "Помилка прийняття з'єднання." << endl;
            continue;
        }

        clientMutex.lock();
        clientSockets.push_back(clientSocket);
        clientMutex.unlock();

        HANDLE thread = CreateThread(NULL, 0, ClientHandler, (LPVOID)clientSocket, 0, NULL);

        if (thread != NULL) {
            CloseHandle(thread);
        }
    }

    const int cleanupResult = WSACleanup();
    if (cleanupResult != 0) {
        cout << "Помилка очищення ресурсів!" << endl;
    }

    return 0;
}

DWORD WINAPI ClientHandler(LPVOID lpParam) {
    SOCKET clientSocket = (SOCKET)lpParam;
    char buf[4096];
    int bytesReceived;
    string clientIP;
    sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    if (getpeername(clientSocket, (sockaddr*)&client_addr, &client_addr_size) == 0) {
        clientIP = inet_ntoa(client_addr.sin_addr);
    }
    else {
        clientIP = "Невідомий клієнт";
    }

    cout << "'" << clientIP << "' Клієнта підключено." << endl;

    while (true) {
        bytesReceived = recv(clientSocket, buf, sizeof(buf) - 1, 0);

        if (bytesReceived <= 0) {
            break;
        }

        buf[bytesReceived] = '\0';
        string receivedMessage = buf;
        string broadcastMessage = "[" + clientIP + "] : " + receivedMessage;

        cout << ">> " << broadcastMessage << endl;

        clientMutex.lock();

        for (SOCKET otherSocket : clientSockets) {
            if (otherSocket != clientSocket) {
                send(otherSocket, broadcastMessage.c_str(), (int)broadcastMessage.length(), 0);
            }
        }

        clientMutex.unlock();
    }

    clientMutex.lock();

    for (auto it = clientSockets.begin(); it != clientSockets.end(); ++it) {
        if (*it == clientSocket) {
            clientSockets.erase(it);
            break;
        }
    }

    clientMutex.unlock();

    closesocket(clientSocket);

    cout << "'" << clientIP << "' Клієнт відключився. З'єднання закрито." << endl;

    return 0;
}