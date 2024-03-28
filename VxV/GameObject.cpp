#include "GameObject.h"

#include "SceneManager.h"
#include "Transform.h"

GameObject* GameObject::GetChildByName(std::string name) {
    for (GameObject* go : childObjects)
    {
        for (GameObject* go : currentScene->GetGameObjects()) {
            if (go->name == name) return go;
        }
    }

    return nullptr;
}

GameObject::GameObject(std::string name_) {
    name = name_;
    components.push_back(new Transform(this));
    components.push_back(new Script(this, "script"));


    Manager* manager = Manager::GetInstance();
    SceneManager* sceneManager = manager->GetManager<SceneManager>();
    currentScene = sceneManager->GetCurrentScene();


    std::vector<GameObject*> goList = currentScene->GetAllGameObjects();


    
    
    
    bool idFound = false;
    int i = 0;

    while (!idFound) {
        bool idFree = true;
        for (GameObject* go : goList) {
            if (go->GetId() == i) {
                idFree = false;
                break;
            }

            if (idFree) id = i;
            else i++;
        }

    }




    currentScene->AddGameObject(this);
}

void GameObject::AddChild(GameObject* go)
{
    go->GetComponent<Transform>()->position.x -= GetComponent<Transform>()->position.x;
    go->GetComponent<Transform>()->position.y -= GetComponent<Transform>()->position.y;
    go->GetComponent<Transform>()->position.z -= GetComponent<Transform>()->position.z;
    go->origin = origin;

    go->isChild = true;
    go->AddParent(this);


    childObjects.push_back(go);
}


void GameObject::Delete(GameObject* go) {
    if (go->isChild) {
        go->GetParent()->GetChildByName(go->name);
        
    }
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

std::vector<GameObject*> GameObject::GetChilds() {
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
    else if (typeStr == "1") {
        type = Component::Type::Mesh_renderer;
    }
    else if (typeStr == "2") {
        type = Component::Type::Script;
    }


    switch (type) {
    case Component::Type::Transform:
        GetComponent<Transform>()->Load(compJson);
        break;
    case Component::Type::Script:
        GetComponent<Script>()->Load(compJson);
        break;
    }
}