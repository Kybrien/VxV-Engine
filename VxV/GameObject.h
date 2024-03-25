#pragma once
#include <vector>
#include <memory>
#include "Component.h"  // Assurez-vous de définir une classe ou une structure de base Component
#include "Transform.h"
#include <iostream>
#include <list>
#include <string>

class GameObject {
public:

    std::list<Component*> components;

    GameObject() {
        //components.push_back(new Transform(this));
        // Reucp scene avec manager
        // origin = scene.origin 
    }

    glm::vec3 origin;


    std::string name; // Nom du GO
    virtual void Init() {}; // Est appelé au début 
    virtual void Start() {}; // Est appelé à la première frame
    virtual void Update() {}; // Est appelé à chaque frame

    const int GetId() { return id; } // Renvoie l'Id du GameObject

    GameObject* GetChildByName(std::string name); // Rechercher un GameObject enfant du GameObject


    void AddChild(GameObject* go);
    template<typename T>
    T* GetComponent(); // Rechercher un component

    template<typename T>
    void AddComponent();


private:
    int id; // ID du GO

    std::list<GameObject*> childObjects; // Liste des enfants
};
