
#include <Windows.h>
#include <codecvt>


template<typename T>
void Manager::SearchFile(std::string fileDirection, std::string extension, T* manager) {

    // Convertir les chaînes std::string en std::wstring
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring fileDirectionW = converter.from_bytes(fileDirection);
    std::wstring extensionW = converter.from_bytes(extension);

    // Former le chemin du dossier avec l'extension
    std::wstring directoryPathW = fileDirectionW;
    std::wstring wildcard = directoryPathW + L"\\*" + extensionW;

    // Utiliser wildcard pour rechercher les fichiers
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(wildcard.c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::wstring fileName = findFileData.cFileName;
                std::wstring filePath = directoryPathW + L"\\" + fileName;
                manager->Load(filePath, fileName);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}


template<typename T>
T* Manager::GetManager() {
    for (Manager* manager : listManager) {
        T* typedManager = dynamic_cast<T*>(manager);
        if (typedManager != nullptr) {
            return typedManager; // Retourner le composant du type T s'il est trouvé
        }
    }

    return nullptr; // Aucun composant de type T trouvé
}


template<typename T>
void Manager::AddManager(T* manager) {
    static_assert(std::is_base_of<Manager, T>::value, "T doit être un descendant de Manager");

    // Ajoute un nouveau composant de type T
    listManager.push_back(manager);
}