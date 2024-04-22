#include "_Chat.h"

void ShowChat(ChatContext* chatContext) {
    ImGui::Begin("Chat");

    // Zone pour les messages
    ImGui::BeginChild("Scrolling", ImVec2(0, 300), true);
    ImGui::TextUnformatted(chatContext->textBuffer.begin(), chatContext->textBuffer.end());
    ImGui::SetScrollHereY(1.0f);  // Auto-scroll au dernier message
    ImGui::EndChild();

    // Champ de saisie
    static char inputBuf[256];
    if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (strlen(inputBuf) > 0) {
            chatContext->textBuffer.appendf("%s\n", inputBuf);  // Ajoute le message au buffer
            send(chatContext->clientSocket, inputBuf, strlen(inputBuf), 0);  // Envoie le message au serveur
            memset(inputBuf, 0, sizeof(inputBuf));  // Nettoyez l'input après l'envoi
        }
        ImGui::SetKeyboardFocusHere(-1);  // Focus sur le champ de saisie
    }

    // Vérification et affichage des messages entrants
    char buffer[1024];
    int bytesReceived = recv(chatContext->clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';  // Assurez-vous que le message est terminé
        chatContext->textBuffer.appendf("%s\n", buffer);  // Ajoute le message entrant au buffer
    }

    ImGui::End();  // Fin de la fenêtre ImGui
}
