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
            int iResult = recv(clientSocket, message, sizeof(message), 0);
            if (iResult > 0) {
                message[iResult] = '\0';
                std::lock_guard<std::mutex> lock(mutex);
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
        int iResult = send(clientSocket, message.c_str(), message.length(), 0);
        if (iResult == SOCKET_ERROR) {
            addMessage("Error sending message.");
        }
    }

    void Draw() {
        ImGui::Begin("Chat Window");

        // Display messages
        for (const auto& message : messages) {
            ImGui::TextUnformatted(message.c_str());
        }

        // Scroll to bottom when a new message is added
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        // Text input
        bool reclaim_focus = false;
        if (ImGui::InputText("Input", inputBuf, sizeof(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
            std::string message = inputBuf;
            if (!message.empty()) {
                sendMessage(message);
                reclaim_focus = true;
            }
            memset(inputBuf, 0, sizeof(inputBuf)); // Clear the input text field
        }

        if (reclaim_focus) {
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
        }

        ImGui::End();
    }
};
