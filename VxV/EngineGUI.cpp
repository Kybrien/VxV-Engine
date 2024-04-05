#include "EngineGUI.h"
#include "FileExplorer.h"
#include "WindowGui.h"
#include <imgui.h>


void EngineGUI::UpdateGui()
{
	ImGui::ShowDemoWindow();

	MainMenuBar();

 // Fenêtre pour l'accès aux fichiers
    ShowFileExplorer(); 
	//ShowMenuBar();
	ShowInfo();
}

void EngineGUI::RenderGui()
{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
