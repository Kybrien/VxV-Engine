#include "PrefabManager.h"
#include <json.h>


PrefabManager::PrefabManager(Manager* manager) {
	manager->AddManager<PrefabManager>(this);

	//Rechercher les prefab
	SearchFile<PrefabManager>("Saves/Prefab", ".prefab", this);
}


void PrefabManager::Load(std::wstring wFileDirection) {
	std::ofstream outputFile(fileDirection + "/" + currentScene->name + extention);
	Json::StreamWriterBuilder builder;
	currentScene->OrganizeGameObjects();



	if (outputFile.is_open())
	{
		outputFile << "SceneSaveFile" << std::endl;

		Json::Value sceneJson;
		sceneJson["Name"] = currentScene->name;
		sceneJson["GameObjects"] = Json::Value(Json::arrayValue);

		for (GameObject* go : currentScene->GetGameObjects()) {
			Json::Value gameObjectJson;

			gameObjectJson["Id"] = go->GetId();
			gameObjectJson["Name"] = go->name;
			gameObjectJson["Is Child"] = go->isChild;
			gameObjectJson["Child Ids"] = Json::Value(Json::arrayValue);

			for (int goChild : go->GetChilds()) {
				gameObjectJson["Child Ids"].append(goChild);
			}


			gameObjectJson["Components"] = Json::Value(Json::arrayValue);

			for (Component* comp : go->GetComponents())
			{
				Json::Value compJson;
				comp->Save(compJson);
				gameObjectJson["Components"].append(compJson);
			}



			sceneJson["GameObjects"].append(gameObjectJson);
		}



		// Conversion de l'objet JSON en une chaîne JSON formatée
		std::string jsonString = Json::writeString(builder, sceneJson);

		outputFile << jsonString << std::endl << std::endl;




		outputFile.close();
		std::cout << "Les données ont été écrites dans le fichier output.json avec succès." << std::endl;
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
	}
}

void PrefabManager::Save() { 

}
