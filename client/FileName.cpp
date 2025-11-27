#define WIN32_LEAN_AND_MEAN


#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <windows.h> 

#pragma comment(lib, "ws2_32.lib")

using namespace std;


DWORD WINAPI ReceiveHandler(LPVOID lpParam) {
    SOCKET clientSocket = (SOCKET)lpParam;
    char buf[4096];
    int bytesReceived;


    while (true) {
        bytesReceived = recv(clientSocket, buf, sizeof(buf) - 1, 0);


        if (bytesReceived <= 0) {
            cout << "Connection lost." << endl;
            closesocket(clientSocket);
            break;
        }


        buf[bytesReceived] = '\0';


        cout << "\r" << buf << endl;


    }
    return 0;
}


int main()
{

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    WSADATA wsaData{};
    int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (startupResult != 0) {
        cout << "Error initialization Winsock!" << endl;
        return 0;
    }
    cout << "Winsock succesful initialized." << endl;

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error socket initialization!" << endl;
        WSACleanup();
        return 0;
    }
    cout << "Soket created!" << endl;

    sockaddr_in clientHint{};
    clientHint.sin_family = AF_INET;
    clientHint.sin_port = htons(5000);
    inet_pton(AF_INET, "10.112.75.236", &clientHint.sin_addr);

    startupResult = connect(clientSocket, (sockaddr*)&clientHint, sizeof(clientHint));
    if (startupResult == SOCKET_ERROR) {
        cout << "Cant connect to server!" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 0;
    }
    cout << "Succsesful connected to server!" << endl;

    string name;
    cout << "Enter your name: ";
    cin >> name;
    int sendName = send(clientSocket, name.c_str(), (int)name.size(), 0);
    if (sendName == SOCKET_ERROR) {
        cout << "Error.Conection failed." << endl;
    }

    HANDLE thread = CreateThread(NULL, 0, ReceiveHandler, (LPVOID)clientSocket, 0, NULL);
    if (thread != NULL) {
        CloseHandle(thread);
    }


    string line;
    cout << "Enter your message: " << endl;

    while (getline(cin, line)) {
        if (line.empty()) {

            continue;
        }


        string message = line;

        int sendResult = send(clientSocket, message.c_str(), (int)message.size(), 0);


        if (sendResult == SOCKET_ERROR) {
            cout << "Error.Conection failed." << endl;
            break;
        }
    }


    closesocket(clientSocket);
    WSACleanup();

    return 0;
}