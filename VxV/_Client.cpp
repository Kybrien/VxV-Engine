#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <mutex>
#include "ChatWindow.hpp"

#pragma comment(lib, "Ws2_32.lib")

std::mutex mutex;

void receiveMessages(SOCKET clientSocket) {
    char message[1024];
    int iResult;

    while (true) {
        iResult = recv(clientSocket, message, sizeof(message), 0);
        if (iResult > 0) {
            message[iResult] = '\0';
            std::lock_guard<std::mutex> lock(mutex);
            std::cout << "Serveur : " << message << std::endl;
        }
        else if (iResult == 0) {
            std::lock_guard<std::mutex> lock(mutex);
            std::cerr << "Connexion au serveur ferm�e\n";
            break;
        }
        else {
            std::lock_guard<std::mutex> lock(mutex);
            std::cerr << "Erreur lors de la r�ception du message: " << WSAGetLastError() << std::endl;
            break;
        }
    }
}

int initClient() {
    WSADATA wsaData;
    SOCKET clientSocket;
    SOCKADDR_IN serverAddr;
    int iResult;
    ChatWindow chatWindow;


    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "Erreur lors de l'initialisation de Winsock: " << iResult << std::endl;
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur lors de la cr�ation du socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("10.3.102.50");

    iResult = connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "Erreur lors de la connexion au serveur: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    //std::cout << "Connect� au serveur de chat\n";
    chatWindow.addMessage("Connect� au serveur de chat\n");
    char message[1024];
    std::thread receiveThread([&]() {
        while (true) {
            iResult = recv(clientSocket, message, sizeof(message), 0);
            if (iResult > 0) {
                message[iResult] = '\0';
                chatWindow.addMessage(std::string("Serveur : ") + message);
            }
            else if (iResult == 0) {
                chatWindow.addMessage("Connexion au serveur ferm�e\n");
                break;
            }
            else {
                chatWindow.addMessage("Erreur lors de la r�ception du message: " + std::to_string(WSAGetLastError()) + "\n");
                break;
            }
        }
        });

    while (true) {
        //std::cout << "Vous : " << std::endl;
        //std::cin.getline(message, sizeof(message));
        chatWindow.addMessage(std::string("Vous : ") + message);
        chatWindow.sendMessage(message);
        // Envoi du message au serveur
        iResult = send(clientSocket, message, strlen(message), 0);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "Erreur lors de l'envoi du message: " << WSAGetLastError() << std::endl;
            break;
        }
        chatWindow.Draw();
    }


    closesocket(clientSocket);
    WSACleanup();

    receiveThread.join();

    return 0;
}