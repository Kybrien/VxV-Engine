#include "SceneManager.h"
#include <json.h>

std::string SceneManager::fileDirection = "Saves/Scenes";
std::string SceneManager::extention = ".scene";






SceneManager::SceneManager(Manager* manager) {
	SceneManager::gameObjectPool = MemoryPool<GameObject, MemPool_Linear<GameObject>>(50);
	manager->AddManager<SceneManager>(this);


	// Search for existing scenes
	SearchFile<SceneManager>(fileDirection, extention, this);

	
	// Add a scene if none exist
	if (scenes.empty()) {
		new Scene();
	}

	currentScene = scenes.front();

}

void SceneManager::Save() {
	
	std::ofstream outputFile(fileDirection + "/" + currentScene->name + extention);
	Json::StreamWriterBuilder builder;


	if (outputFile.is_open())
	{
		outputFile << "SceneSaveFile" << std::endl;

		Json::Value sceneJson;
		sceneJson["Name"] = currentScene->name;
		sceneJson["GameObjects"] = Json::Value(Json::arrayValue);

		for (GameObject* go : currentScene->GetGameObjects()) {
			go->Save(sceneJson);
		}

		// Convert the JSON object to a formatted JSON string
		std::string jsonString = Json::writeString(builder, sceneJson);

		outputFile << jsonString << std::endl << std::endl;

		outputFile.close();
		std::cout << "Data has been successfully written in the file:" + fileDirection + "." << std::endl;
	}
	else {
		std::cerr << "Error while opening the file." << std::endl;
	}

}







void SceneManager::Load(std::wstring wFileDirection, std::wstring wFileName) {

	// Convert wstring to string
	std::string fileDirection;
	for (wchar_t wc : wFileDirection) {
		fileDirection.push_back(static_cast<char>(wc));
	}

	std::ifstream file(fileDirection);



	if (file.is_open()) {

		// Check if the file is a scene save
		std::string line;
		std::getline(file, line);





		try {

			if (line == "SceneSaveFile") {

				// Read the file content into a string
				std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

				// Close the file
				file.close();

				// Analyse the JSON string
				Json::Value sceneJson;
				Json::Reader reader;
				if (!reader.parse(jsonString, sceneJson)) {
					std::cerr << "Erreur lors de l'analyse du JSON : " << reader.getFormattedErrorMessages() << std::endl;
				}
				else {
					// Access the JSON data
					// Create the scene
					Scene* scene = new Scene;
					currentScene = scene;

					// Get the scene name
					scene->name = sceneJson["Name"].asString();

					// Get the GameObjects datas
					for (const Json::Value gameObjectJson : sceneJson["GameObjects"])
					{
						GameObject::Load(gameObjectJson);
					}
				}
			}
		}
		catch (const std::exception e) {

			std::cerr << "Error when reading the file : " + fileDirection << std::endl;
		}
	}
	else {
		std::cerr << "Error when opening the file : " + fileDirection << std::endl;
	}

}

