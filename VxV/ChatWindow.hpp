#pragma once
#include <deque>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <imgui.h>

struct ChatWindow
{
    std::deque<std::string> messages;
    char inputBuf[256];

    ChatWindow()
    {
        memset(inputBuf, 0, sizeof(inputBuf));
    }

    void Draw()
    {
        ImGui::Begin("Chat Window");

        // Display messages
        for (auto& message : messages)
        {
            ImGui::TextUnformatted(message.c_str());
        }

        // Scroll to bottom when a new message is added
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        // Text input
        bool reclaim_focus = false;
        if (ImGui::InputText("Input", inputBuf, sizeof(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            std::string message = inputBuf;
            if (!message.empty())
            {
                // Send the message over the network
                sendMessage(message);
                reclaim_focus = true;
            }

            // Clear the input text field
            memset(inputBuf, 0, sizeof(inputBuf));
        }

        // Auto focus on the text input field
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    // Add a method to add a message to the chat history
    void addMessage(const std::string& message)
    {
        messages.push_back(message);
    }

    // Method to send a message over the network
    void sendMessage(const std::string& message)
    {
        messages.push_back(message);
        // Code to send the message over the network...
    }
};
