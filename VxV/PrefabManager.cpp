#include "PrefabManager.h"
#include "SceneManager.h"
#include <json.h>
#include <iostream>


std::string PrefabManager::fileDirection = "Saves/Prefab";
std::string PrefabManager::extention = ".prefab";


PrefabManager::PrefabManager(Manager* manager) {
	manager->AddManager<PrefabManager>(this);

	// Search for all prefab files
	SearchFile<PrefabManager>(fileDirection, extention, this);
}


void PrefabManager::Load(std::wstring wFileDirection, std::wstring wFileName) {
	std::string fileDirection;
	for (wchar_t wc : wFileDirection) {
		fileDirection.push_back(static_cast<char>(wc));
	}

	std::ifstream file(fileDirection);

	if (file.is_open()) {

		// Check if the file is a prefab file
		std::string line;
		std::getline(file, line);

		try {

			if (line == "PrefabSaveFile") {
				// Read the file content into a string
				std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

				file.close();

				// Analyse of the JSON string
				Json::Value PrefabJson;
				Json::Reader reader;
				if (!reader.parse(jsonString, PrefabJson)) {
					std::cerr << "Error while analysing the JSON file : " << reader.getFormattedErrorMessages() << std::endl;
				}
				else {
					GameObject* go = new GameObject("", true);

					go->Load(PrefabJson, nullptr, true);

					Prefab* prefab = new Prefab(go);
					prefab->name = PrefabJson["Name"].asString();

					for (GameObject* go : Manager::GetInstance()->GetManager<SceneManager>()->GetCurrentScene()->GetAllGameObjects()) {
						if (prefab->name == go->getPrefabName())
							go->SetPrefab(prefab);
					}
				}

			}
		}
		catch (const std::exception e) {

			std::cerr << "Error while reading the file : " + fileDirection << std::endl;
		}
	}

	
}

void PrefabManager::Save() { 
	for (Prefab* prefab : prefabs) {
		std::ofstream outputFile(fileDirection + "/" + prefab->name + extention);
		Json::StreamWriterBuilder builder;


		if (outputFile.is_open())
		{
			outputFile << "PrefabSaveFile" << std::endl;

			Json::Value PrefabJson;
			PrefabJson["Name"] = prefab->name;
			PrefabJson["GameObjects"] = Json::Value(Json::arrayValue);

			prefab->getGameObject()->Save(PrefabJson);

			// Convert the JSON object into a formatted JSON string
			std::string jsonString = Json::writeString(builder, PrefabJson);

			outputFile << jsonString << std::endl << std::endl;

			outputFile.close();
			std::cout << "Data has been successfully written in the file: " + fileDirection + "." << std::endl;
		}
		else {
			std::cerr << "Error while opening the file: " + fileDirection + "." << std::endl;
		}
	}
}
