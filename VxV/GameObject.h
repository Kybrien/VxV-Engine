#pragma once
#include <memory>
#include "Component.h"  // Assurez-vous de définir une classe ou une structure de base Component
#include "Transform.h"
#include <iostream>
#include <vector>
#include <string>

class SceneManager;
class Scene;


class GameObject {
public:

    std::vector<Component*> components;
    SceneManager* sceneManager;
    Scene* currentScene;

    GameObject(std::string name_ = "GO");

    glm::vec3 origin;
    bool isChild = false;


    std::string name; // Nom du GO
    virtual void Init() {}; // Est appelé au début 
    virtual void Start() {}; // Est appelé à la première frame
    virtual void Update() {}; // Est appelé à chaque frame

    const int GetId() { return id; } // Renvoie l'Id du GameObject
    void SetId(int id_) { id = id_; }

    GameObject* GetChildByName(std::string name); // Rechercher un GameObject enfant du GameObject
    std::vector<Component*> GetComponents();
    std::vector<int> GetChilds();


    void AddChild(GameObject* go);


    // Mettre des enable if
    template<typename T>
    T* GetComponent(); // Rechercher un component

    template<typename T>
    void AddComponent();

    void LoadComponent(Json::Value compJson);


private:
    int id; // ID du GO
    std::vector<int> childObjects; // Liste des enfants
};
