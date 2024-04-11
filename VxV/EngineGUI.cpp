#include "EngineGUI.h"
#include <imgui.h>
#include "FileExplorer.h"
#include <imgui.h>
#include "WindowGui.h"

#include "imgui_impl_opengl3.h"

void EngineGUI::UpdateGui()
{
	//ImGui::ShowDemoWindow();

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
