#include "EngineGUI.h"
#include "FileExplorer.h"
#include <imgui.h>
#include "WindowGui.h"


void EngineGUI::UpdateGui()
{
	//ImGui::ShowDemoWindow();

	MainMenuBar();
	ShowInfo();
}

void EngineGUI::RenderGui()
{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
