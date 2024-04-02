#include "EngineGUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui.h>

void EngineGUI::UpdateGui()
{
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Hello, world!");
	ImGui::Button("Look at this pretty button");
	ImGui::End();
	ImGui::Begin("Hello, world!2");
	ImGui::Button("Look at this pretty button");
	ImGui::End();
	ImGui::Begin("Hello, world!3");
	ImGui::Button("Look at this pretty button");
	ImGui::End();
	ImGui::Begin("Hello, world!4");
	ImGui::Button("Look at this pretty button");
	ImGui::End();
	ImGui::Begin("Hello, world!5");
	ImGui::Button("Look at this pretty button");
	ImGui::End();
	ImGui::Begin("Hello, world!6");
	ImGui::Button("Look at this pretty button");
	ImGui::End();
	ImGui::Begin("Hello, world!7");
	ImGui::Button("Look at this pretty button");
	ImGui::End();
	ImGui::Begin("Hello, world!8");
	ImGui::Button("Look at this pretty button");
	ImGui::End();
}

void EngineGUI::RenderGui()
{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
