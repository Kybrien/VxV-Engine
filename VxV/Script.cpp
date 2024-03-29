#include "Script.h"
#include "ScriptManager.h"
#include <iostream>

Script::Script(GameObject* gameObject, std::string _name) : Component(gameObject) {
    linkedGameObject = gameObject;

	type = Component::Type::Script;
	name = _name;
	ScriptManager* scriptManager = Manager::GetInstance()->GetManager<ScriptManager>();
	scriptManager->AddScript(this, gameObject);
}
void Script::GenerateScript(const std::string& className, const std::string& templateFileName) {
    std::ifstream templateFile(templateFileName);
    if (!templateFile.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier modèle " << templateFileName << std::endl;
        return;
    }
    std::ofstream outFile(ScriptManager::fileDirection + "/" + className + ".cpp");
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
void Script::Start()
{
	std::cout << "Default Start() function called." << std::endl;
}

void Script::Update()
{
	std::cout << "Default Update() function called." << std::endl;
}
