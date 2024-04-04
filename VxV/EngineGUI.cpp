#include "EngineGUI.h"
#include "FileExplorer.h"
#include "WindowGui.h"
#include <imgui.h>

void EngineGUI::UpdateGui()
{
	ImGui::ShowDemoWindow();

 // Fenêtre pour l'accès aux fichiers
    ShowFileExplorer(); 
	ShowMenuBar();

}

void EngineGUI::RenderGui()
{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
