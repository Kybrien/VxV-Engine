#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")

struct ClientInfo {
    SOCKET socket;
    std::string nickname;
};

std::vector<ClientInfo> clients;
std::mutex clientsMutex;

void handleClientDisconnection(SOCKET clientSocket, const std::string& nickname) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (it->socket == clientSocket) {
            clients.erase(it);
            break;
        }
    }
    std::cerr << "Connection with " << nickname << " closed\n";
}

void handleClient(SOCKET clientSocket) {
    char message[1024];
    int iResult;

    // Recevoir le pseudonyme du client
    std::string nickname;
    iResult = recv(clientSocket, message, sizeof(message), 0);
    if (iResult > 0) {
        message[iResult] = '\0';
        nickname = message;
    }

    // Ajouter le client à la liste des clients connectés
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.push_back({ clientSocket, nickname });
    }

    // Boucle de réception des messages du client
    while (true) {
        iResult = recv(clientSocket, message, sizeof(message), 0);
        if (iResult > 0) {
            message[iResult] = '\0';
            std::string fullMessage = nickname + ": " + std::string(message);

            // Diffuser le message préfixé
            std::lock_guard<std::mutex> lock(clientsMutex);
            for (const auto& otherClient : clients) {
                if (otherClient.socket != clientSocket) {
                    send(otherClient.socket, fullMessage.c_str(), fullMessage.length(), 0);
                }
            }
        }
        else {
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                for (auto it = clients.begin(); it != clients.end(); ++it) {
                    if (it->socket == clientSocket) {
                        clients.erase(it);
                        break;
                    }
                }
            }
            //std::cerr << "Erreur lors de la reception du message: " << WSAGetLastError() << std::endl;
            std::cout << nickname << " is OFFLINE" << std::endl;
            break;
        }
    }

    closesocket(clientSocket);
}


int initServ() {
    WSADATA wsaData;
    SOCKET serverSocket;
    SOCKADDR_IN serverAddr;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "Erreur lors de l'initialisation de Winsock: " << iResult << std::endl;
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur lors de la creation du socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(12345); // Port arbitraire

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur lors de la liaison du socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Erreur lors de la mise en ecoute du socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Le serveur de chat est en ecoute...\n";

    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        char nickname[1024];
        int nicknameLength = recv(clientSocket, nickname, sizeof(nickname), 0);
        if (nicknameLength > 0) {
            nickname[nicknameLength] = '\0'; // Ajoutez le caractère de fin de chaîne
        }
        std::string clientNickname = nickname;
        std::cout << "| " << clientNickname << " is ONLINE |\n";

        char joinMessage[1024] = " a rejoint le chat...";
        std::string welcomeServ = nickname + std::string(joinMessage);
        for (const auto& otherClient : clients) {
            if (otherClient.socket != clientSocket) {
                send(otherClient.socket, welcomeServ.c_str(), welcomeServ.length(), 0);
            }
        }
        

        std::thread clientThread(handleClient, clientSocket);

        clientThread.detach(); // Détacher le thread pour qu'il se termine automatiquement

        Sleep(100); // Attente courte pour éviter les problèmes de concurrence
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
