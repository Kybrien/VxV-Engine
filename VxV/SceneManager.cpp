#include "SceneManager.h"
#include <json.h>

#include <fstream>



SceneManager* SceneManager::instance = nullptr;


SceneManager::SceneManager() {
	instance = this;


	//Rechercher les scenes
	//Les foutre dans la liste
	// 

	
	// Ajouter une scene s'il n'en existe pas déjà
	if (scenes.empty()) {
		new Scene;
	}

	currentScene = scenes.front();

	for (Scene* sc : scenes) {
		std::string str = sc->name;
		std::cout << str << std::endl;
	}

}

void SceneManager::SaveScene() {
	
	std::ofstream outputFile("Saves/Scenes/" + currentScene->name + ".json");
	Json::StreamWriterBuilder builder;



	if (outputFile.is_open())
	{
		Json::Value sceneJson;
		sceneJson["Name"] = currentScene->name;
		sceneJson["GameObjects"] = Json::Value(Json::arrayValue);

		for (GameObject* go : currentScene->GetGameObjects()) {
			Json::Value gameObjectJson;

			gameObjectJson["Id"] = go->GetId();
			gameObjectJson["Name"] = go->name;
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