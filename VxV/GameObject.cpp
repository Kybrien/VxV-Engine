#include "GameObject.h"
#include "Debug.h"


GameObject* GameObject::GetChildByName(std::string name) {
	for (GameObject* go : childObjects)
	{
		if (go && go->name == name) return go;
	}

	return nullptr;
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


template<typename T>
void GameObject::AddComponent() {

    static_assert(std::is_base_of<Component, T>::value, "T doit être un descendant de Component");

    // Ajoute un nouveau composant de type T
    components.push_back(new T(this));
}