#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once

#include <deque>
#include <string>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <imgui.h>
#include <cstdlib>
#include <iostream> 

#pragma comment(lib, "Ws2_32.lib")

/**
 * @struct ChatWindow
 * @brief Represents a chat window including connection handling, message sending, and receiving.
 */
struct ChatWindow {
    std::string username;
    std::deque<std::string> messages;
    char inputBuf[256];
    SOCKET clientSocket;
    std::string serverIP = "Your Ipv4 Adress";  // Default IPV4 address
    std::thread receiveThread;
    std::mutex mutex;
    bool isConnected = false;
    char ipAddressBuf[256];

    /**
   * @brief Constructor for ChatWindow.
   */
    ChatWindow() {
        memset(inputBuf, 0, sizeof(inputBuf));
        memset(ipAddressBuf, 0, sizeof(ipAddressBuf));
    }

    /**
     * @brief Destructor for ChatWindow.
     */
    ~ChatWindow() {
        closeNetwork();
    }


    /**
     * @brief Gets the local IP address of the host machine.
     * @return The local IP address as a string.
     */

    std::string get_local_ip_address() {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed: " << result << std::endl;
            return "";
        }

        char hostname[NI_MAXHOST];
        result = gethostname(hostname, NI_MAXHOST);
        if (result != 0) {
            std::cerr << "Error getting hostname: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return "";
        }

        struct addrinfo* info = nullptr;
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;  // Use IPv4
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        result = getaddrinfo(hostname, nullptr, &hints, &info);
        if (result != 0) {
            std::cerr << "Error getting address info: " << result << std::endl;
            WSACleanup();
            return "";
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &((struct sockaddr_in*)info->ai_addr)->sin_addr, ip, INET_ADDRSTRLEN);

        freeaddrinfo(info);
        WSACleanup();

        return std::string(ip);
    }

    /**
    * @brief Starts the chat server.
    */
    void startServer() {
        system("start Server.exe");
        // Get the local IP address after the server starts
        std::string ipAddress = get_local_ip_address();
        // Copy the IP address into the IP address buffer using strcpy_s
        strcpy_s(ipAddressBuf, sizeof(ipAddressBuf), ipAddress.c_str());
        // Show the IP address in the console (for debugging)
        std::cout << "Local IP Address: " << ipAddress << std::endl;
    }

    /**
   * @brief Sets up the connection to the chat server.
   * @param user The username of the client.
   * @param ip The IP address of the server.
   */
    void setupConnection(const std::string& user, const std::string& ip) {
        username = user;
        serverIP = ip;
        initNetwork();
    }

    /**
     * @brief Initializes the network connection.
     */
    void initNetwork() {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
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
        inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

        if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(clientSocket);
            WSACleanup();
            addMessage("Connection failed.");
            return;
        }
        send(clientSocket, username.c_str(), username.length(), 0); // Send username right after connecting
        receiveThread = std::thread(&ChatWindow::receiveMessages, this);
        receiveThread.detach();  // It's detached to prevent blocking.
        isConnected = true;
        send(clientSocket, username.c_str(), username.length(), 0);
    }

    /**
    * @brief Closes the network connection.
    */
    void closeNetwork() {
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
        }
        WSACleanup();
        if (receiveThread.joinable()) {
            receiveThread.join();
        }
    }

    /**
    * @brief Receives messages from the server in a separate thread.
    */
    void receiveMessages() {
        char buffer[1024];
        while (true) {
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0';
                addMessage(std::string(buffer));
            }
            else {
                if (bytesReceived == 0) {
                    addMessage("Connection closed by server.");
                }
                else {
                    addMessage("Error receiving message.");
                }
                break;
            }
        }
    }

    /**
     * @brief Adds a message to the message queue.
     * @param message The message to add.
     */
    void addMessage(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);
        messages.push_back(message);
    }

    /**
    * @brief Sends a message to the chat server.
    * @param message The message to send.
    */
    void sendMessage(const std::string& message) {
        std::string fullMessage = message;
        int bytesSent = send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0);
        if (bytesSent == SOCKET_ERROR) {
            addMessage("Error sending message: " + std::to_string(WSAGetLastError()));
        }
        else {
            addMessage("You: " + message);
        }
    }

    /**
    * @brief Draws the login interface.
    */
    void DrawLogin() {
        static char usernameBuf[256] = "";
        static char serverIPBuf[256] = "Your Ipv4 Adress";
        static bool showIPAddress = false;

        ImGui::Begin("Login");

        ImGui::InputText("Username", usernameBuf, sizeof(usernameBuf));
        ImGui::InputText("Server IP", serverIPBuf, sizeof(serverIPBuf));

        ImGui::PushID(1); // To avoid ID conflicts
        if (ImGui::Button("Host")) {
            startServer();
            showIPAddress = true;
        }
        ImGui::PopID();

        if (showIPAddress) {
            ImGui::SameLine();
            ImGui::Text("Host IP: %s", ipAddressBuf);
        }

        if (ImGui::Button("Connect")) {
            setupConnection(std::string(usernameBuf), std::string(serverIPBuf));
            if (isConnected) {
                // Successfully connected, so clear the buffers
                memset(usernameBuf, 0, sizeof(usernameBuf));  // Clear buffers after successful login
                memset(serverIPBuf, 0, sizeof(serverIPBuf));
                ImGui::End();
                return;
            }
        }

        ImGui::End();
    }

    /**
    * @brief Disconnects from the chat server.
    */
    void disconnect() {
        if (isConnected) {
            closeNetwork();
            isConnected = false;
        }
    }

    /**
    * @brief Draws the chat interface.
    */
    void Draw() {
        if (!isConnected) {
            DrawLogin();
        }
        else {
            ImGui::Begin("Chat");
            if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar)) {
                for (const auto& message : messages) {
                    ImGui::TextUnformatted(message.c_str());
                }
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                    ImGui::SetScrollHereY(1.0f);
                }
            }
            ImGui::EndChild();

            bool reclaim_focus = false;
            if (ImGui::InputText("Input", inputBuf, sizeof(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
                std::string message = inputBuf;
                if (!message.empty()) {
                    sendMessage(message);
                    reclaim_focus = true;
                }
                memset(inputBuf, 0, sizeof(inputBuf));
            }
            if (reclaim_focus) {
                ImGui::SetKeyboardFocusHere(-1);
            }

            if (ImGui::Button("Disconnect")) {
                disconnect();
            }

            ImGui::End();
        }
    }

};