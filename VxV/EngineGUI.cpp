#include "EngineGUI.h"
#include <imgui.h>
#include "FileExplorer.h"
#include "WindowGui.h"
#include "ChatWindow.hpp"
#include "imgui_impl_opengl3.h"

void EngineGUI::UpdateGui()
{
	static ChatWindow chatWindow;
	//ImGui::ShowDemoWindow();

	MainMenuBar();

	/*chatWindow.DrawLogin();*/
	chatWindow.Draw();
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
