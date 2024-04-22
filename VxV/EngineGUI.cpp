#include "EngineGUI.h"


void EngineGUI::UpdateGui(ChatContext* chatContext)
{
	//ImGui::ShowDemoWindow();

	MainMenuBar();

	// Fen�tre pour l'acc�s aux fichiers
	ShowFileExplorer();
	//ShowMenuBar();
	ShowChat(chatContext);
	ShowInfo();
}

void EngineGUI::RenderGui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
