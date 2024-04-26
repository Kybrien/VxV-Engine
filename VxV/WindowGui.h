#pragma once

#include <stdlib.h>
#include <string>
#include "imfilebrowser.h"
#include "GameObject.h"
#include "ConsoleRecup.h"
#include "Scene.h"
#include "Engine.hpp"


static bool enabled = true;
static ImGui::FileBrowser fileDialog;
GameObject* selectedGameObject = nullptr;

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


			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::BeginMenu("Controls"))
			{
				// Affichez le texte d'aide
				ImGui::Text("Utilisez ZQSD et EA pour deplacer la cam.");
				ImGui::Text("Utilisez Shift pour deplacer plus vite");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("About"))
			{
				// Affichez le texte d'information
				ImGui::Text("This engine was created by:");
				ImGui::Text(" VxV Group");
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

static void ShowInfo() {
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Informations", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Obtenez le nombre de FPS
	float fps = ImGui::GetIO().Framerate;

	// Affichez le nombre de FPS
	ImGui::Text("FPS: %.1f", fps);

	// Obtenez la position de la souris
	ImVec2 mousePos = ImGui::GetIO().MousePos;

	// Affichez la position de la souris
	ImGui::Text("Position de la souris: (%.1f, %.1f)", mousePos.x, mousePos.y);

	// Terminez la fen?tre
	ImGui::End();
}

void ShowError(const std::string& message)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Erreur", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'erreur
	ImGui::Text(message.c_str());

	// Terminez la fen?tre
	ImGui::End();
}

void ShowSuccess(const std::string& message)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Succ?s", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message de succ?s
	ImGui::Text(message.c_str());

	// Terminez la fen?tre
	ImGui::End();
}

void ShowWarning(const std::string& message)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Avertissement", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'avertissement
	ImGui::Text(message.c_str());

	// Terminez la fen?tre
	ImGui::End();
}

void ShowConfirmation(const std::string& message, bool& show)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Confirmation", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message de confirmation
	ImGui::Text(message.c_str());

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer"))
	{
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler"))
	{
		show = false;
	}

	// Terminez la fen?tre
	ImGui::End();
}

void ShowInputInt(const std::string& message, int& input, bool& show)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Entr?e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr?e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr?e
	ImGui::InputInt("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer"))
	{
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler"))
	{
		show = false;
	}

	// Terminez la fen?tre
	ImGui::End();
}

void ShowInputFloat(const std::string& message, float& input, bool& show)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Entr?e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr?e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr?e
	ImGui::InputFloat("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer"))
	{
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler"))
	{
		show = false;
	}

	// Terminez la fen?tre
	ImGui::End();
}

void ShowInputDouble(const std::string& message, double& input, bool& show)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Entr?e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr?e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr?e
	ImGui::InputDouble("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer"))
	{
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler"))
	{
		show = false;
	}

	// Terminez la fen?tre
	ImGui::End();
}

void ShowInputVec2(const std::string& message, glm::vec2& input, bool& show)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Entr?e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr?e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr?e
	ImGui::InputFloat2("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer"))
	{
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler"))
	{
		show = false;
	}

	// Terminez la fen?tre
	ImGui::End();
}

void ShowInputVec3(const std::string& message, glm::vec3& input, bool& show)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Entr?e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr?e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr?e
	ImGui::InputFloat3("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer"))
	{
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler"))
	{
		show = false;
	}

	// Terminez la fen?tre
	ImGui::End();
}

void ShowInputVec4(const std::string& message, glm::vec4& input, bool& show)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Entr?e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr?e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr?e
	ImGui::InputFloat4("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer"))
	{
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler"))
	{
		show = false;
	}

	// Terminez la fen?tre
	ImGui::End();
}

void ShowInputColor(const std::string& message, glm::vec4& input, bool& show)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Entr?e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr?e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr?e
	ImGui::ColorEdit4("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer"))
	{
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler"))
	{
		show = false;
	}

	// Terminez la fen?tre
	ImGui::End();
}

