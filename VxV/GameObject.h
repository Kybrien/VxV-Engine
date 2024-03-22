#pragma once
#include <vector>
#include <memory>
#include "Components.h"  // Assurez-vous de définir une classe ou une structure de base Component

class GameObject {
public:
    std::vector<std::shared_ptr<Component>> components;

    template <typename T>
    void AddComponent(std::shared_ptr<T> component) {
        components.push_back(component);
    }

    template <typename T>
    T* GetComponent() {
        for (auto& comp : components) {
            if (typeid(*comp) == typeid(T)) {
                return dynamic_cast<T*>(comp.get());
            }
        }
        return nullptr;
    }
};
