#include "EngineGUI.h"
#include "FileExplorer.h"
#include <imgui.h>
#include "FileExplorer.h"
#include "WindowGui.h"
#include "imfilebrowser.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void EngineGUI::initImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// Chargez la police qui supporte les caractères accentués
	ImFont* font = io.Fonts->AddFontFromFileTTF("monocraft.ttf", 17);

	// V�rifiez si la police a �t� charg�e correctement
	if (font == nullptr)
	{
		std::cerr << "Erreur lors du chargement de la police." << std::endl;
	}
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void EngineGUI::UpdateGui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();



	//ImGui::ShowDemoWindow();
	RedirectCout();
	MainMenuBar();

	// Fen�tre pour l'acc�s aux fichiers
	ShowFileExplorer();
	//ShowMenuBar();
	ShowInfo();
	ShowConsoleWindow();
	ShowAddGameObject();
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New Scene");
            ImGui::MenuItem("Open Scene...");
            ImGui::MenuItem("Save Scene");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            // ... Edit menu items
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Assets"))
        {
            // ... Assets menu items
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("GameObject"))
        {
            // ... GameObject menu items
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            // ... Window menu items
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            // ... Help menu items
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Docking space setup for a flexible layout, assuming ImGui version is 1.71 or later
    //const ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    //ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);
    // Hierarchy panel
    if (ImGui::Begin("Hierarchy"))
    {
        ImGui::Text("Entities");
        ImGui::Separator();
        for (int i = 0; i < 10; ++i)
        {
            ImGui::PushID(i);
            if (ImGui::Selectable("Entity", false))
            {
                // Select entity logic
            }
            ImGui::PopID();
        }
        ImGui::End();
    }

    // Inspector panel
    /*if (ImGui::Begin("Inspector"))
    {
        if (ImGui::CollapsingHeader("Transform Component"))
        {
            // Transform properties
            ImGui::DragFloat3("Position", position, 0.1f);
            ImGui::DragFloat3("Rotation", rotation, 0.1f);
            ImGui::DragFloat3("Scale", scale, 0.1f);
        }

        if (ImGui::CollapsingHeader("Rendering Component"))
        {
            // Rendering properties
            ImGui::ColorEdit3("Albedo", albedo);
            ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
            ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
            // ... More rendering properties
        }
        // ... More components
        ImGui::End();
    }*/

    // Other panels (e.g., asset browser, scene view, etc.) would be created in a similar fashion

    // End the main window
    ImGui::End();

    // Render the ImGui frame
    ImGui::Render();
}


void EngineGUI::RenderGui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
