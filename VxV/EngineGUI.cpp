﻿#include "EngineGUI.h"
#include "FileExplorer.h"
#include <imgui.h>
#include "ChatWindow.hpp"
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
    // Load the font
    ImFont* font = io.Fonts->AddFontFromFileTTF("monocraft.ttf", 17);

    //Check if the font has been loaded correctly
    if (font == nullptr)
    {
        std::cerr << "Erreur lors du chargement de la police." << std::endl;
    }
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

ChatWindow chatWindow;

void EngineGUI::UpdateGui()
{
    
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    chatWindow.Draw();
    ImGui::ShowDemoWindow();
    RedirectCout();
    MainMenuBar();
    ShowInfo();
    //RenderToolbar();
    ShowConsoleWindow();
    ShowAddGameObject();
}



void EngineGUI::RenderGui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}