void ShowInputBool(const std::string& message, bool& input, bool& show)
{
	// Cr?ez une nouvelle fen?tre ImGui
	ImGui::Begin("Entr?e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr?e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr?e
	ImGui::Checkbox("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer"))
	{
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler"))
	{
		show = false;
	}

	// Terminez la fen?tre
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
			std::cout << "Could not add Model component to GameObject" << std::endl;
		}
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
				if (ImGui::Button("open file obj"))
					fileDialog.Open();

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Prefab"))
			{
				if (ImGui::Button("Cube"))
				{
					CreateGameObjectWithModel("cube.obj");
				}
				if (ImGui::Button("Sphere"))
				{
					CreateGameObjectWithModel("sphere.obj");
				}

				if (ImGui::Button("Capsule"))
				{
					CreateGameObjectWithModel("capsule.obj");
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


				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	fileDialog.Display();

	if (fileDialog.HasSelected())
	{

		std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
		CreateGameObjectWithModel(fileDialog.GetSelected().string());
		fileDialog.ClearSelected();
	}
	ImGui::Separator();
}




void ShowConsoleWindow()
{
	// Commencer une nouvelle fenêtre ImGui
	ImGui::Begin("Console");

	// Parcourir la liste des messages de la console
	for (const std::string& message : consoleMessages)
	{
		// Afficher le message
		ImGui::TextUnformatted(message.c_str());
	}

	// Terminer la fenêtre ImGui
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


// Ajoutez un paramètre Scene à la fonction ShowHierarchy
static void ShowHierarchy()
{
	SceneManager* sceneManager = Manager::GetInstance()->GetManager<SceneManager>();
	Scene& scene = *sceneManager->GetCurrentScene();

	// Utilisez la fonction GetAllGameObjects() pour obtenir tous les GameObjects
	std::vector<GameObject*> allGameObjects = scene.GetAllGameObjects();
	// Dans votre boucle de rendu ImGui
	ImGui::Begin("Hierarchie de la scene");
	{
		// Affichez une liste de tous les GameObjects
		for (GameObject* gameobject : allGameObjects)
		{
			std::string temp = gameobject->name + "_" + std::to_string(gameobject->id);
			if (ImGui::Selectable(temp.c_str()))
			{
				// Si l'utilisateur clique sur un GameObject, stockez une référence à ce GameObject
				selectedGameObject = gameobject;
			}
		}
	}
	ImGui::End();


	// Si un GameObject a été sélectionné, affichez ses informations dans une fenêtre ImGui
	if (selectedGameObject != nullptr)
	{
		std::string temp = selectedGameObject->name + "_" + std::to_string(selectedGameObject->id);
		static int selectedComponent = 0;
		ImGui::Begin("Inspector");

		// Affichez le nom du GameObject
		ImGui::Text("Nom: %s", temp.c_str());
		ImGui::Separator();

		// Obtenez le Transform du GameObject
		Transform* transform = selectedGameObject->GetComponent<Transform>();

		// Créez des contrôles de glissement pour la position, la rotation et l'échelle
		float position[3] = { transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z };
		glm::vec3 rotation = transform->GetRotation();
		glm::vec3 scale = transform->GetScale();
		std::string name = selectedGameObject->name;
		std::cout << position << std::endl;



		if (ImGui::DragFloat3("Positon", &position[0]))
		{
			// Si l'utilisateur modifie la position, mettez à jour le Transform
			transform->SetPosition(glm::vec3(position[0], position[1], position[2]));
		}

		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			// Si l'utilisateur modifie la rotation, mettez à jour le Transform
			transform->SetRotation(rotation.x, rotation.y, rotation.z);
		}

		if (ImGui::DragFloat3("Scale", &scale.x, 0.1f, 0.1f, 10.0f))
		{
			// Si l'utilisateur modifie l'échelle, mettez à jour le Transform
			transform->SetScale(scale);
		}

		if (ImGui::Button("Delete"))
		{
			selectedGameObject->Delete();
			sceneManager->gameObjectPool.Free(selectedGameObject);
			selectedGameObject = nullptr;
		}


		ImGui::End();


	}

}