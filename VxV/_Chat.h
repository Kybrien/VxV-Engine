#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <mutex>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#pragma comment(lib, "Ws2_32.lib")

struct ChatContext {
    SOCKET clientSocket;
    ImGuiTextBuffer textBuffer;
    std::string incomingMessage;
};

void ShowChat(ChatContext* chatContext);

