#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr;

    // Initialisation de Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "Erreur lors de l'initialisation de Winsock: " << iResult << std::endl;
        return 1;
    }

    // Création du socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur lors de la création du socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Configuration de l'adresse et du port du serveur
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(12345); // Port arbitraire

    // Liaison du socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur lors de la liaison du socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Mise en écoute des connexions entrantes
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Erreur lors de la mise en écoute du socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Le serveur de chat est en écoute...\n";

    // Acceptation des connexions entrantes
    SOCKET clientSocket1 = accept(serverSocket, NULL, NULL);
    std::cout << "Client 1 connecté\n";

    SOCKET clientSocket2 = accept(serverSocket, NULL, NULL);
    std::cout << "Client 2 connecté\n";

    // Boucle de chat
    char buffer[1024];
    while (true) {
        // Recevoir le message du client 1
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket1, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Erreur lors de la réception du message du client 1: " << WSAGetLastError() << std::endl;
            break;
        }
        std::cout << "Client 1 : " << buffer << std::endl;

        // Envoyer le message du client 1 au client 2
        send(clientSocket2, buffer, bytesReceived, 0);

        // Recevoir le message du client 2
        memset(buffer, 0, sizeof(buffer));
        bytesReceived = recv(clientSocket2, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Erreur lors de la réception du message du client 2: " << WSAGetLastError() << std::endl;
            break;
        }
        std::cout << "Client 2 : " << buffer << std::endl;

        // Envoyer le message du client 2 au client 1
        send(clientSocket1, buffer, bytesReceived, 0);
    }

    // Fermeture des sockets
    closesocket(clientSocket1);
    closesocket(clientSocket2);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
