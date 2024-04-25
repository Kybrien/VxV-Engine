#include "PrefabManager.h"
#include "SceneManager.h"
#include <json.h>
#include <iostream>


std::string PrefabManager::fileDirection = "Saves/Prefab";
std::string PrefabManager::extention = ".prefab";


PrefabManager::PrefabManager(Manager* manager) {
	manager->AddManager<PrefabManager>(this);

	//Rechercher les prefab
	SearchFile<PrefabManager>(fileDirection, extention, this);
}


void PrefabManager::Load(std::wstring wFileDirection, std::wstring wFileName) {
	std::string fileDirection;
	for (wchar_t wc : wFileDirection) {
		fileDirection.push_back(static_cast<char>(wc));
	}

	std::ifstream file(fileDirection);



	if (file.is_open()) {

		// V�rification que le fichier est bien une save de scene
		std::string line;
		std::getline(file, line);





		try {

			if (line == "PrefabSaveFile") {
				// Lecture du contenu du fichier dans une cha�ne de caract�res
				std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

				// Fermeture du fichier
				file.close();

				// Analyse de la chaine JSON
				Json::Value PrefabJson;
				Json::Reader reader;
				if (!reader.parse(jsonString, PrefabJson)) {
					std::cerr << "Erreur lors de l'analyse du JSON : " << reader.getFormattedErrorMessages() << std::endl;
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

			std::cerr << "Erreur lors de la lecture du fichier : " + fileDirection << std::endl;
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

			// Conversion de l'objet JSON en une cha�ne JSON format�e
			std::string jsonString = Json::writeString(builder, PrefabJson);

			outputFile << jsonString << std::endl << std::endl;




			outputFile.close();
			std::cout << "Les donn�es ont �t� �crites dans le fichier " + fileDirection + " avec succ�s." << std::endl;
		}
		else {
			std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
		}
	}
}
