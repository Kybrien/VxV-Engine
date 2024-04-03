#include "ScriptManager.h"
#include "GameObject.h"


std::string ScriptManager::fileDirection = "Scripts";
std::string ScriptManager::extention = ".cpp";



ScriptManager::ScriptManager(Manager* manager) {

	manager->AddManager<ScriptManager>(this);

    SearchFile<ScriptManager>(fileDirection, extention, this);

}



void ScriptManager::Save()
{

}

void ScriptManager::Load(std::wstring wFileDirection, std::wstring wFileName)
{
    // Conversion de wstring en string
    std::string fileDirection;
    for (wchar_t wc : wFileDirection) {
        fileDirection.push_back(static_cast<char>(wc));
    }

    // Conversion de wstring en string
    std::string FileName;
    for (wchar_t wc : wFileName) {
        FileName.push_back(static_cast<char>(wc));
    }

    Script* script = new Script();
    script->Load(fileDirection, FileName);
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

Script* ScriptManager::NewScript(std::string _name) {
    Script* script = new Script();

    bool nameFound = false;
    int i = 1;
    std::string nameChecking = _name;

    while (!nameFound) {

        bool nameTaken = false;
        for (Script* scr : Manager::GetInstance()->GetManager<ScriptManager>()->GetAllScripts()) {
            if (scr->name == nameChecking) {
                nameTaken = true;
                break;
            }
        }

        if (nameTaken) {
            nameChecking = _name + "(" + std::to_string(i) + ")";
        }
        else {
            nameFound = true;
        }

        i++;

    }

    script->name = nameChecking;

    GenerateScript(_name);

    return script;
}