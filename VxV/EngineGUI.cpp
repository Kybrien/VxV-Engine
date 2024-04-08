#include "EngineGUI.h"
#include "FileExplorer.h"
#include "WindowGui.h"
#include <imgui.h>


void EngineGUI::UpdateGui()
{
	ImGui::ShowDemoWindow();

	MainMenuBar();

 // Fen�tre pour l'acc�s aux fichiers
    ShowFileExplorer(); 
	//ShowMenuBar();
	ShowInfo();
}

void EngineGUI::RenderGui()
{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
