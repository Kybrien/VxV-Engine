#include "SceneManager.h"
#include <json.h>

#include <Windows.h>



#include <filesystem>


#include <fstream>



SceneManager* SceneManager::instance = nullptr;



SceneManager::SceneManager() {
	instance = this;


	//Rechercher les scenes
	//std::string folderPath = "Saves/Scenes/"; // Sp�cifiez le chemin du dossier que vous souhaitez parcourir

	//WIN32_FIND_DATA FindFileData;
	//HANDLE hFind = FindFirstFile((folderPath + "\\*.json").c_str(), &FindFileData);

	//if (hFind != INVALID_HANDLE_VALUE) {
	//	do {
	//		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

	//			wchar_t* wideFileName = FindFileData.cFileName;
	//			int len = wcslen(wideFileName) + 1;
	//			char* fileName = new char[len];
	//			wcstombs(fileName, wideFileName, len);

	//			LoadScene(folderPath + fileName);
	//		}
	//	} while (FindNextFile(hFind, &FindFileData) != 0);
	//	FindClose(hFind);
	//}
	//else {
	//	std::cerr << "Impossible d'ouvrir le dossier." << std::endl;
	//}


	
	// Ajouter une scene s'il n'en existe pas d�j�
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
	


		// Conversion de l'objet JSON en une cha�ne JSON format�e
		std::string jsonString = Json::writeString(builder, sceneJson);

		outputFile << jsonString << std::endl << std::endl;




		outputFile.close();
		std::cout << "Les donn�es ont �t� �crites dans le fichier output.json avec succ�s." << std::endl;
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
	}

}

void SceneManager::LoadScene(std::string fileDirection) {

}