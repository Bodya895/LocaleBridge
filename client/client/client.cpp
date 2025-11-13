#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;


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
    
	string line;
	cout << "Enter messages to send to server. Empty line to quit." << endl;
    while (getline(cin, line)) {
        if (line.empty()) {
            break;
        }
        
        string message = line + "\n";

        int sendResult = send(clientSocket, message.c_str(), message.size(), 0);
        if (sendResult == SOCKET_ERROR) {
            cout << "Send failed!" << endl;
            break;
        }
        
    }

    char buff[4096];

    while (true)
    {
        
		int bytesReceived = recv(clientSocket, buff, 4096, 0);
        if (bytesReceived > 0) {
			buff[bytesReceived] = '\0';
            cout << "Server: " << buff << endl;
        }
        break;
    }
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}

