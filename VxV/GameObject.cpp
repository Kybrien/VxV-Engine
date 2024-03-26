#include "GameObject.h"
#include "Debug.h"
#include "SceneManager.h"


GameObject* GameObject::GetChildByName(std::string name) {
    for (int goID : childObjects)
    {
        for (GameObject* go : currentScene->GetGameObjects()) {
            if (go->GetId() == goID) return go;
        }
    }

    return nullptr;
}

GameObject::GameObject(std::string name_) {
    name = name_;
    components.push_back(new Transform(this));

    sceneManager = SceneManager::GetInstance();
    currentScene = sceneManager->GetCurrentScene();



    currentScene->OrganizeGameObjects();
    std::vector<GameObject*> goList = currentScene->GetGameObjects();


    id = -1;

    for (int i = 0; i < goList.size(); i++) {
        if (goList[i]->GetId() != i) {
            id = i;
            break;
        }
    }

    if (goList.empty()) {
        id = 0;
    }
    else if (id == -1) {
        id = goList.size();
    }



    currentScene->AddGameObject(this);
    // Reucp scene avec manager
    // origin = scene.origin 
}

void GameObject::AddChild(GameObject* go)
{
    go->GetComponent<Transform>()->position.x -= GetComponent<Transform>()->position.x;
    go->GetComponent<Transform>()->position.y -= GetComponent<Transform>()->position.y;
    go->GetComponent<Transform>()->position.z -= GetComponent<Transform>()->position.z;
    go->origin = origin;

    go->isChild = true;

    childObjects.push_back(go->GetId());
}

// Implémentation de GetComponent pour d'autres types de composants
template<typename T>
T* GameObject::GetComponent() {

    for (Component* comp : components) {
        T* typedComp = dynamic_cast<T*>(comp);
        if (typedComp != nullptr) {
            return typedComp; // Retourner le composant du type T s'il est trouvé
        }
    }

    return nullptr; // Aucun composant de type T trouvé
}

std::vector<Component*> GameObject::GetComponents() {
    return components;
}

std::vector<int> GameObject::GetChilds() {
    return childObjects;
}


template<typename T>
void GameObject::AddComponent() {

    static_assert(std::is_base_of<Component, T>::value, "T doit être un descendant de Component");

    // Ajoute un nouveau composant de type T
    components.push_back(new T(this));
}

void GameObject::LoadComponent(Json::Value compJson) {

    Component::Type type ;
    std::string typeStr = compJson["Type"].asString();

    if (typeStr == "0") {
        type = Component::Type::Transform;
    }


    switch (type) {
    case Component::Type::Transform:
        GetComponent<Transform>()->Load(compJson);
        break;
    }
}