#include "ScriptManager.h"
#include "GameObject.h"

ScriptManager::ScriptManager(Manager* manager) {

	manager->AddManager<ScriptManager>(this);

	//Rechercher les scripts
	SearchFile<ScriptManager>(fileDirection, extention, this);
}
void ScriptManager::AddScript(Script* script, bool alrExisting) {
	scripts.push_back(script);

    if(!alrExisting) GenerateScript(script->name);
}
void ScriptManager::Save()
{

}

void ScriptManager::Load(std::wstring wFileDirection, std::wstring wFileName)
{
	// Conversion de wstring en string
	std::string fileName;
	for (wchar_t wc : wFileName) {
		fileName.push_back(static_cast<char>(wc));
	}



    //Supprimer l'extention
    std::string fileExtention = "";
    size_t pos = fileName.find_last_of(".");
    if (pos != std::string::npos) {
        fileExtention = fileName.substr(pos); // Extraire l'extension
    }

    if (fileExtention == ".cpp") {
        fileName = fileName.substr(0, pos); // Supprimer l'extension
    }



    Script* script = new Script(nullptr, false, fileName, true);
}




void ScriptManager::GenerateScript(const std::string& className, const std::string& templateFileName) {
    std::ifstream templateFile(templateFileName);
    if (!templateFile.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier modèle " << templateFileName << std::endl;
        return;
    }
    std::ofstream outFile(fileDirection + "/" + className + ".cpp");
    if (!outFile.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << className << ".cpp" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(templateFile, line)) {
        // Remplacer le nom de la classe dans chaque ligne
        size_t pos = line.find("CustomScript");
        if (pos != std::string::npos) {
            line.replace(pos, 12, className);
        }
        outFile << line << '\n';
    }

    templateFile.close();
    outFile.close();
    std::cout << "Fichier " << className << ".cpp créé avec succès." << std::endl;
}