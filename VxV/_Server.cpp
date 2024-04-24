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

void handleClient(SOCKET clientSocket) {
    char message[1024];
    int iResult;

    // Demander et recevoir le pseudonyme du client
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

    while (true) {
        iResult = recv(clientSocket, message, sizeof(message), 0);
        if (iResult > 0) {
            message[iResult] = '\0';
            // Diffuser le message à tous les autres clients
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                for (const auto& otherClient : clients) {
                    if (otherClient.socket != clientSocket) {
                        send(otherClient.socket, message, iResult, 0);
                    }
                }
            }
        }
        else if (iResult == 0) {
            // Supprimer le client de la liste des clients connectés
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                for (auto it = clients.begin(); it != clients.end(); ++it) {
                    if (it->socket == clientSocket) {
                        clients.erase(it);
                        break;
                    }
                }
            }
            std::cerr << "Connexion au client fermée\n";
            break;
        }
        else {
            std::cerr << "Erreur lors de la réception du message: " << WSAGetLastError() << std::endl;
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
        std::cerr << "Erreur lors de la création du socket: " << WSAGetLastError() << std::endl;
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
        std::cerr << "Erreur lors de la mise en écoute du socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Le serveur de chat est en écoute...\n";

    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        std::cout << "Client connecté\n";

        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach(); // Détacher le thread pour qu'il se termine automatiquement

        Sleep(100); // Attente courte pour éviter les problèmes de concurrence
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
