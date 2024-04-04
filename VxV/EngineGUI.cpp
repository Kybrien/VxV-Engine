#include "EngineGUI.h"
#include "FileExplorer.h"
#include <imgui.h>

void EngineGUI::UpdateGui()
{
	ImGui::ShowDemoWindow();

 // Fenêtre pour l'accès aux fichiers
    ShowFileExplorer(); 
    // Début de la fenêtre transparente
    ImGui::SetNextWindowBgAlpha(0.5f); // Rendre la fenêtre semi-transparente
    ImGui::Begin("Statistiques", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);

    // Afficher les FPS
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    // Afficher la RAM consommée en pourcentage
    ImGui::Text("RAM: %.1f%%", (float)(ImGui::GetIO().MetricsAllocs * 100) / ImGui::GetIO().MetricsAllocsMax);

    ImGui::End();


}

void EngineGUI::RenderGui()
{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
