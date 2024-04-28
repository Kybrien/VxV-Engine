#pragma once

#include <stdlib.h>
#include <string>
#include "imfilebrowser.h"
#include "GameObject.h"
#include "ConsoleRecup.h"
#include "Scene.h"
#include "ScriptingComponent.h"
#include <filesystem>
#include "Engine.hpp"
#include "ComponentManager.hpp"
#include "ScriptManager.h"
#include "Transform.h"
#include "Light.h"


static bool enabled = true;
static ImGui::FileBrowser fileDialog;
GameObject* selectedGameObject = nullptr;

enum class FileState
{
	Init,
	LoadFile,
	ChangeModel
};

FileState fileState = FileState::Init;
Engine* engine = Engine::GetInstance();

static void ShowExampleMenuFile()
{

	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}

	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("test");
		ImGui::MenuItem("test2");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("test3");
			ImGui::MenuItem("test4");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		ImGui::MenuItem("show fps & mouse position,", "", &enabled);
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value slider", &f, 0.0f, 1.0f);
		ImGui::InputInt("value Input", &n, 1);
		ImGui::Combo("Choice", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}

	// Here we demonstrate appending again to the "Options" menu (which we already created above)
	// Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
	// In a real code-base using it would make senses to use this feature from very different code locations.
	if (ImGui::BeginMenu("Options")) // <-- Append!
	{
		static bool b = true;
		ImGui::Checkbox("Checkbox option", &b);
		ImGui::EndMenu();
	}

	ImGui::Separator();
	if (ImGui::MenuItem("Quit", "Alt+F4")) { exit(0); }
}


