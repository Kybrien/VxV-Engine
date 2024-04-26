/**
* @brief Adds a component of type T to the GameObject.
* @tparam T The type of component to add.
* @details The type T must be a descendant of Component.
* @throws std::invalid_argument if T is not a descendant of Component.
* Represents adding a new component of type T to the GameObject.
*/
template<typename T>
void GameObject::AddComponent() {

    static_assert(std::is_base_of<Component, T>::value, "T must be a descendant of Component");

    // Adds a new component of type T
    components.push_back(new T(this));
}


/**
* @brief Retrieves a component of type T attached to the GameObject.
* @tparam T The type of component to retrieve.
* @return Pointer to the component of type T if found, otherwise nullptr.
* Represents searching for a component of type T attached to the GameObject.
*/
template<typename T>
T* GameObject::GetComponent() {

    for (Component* comp : components) {
        T* typedComp = dynamic_cast<T*>(comp);
        if (typedComp != nullptr) {
            return typedComp; // Returns the component of type T if found
        }
    }

    return nullptr; // No component of type T found
}
