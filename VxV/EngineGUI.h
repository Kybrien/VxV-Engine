#pragma once
#include "EngineGUI.h"
#include <imgui.h>
#include "FileExplorer.h"
#include "WindowGui.h"
#include "_Chat.h"
#include "imgui_impl_opengl3.h"

class EngineGUI
{
public:
	void UpdateGui(ChatContext* chat);
	void RenderGui();
};