/**
* @brief Show the main menu bar
* @details This function will show the main menu bar
* @return void
*/
static void MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {} // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Load")) {}
			if (ImGui::MenuItem("Save")) {}
			if (ImGui::MenuItem("Save As")) {}


			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("General")) {}
			if (ImGui::MenuItem("Editor")) {}
			if (ImGui::MenuItem("Scene")) {}
			if (ImGui::MenuItem("Input")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::BeginMenu("Controls"))
			{
				ImGui::Text("Use ZQSD/WASD to move the camera while holding mouse right click.");
				ImGui::Text("Hold Shift to move faster");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("About"))
			{
				ImGui::Text("This engine was created by:");
				ImGui::Text(" VxV Group");
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

/**
* @brief Show information about the engine
*/
static void ShowInfo() {
	ImGui::Begin("Informations", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	float fps = ImGui::GetIO().Framerate;
	ImGui::Text("FPS: %.1f", fps);
	ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImGui::Text("Mouse cursor position: (%.1f, %.1f)", mousePos.x, mousePos.y);
	ImGui::End();
}

void ShowError(const std::string& message)
{
	ImGui::Begin("Error", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::End();
}

void ShowSuccess(const std::string& message)
{
	ImGui::Begin("Success", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::End();
}

void ShowWarning(const std::string& message)
{
	ImGui::Begin("Warning", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::End();
}

void ShowConfirmation(const std::string& message, bool& show)
{
	ImGui::Begin("Confirmation", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	if (ImGui::Button("Confirm"))
	{
		show = false;
	}
	if (ImGui::Button("Cancel"))
	{
		show = false;
	}
	ImGui::End();
}

void ShowInputInt(const std::string& message, int& input, bool& show)
{
	ImGui::Begin("Input", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::InputInt("", &input);
	if (ImGui::Button("Confirm"))
	{
		show = false;
	}
	if (ImGui::Button("Cancel"))
	{
		show = false;
	}
	ImGui::End();
}

void ShowInputFloat(const std::string& message, float& input, bool& show)
{
	ImGui::Begin("Input", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::InputFloat("", &input);
	if (ImGui::Button("Confirm"))
	{
		show = false;
	}
	if (ImGui::Button("Canel"))
	{
		show = false;
	}
	ImGui::End();
}

void ShowInputDouble(const std::string& message, double& input, bool& show)
{
	ImGui::Begin("Input", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::InputDouble("", &input);
	if (ImGui::Button("Confirm"))
	{
		show = false;
	}
	if (ImGui::Button("Cancel"))
	{
		show = false;
	}
	ImGui::End();
}

void ShowInputVec2(const std::string& message, glm::vec2& input, bool& show)
{
	ImGui::Begin("Input", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::InputFloat2("", &input.x);
	if (ImGui::Button("Confirm"))
	{
		show = false;
	}
	if (ImGui::Button("Cancel"))
	{
		show = false;
	}
	ImGui::End();
}

void ShowInputVec3(const std::string& message, glm::vec3& input, bool& show)
{
	ImGui::Begin("Input", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::InputFloat3("", &input.x);
	if (ImGui::Button("Confirm"))
	{
		show = false;
	}
	if (ImGui::Button("Cancel"))
	{
		show = false;
	}
	ImGui::End();
}

void ShowInputVec4(const std::string& message, glm::vec4& input, bool& show)
{
	ImGui::Begin("Input", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::InputFloat4("", &input.x);
	if (ImGui::Button("Confirm"))
	{
		show = false;
	}
	if (ImGui::Button("Cancel"))
	{
		show = false;
	}
	ImGui::End();
}

void ShowInputColor(const std::string& message, glm::vec4& input, bool& show)
{
	ImGui::Begin("Input", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::ColorEdit4("", &input.x);
	if (ImGui::Button("Confirm"))
	{
		show = false;
	}
	if (ImGui::Button("Cancel"))
	{
		show = false;
	}
	ImGui::End();
}

void ShowInputBool(const std::string& message, bool& input, bool& show)
{
	ImGui::Begin("Input", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text(message.c_str());
	ImGui::Checkbox("", &input);
	if (ImGui::Button("Confirm"))
	{
		show = false;
	}
	if (ImGui::Button("Cancel"))
	{
		show = false;
	}
	ImGui::End();
}



GameObject* CreateGameObjectWithModel(const std::string& modelName) {
	Manager* manager = Manager::GetInstance();
	SceneManager* sceneManager = manager->GetManager<SceneManager>();

	GameObject* go = sceneManager->gameObjectPool.CreateGoFromPool(modelName);

	if (go != nullptr) {
		go->AddComponent<Model>();
		Model* modelComponent = go->GetComponent<Model>();
		if (modelComponent != nullptr) {
			modelComponent->SetModel(modelName);
		}
		else {
			std::cout << "Could not add Model component to GameObject." << std::endl;
		}
	}
	else {
		std::cout << "Could not create GameObject." << std::endl;
	}

	return go; // This will return nullptr if GameObject creation failed
}

GameObject* CreateLight() {
	Manager* manager = Manager::GetInstance();
	SceneManager* sceneManager = manager->GetManager<SceneManager>();

	GameObject* go = sceneManager->gameObjectPool.CreateGoFromPool("light");

	if (go != nullptr) {
		go->AddComponent<Light>();
	}
	else {
		std::cout << "Could not create GameObject" << std::endl;
	}

	return go; // This will return nullptr if GameObject creation failed
}

void ShowAddGameObject() {
	if (ImGui::Begin("Create Game Object"))
	{
		static char gameObjectName[128];

		if (ImGui::BeginTabBar("TabBar"))
		{
			if (ImGui::BeginTabItem("Create"))
			{

				// open file dialog when user clicks this button
				fileDialog.SetTypeFilters({ ".obj" });
				if (ImGui::Button("Open file obj"))
				{
					fileState = FileState::LoadFile;
					fileDialog.Open();
				}
				fileDialog.Display();

				if (fileDialog.HasSelected() && fileState == FileState::LoadFile)
				{
					std::filesystem::path filePath(fileDialog.GetSelected().string());

					std::cout << "Selected filename: " << filePath.stem().string() << std::endl;
					CreateGameObjectWithModel(filePath.stem().string());
					fileDialog.ClearSelected();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Prefab"))
			{
				if (ImGui::Button("Cube"))
				{
					CreateGameObjectWithModel("cube");
				}
				if (ImGui::Button("Sphere"))
				{
					CreateGameObjectWithModel("sphere");
				}

				if (ImGui::Button("Capsule"))
				{
					CreateGameObjectWithModel("capsule");
				}
				if (ImGui::Button("Plane"))
				{
					CreateGameObjectWithModel("plane.obj");
				}

				if (ImGui::Button("Cylinder"))
				{
					CreateGameObjectWithModel("cylinder.obj");
				}
				if (ImGui::Button("Miku"))
				{
					CreateGameObjectWithModel("miku.obj");
				}
				if (ImGui::Button("Light"))
				{
					CreateLight();
				}



				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	ImGui::Separator();
}


void ShowConsoleWindow()
{
	ImGui::Begin("Console");
	for (const std::string& message : consoleMessages)
	{
		ImGui::TextUnformatted(message.c_str());
	}
	ImGui::End();
}



void RenderToolbar() {
	// Assuming toolbarHeight is set to your desired height for the toolbar

	// We don't want the toolbar window to be moveable or have any framing
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::Begin("##Toolbar", nullptr, window_flags);

	// Left-aligned buttons
	if (ImGui::Button("Play")) {
		std::cout << "Play" << std::endl;
		engine->GetEngineState()->StartRunTime();
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause")) {
		engine->GetEngineState()->PauseRunTime();
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop")) {
		engine->GetEngineState()->ExitRunTime();
	}

	// Calculate the size needed to center the next set of buttons
	float availableSpace = ImGui::GetContentRegionAvail().x;
	float buttonSize = ImGui::CalcTextSize("Centered Button").x + ImGui::GetStyle().FramePadding.x * 2;
	// Spacing needed to center the buttons
	float spacing = (availableSpace - buttonSize * 3) * 0.5f;
	if (spacing > 0) {
		ImGui::SameLine(spacing);
	}

	if (ImGui::Button("Save"))
	{
		engine->manager->GetManager<SceneManager>()->Save();
	}

	// Center-aligned buttons
	//for (int i = 0; i < 3; ++i) {
	//	if (i > 0) ImGui::SameLine();
	//	if (ImGui::Button("Centered Button")) {
	//		// Trigger action for this button
	//	}
	//}



	ImGui::End();
}

static void ShowHierarchy()
{
	SceneManager* sceneManager = Manager::GetInstance()->GetManager<SceneManager>();
	Scene& scene = *sceneManager->GetCurrentScene();

	std::vector<GameObject*> allGameObjects = scene.GetAllGameObjects();
	ImGui::Begin("Scene hierarchy");
	{
		for (GameObject* gameobject : allGameObjects)
		{
			std::string temp = gameobject->name + "_" + std::to_string(gameobject->id);
			if (ImGui::Selectable(temp.c_str()))
			{
				selectedGameObject = gameobject;
			}
		}
	}
	ImGui::End();


	if (selectedGameObject != nullptr)
	{
		std::string temp = selectedGameObject->name + "_" + std::to_string(selectedGameObject->id);
		static int selectedComponent = 0;
		ImGui::Begin("Inspector");

		ImGui::Text("Name: %s", temp.c_str());
		ImGui::Separator();

		Transform* transform = selectedGameObject->GetComponent<Transform>();
		Model* modelComponent = selectedGameObject->GetComponent<Model>();
		ScriptingComponent* scriptComponent = selectedGameObject->GetComponent<ScriptingComponent>();
		Light* light = selectedGameObject->GetComponent<Light>();

		float position[3] = { transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z };
		glm::vec3 rotation = transform->GetRotation();
		glm::vec3 scale = transform->GetScale();
		std::string name = selectedGameObject->name;



		if (ImGui::DragFloat3("Position", &position[0]))
		{
			transform->SetPosition(glm::vec3(position[0], position[1], position[2]));
		}

		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			transform->SetRotation(rotation.x, rotation.y, rotation.z);
		}

		if (ImGui::DragFloat3("Scale", &scale.x, 0.1f, 0.1f, 10.0f))
		{
			transform->SetScale(scale);
		}
		if (ImGui::BeginCombo("Add Component", ComponentManager::GetInstance()->GetComponentNameList()[0].c_str()))
		{
			for (std::string compo : ComponentManager::GetInstance()->GetComponentNameList())
			{
				bool is_selected = false;
				if (ImGui::Selectable(compo.c_str(), is_selected))
				{
					std::cout << "Component " << compo << " Added." << std::endl;
					selectedGameObject->AddComponent(ComponentManager::GetInstance()->createComponent(compo));
				}
			}
			ImGui::EndCombo();
		}


		if (ImGui::Button("Delete"))
		{
			selectedGameObject->Delete();
			sceneManager->gameObjectPool.Free(selectedGameObject);
			selectedGameObject = nullptr;
		}


		ImGui::Separator();

		ImGui::Text("Script");
		// Ajoutez un bouton pour ajouter un script
		if (ImGui::Button("Add script"))
		{
		//// Ajoutez un bouton pour ajouter un script
		//if (ImGui::Button("Ajouter un script"))
		//{
		//	// Ajoutez un ScriptingComponent au GameObject
		//	ScriptingComponent* scriptComponent = selectedGameObject->AddComponent<ScriptingComponent>();
		//	// Vous pouvez �galement d�finir le script ici si vous le souhaitez
		//	// scriptComponent->SetScript(...);
		//}
		ImGui::Separator();
		ImGui::Text("Light");

		if (light != nullptr)
		{
			glm::vec3 color = light->GetColor();
			float power = light->GetPower();
			if (ImGui::ColorEdit3("Color", &color.x))
			{
				light->SetColor(color);
			}
			if (ImGui::DragFloat("Power", &power, 0.1f, 0.0f, 100.0f))
			{
				light->SetPower(power);
			}
		}
		//else
		//{
		//	// Ajoutez un bouton pour ajouter une lumi�re
		//	if (ImGui::Button("Ajouter une lumi�re"))
		//	{
		//		// Ajoutez un LightComponent au GameObject
		//		Light* lightComponent = selectedGameObject->AddComponent<Light>();
		//		if (lightComponent != nullptr)
		//		{
		//			// D�finissez la couleur et la puissance par d�faut
		//			lightComponent->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		//			lightComponent->SetPower(1.0f);
		//		}
		//	}
		//}



			if (scriptComponent == nullptr) {
				selectedGameObject->AddComponent<ScriptingComponent>();
				scriptComponent = selectedGameObject->GetComponent<ScriptingComponent>();
				std::cout << "ScriptingComponent added." << std::endl;
			}
			
			
		}
		// Add an input field to add a script
		static char scriptName[128] = "";
		static char sName[128] = "";

		if (ImGui::InputText("##", scriptName, IM_ARRAYSIZE(scriptName)))
		{
			if (ImGui::Button("ScriptName"))
			{
				std::string sName = scriptName;

			}
		}
		if (ImGui::Button("Add ScriptName"))
		{
			Script* script = ScriptManager::NewScript(sName);

			scriptComponent->AddScript(script);
		}

		// You can also set the script here if you want
		// scriptComponent->SetScript(...);
		ImGui::Separator();
		ImGui::Text("Model");

		if (ImGui::Button("Change model"))
		{
			fileState = FileState::ChangeModel;
			fileDialog.Open();
		}

		if (fileDialog.HasSelected() && fileState == FileState::ChangeModel)
		{
			std::filesystem::path filePath(fileDialog.GetSelected().string());
			std::cout << "Selected filename: " << filePath.stem().string() << std::endl;
			modelComponent->SetModel(filePath.stem().string());
			fileDialog.ClearSelected();
		}

		if (ImGui::Button("Reset Model"))
		{
			modelComponent->SetModel("cube");

		}
		ImGui::End();
	}

}

