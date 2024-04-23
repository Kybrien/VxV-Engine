#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once

#include <deque>
#include <string>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <imgui.h>

#pragma comment(lib, "Ws2_32.lib")

struct ChatWindow {
    std::deque<std::string> messages;
    char inputBuf[256];
    SOCKET clientSocket;
    std::thread receiveThread;
    std::mutex mutex;

    ChatWindow() {
        memset(inputBuf, 0, sizeof(inputBuf));
        initNetwork();
    }

    ~ChatWindow() {
        closeNetwork();
    }

    void initNetwork() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            addMessage("Winsock initialization failed.");
            return;
        }

        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket == INVALID_SOCKET) {
            WSACleanup();
            addMessage("Socket creation failed.");
            return;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345);
        InetPton(AF_INET, TEXT("10.3.102.41"), &serverAddr.sin_addr);

        if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(clientSocket);
            WSACleanup();
            addMessage("Connection failed.");
            return;
        }

        receiveThread = std::thread(&ChatWindow::receiveMessages, this);
        receiveThread.detach();  // It's detached to prevent blocking.
    }

    void closeNetwork() {
        closesocket(clientSocket);
        WSACleanup();
        if (receiveThread.joinable()) {
            receiveThread.join();
        }
    }

    void receiveMessages() {
        char message[1024];
        while (true) {
            int iResult = recv(clientSocket, message, sizeof(message) -1, 0);
            if (iResult > 0) {
                message[iResult] = '\0';
                addMessage("Server: " + std::string(message));
            }
            else {
                if (iResult == 0) {
                    addMessage("Connection closed by server.");
                }
                else {
                    addMessage("Error receiving message.");
                }
                break;
            }
        }
    }

    void addMessage(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);
        messages.push_back(message);
    }

    void sendMessage(const std::string& message) {
        std::cout << "Sending message: " << message << std::endl;
        int iResult = send(clientSocket, message.c_str(), message.length(), 0);
        if (iResult == SOCKET_ERROR) {
            addMessage("Error sending message: " + std::to_string(WSAGetLastError()));
        }
        else {
            // Ajout du message envoyé à l'affichage du chat avec une indication que vous l'avez envoyé
            addMessage("Vous: " + message);
        }
    }

    void Draw() {
        ImGui::Begin("Chat Window");

        // Zone pour afficher les messages avec une barre de défilement
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar)) {
            for (const auto& message : messages) {
                ImGui::TextUnformatted(message.c_str());
            }
            // Défilement automatique pour voir le dernier message
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                ImGui::SetScrollHereY(1.0f);
            }
        }
        ImGui::EndChild();

        // Zone de texte pour l'entrée des messages
        bool reclaim_focus = false;
        if (ImGui::InputText("Input", inputBuf, sizeof(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
            std::string message = inputBuf;
            if (!message.empty()) {
                sendMessage(message);
                reclaim_focus = true;
            }
            memset(inputBuf, 0, sizeof(inputBuf)); // Clear the input text field after sending
        }

        // Rétablir le focus sur la zone de texte après l'envoi d'un message
        if (reclaim_focus) {
            ImGui::SetKeyboardFocusHere(-1);
        }

        ImGui::End();
    }
};
