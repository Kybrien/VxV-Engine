#include "EngineGUI.h"

#include <imgui.h>

void EngineGUI::UpdateGui()
{
	//ImGui::ShowDemoWindow();

 // Fenêtre pour l'accès aux fichiers
    if (ImGui::Begin("Explorateur de fichiers")) {
        // Utiliser ici les fonctions de ImGui pour lister les fichiers et gérer les interactions
        ImGui::Text("Contenu du répertoire : ...");
    }
    ImGui::End();



    // Fenêtre pour les GameObjects
    if (ImGui::Begin("GameObjects")) {
        // Affiche et permet la manipulation des GameObjects existants
    }
    ImGui::End();

    // Fenêtre pour les scènes
    if (ImGui::Begin("Scènes")) {
        // Liste les scènes disponibles et permet d'en sélectionner une, la charger, etc.
    }
    ImGui::End();

    // Console
    if (ImGui::Begin("Console")) {
        // Tu peux ici implémenter une console pour les logs ou les commandes
    }
    ImGui::End();
}

void EngineGUI::RenderGui()
{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
