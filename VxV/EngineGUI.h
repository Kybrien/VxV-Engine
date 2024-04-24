#pragma once
#include <GLFW/glfw3.h>
class EngineGUI
{
public:

	void initImgui(GLFWwindow* window);
	void UpdateGui();
	void RenderGui();
};