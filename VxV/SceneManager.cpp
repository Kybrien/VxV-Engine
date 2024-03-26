#include "SceneManager.h"
#include <json.h>

#include <Windows.h>


#include <fstream>



SceneManager* SceneManager::instance = nullptr;



SceneManager::SceneManager() {
	instance = this;

	//Rechercher les scenes
	const wchar_t* directoryPath = L"Saves/Scenes"; // Spécifiez le chemin du dossier que vous souhaitez parcourir 
	WIN32_FIND_DATA findFileData; 
	HANDLE hFind = INVALID_HANDLE_VALUE; 
	std::wstring wildcard = std::wstring(directoryPath) + L"\\*.json";
	
	hFind = FindFirstFile(wildcard.c_str(), &findFileData);
	if (hFind != INVALID_HANDLE_VALUE) { 
		do {
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { 
				std::wstring filePath = std::wstring(directoryPath) + L"\\" + findFileData.cFileName;
				LoadScene(filePath);
			} 
		} while (FindNextFile(hFind, &findFileData) != 0); 
		FindClose(hFind); 
	}

	
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

void SceneManager::LoadScene(std::wstring wFileDirection) {

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



			// Accéder aux données JSON
			try {

				// Création de la scene
				Scene* scene = new Scene;
				currentScene = scene;

				// Recupération des données de la scene
				scene->name = sceneJson["Name"].asString();

				// Récupération des données des GameObjects
				for (const Json::Value gameObjectJson : sceneJson["GameObjects"])
				{
					GameObject* go = new GameObject();
					go->SetId(gameObjectJson["Id"].asInt());
					go->name = gameObjectJson["Name"].asString();
					go->isChild = gameObjectJson["Is Child"].asBool();
					
					// On skip les child pour le moment, le temps que tous les gameObject soient créés

					for (const Json::Value compJson : gameObjectJson["Components"]) {
						go->LoadComponent(compJson);
					}
				}
				
			} 
			catch (const std::exception& e) {
				std::cerr << "Erreur lors du chargement du fichier " + fileDirection << std::endl;
			}


			
		}
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier ! : " + fileDirection << std::endl;
	}

}
