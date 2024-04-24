#pragma once
#include "loadingShader.hpp"
#include "APIopenGL.hpp"
#include <iostream>

#include "Engine.h"
#include "EngineGUI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imfilebrowser.h"

#include "Debug.h"

void startup(EngineGUI* _gui, APIopenGL* _apiGraphic);
void apiDrawLoopSetup(APIopenGL* _apiGraphic);
void checkCloseWindow(APIopenGL* _apiGraphic, Engine* _engine);