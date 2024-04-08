#include "SceneManager.h"
#include <json.h>

std::string SceneManager::fileDirection = "Saves/Scenes";
std::string SceneManager::extention = ".scene";






SceneManager::SceneManager(Manager* manager) {

	manager->AddManager<SceneManager>(this);


	//Rechercher les scenes
	SearchFile<SceneManager>(fileDirection, extention, this);

	
	// Ajouter une scene s'il n'en existe pas déjà
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

		// Conversion de l'objet JSON en une chaîne JSON formatée
		std::string jsonString = Json::writeString(builder, sceneJson);

		outputFile << jsonString << std::endl << std::endl;




		outputFile.close();
		std::cout << "Les données ont été écrites dans le fichier " + fileDirection + " avec succès." << std::endl;
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
	}

}







void SceneManager::Load(std::wstring wFileDirection, std::wstring wFileName) {

	// Conversion de wstring en string
	std::string fileDirection;
	for (wchar_t wc : wFileDirection) {
		fileDirection.push_back(static_cast<char>(wc));
	}

	std::ifstream file(fileDirection);



	if (file.is_open()) {

		// Vérification que le fichier est bien une save de scene
		std::string line;
		std::getline(file, line);





		try {

			if (line == "SceneSaveFile") {

				// Lecture du contenu du fichier dans une chaîne de caractères
				std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

				// Fermeture du fichier
				file.close();

				// Analyse de la chaîne JSON
				Json::Value sceneJson;
				Json::Reader reader;
				if (!reader.parse(jsonString, sceneJson)) {
					std::cerr << "Erreur lors de l'analyse du JSON : " << reader.getFormattedErrorMessages() << std::endl;
				}
				else {
					// Accéder aux données JSON
					// Création de la scene
					Scene* scene = new Scene;
					currentScene = scene;

					// Recupération des données de la scene
					scene->name = sceneJson["Name"].asString();

					// Récupération des données des GameObjects
					for (const Json::Value gameObjectJson : sceneJson["GameObjects"])
					{
						GameObject::Load(gameObjectJson);
					}
				}
			}
		}
		catch (const std::exception e) {

			std::cerr << "Erreur lors de la lecture du fichier : " + fileDirection << std::endl;
		}
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier ! : " + fileDirection << std::endl;
	}

}

