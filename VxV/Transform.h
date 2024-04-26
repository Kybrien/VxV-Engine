#pragma once
#include <glm/glm.hpp>
#include "Component.h"


using namespace glm;
class Transform : public Component {
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;


public:
	/**
	* @brief Instanciate a component Transform.
	* @param A reference to the linked GameObject
	*/
	Transform(GameObject* gameObject);

	std::string name = "Transform";



	/**
	* @brief Set the position of the GameObject.
	* @param A vector3 with the new position
	* @param A boolean to know if the position is a child position
	* @return void
	*/
	void SetPosition(glm::vec3 pos, bool isChild = false);

	/**
	* @brief Set the rotation of the GameObject.
	* @brief A float with the new rotation on X
	* @brief A float with the new rotation on Y
	* @brief A float with the new rotation on Z
	* @param A boolean to know if the position is a child position
	* @return void
	*/
	void SetRotation(float xAngle, float yAngle, float zAngle, bool isChild = false);

	/**
	* @brief Set the scale of the GameObject.
	* @param A vector3 with the new scale
	* @param A boolean to know if the position is a child position
	* @return void
	*/
	void SetScale(glm::vec3 sca, bool isChild = false);


	/**
	* @brief Translate the GameObject.
	* @param A vector3 with the deplacement vector
	* @param A boolean to know if the position is a child position
	* @return void
	*/
	void Translate(glm::vec3 pos, bool isChild = false);

	/**
	* @brief Rotate the GameObject.
	* @param A float with the rotation on X
	* @param A float with the rotation on Y
	* @param A float with the rotation on Z
	* @param A boolean to know if the position is a child position
	* @return void
	*/
	void Rotate(float xAngle, float yAngle, float zAngle, bool isChild = false);

	/**
	* @brief Scale the GameObject.
	* @param A vector3 with the Scale changement
	* @param A boolean to know if the position is a child position
	* @return void
	*/
	void Scale(glm::vec3 sca, bool isChild = false);



	/**
	* @brief Get the position of the GameObject.
	* @return glm::vec3
	*/
	glm::vec3 GetPosition() { return position; }

	/**
	* @brief Get the rotation of the GameObject.
	* @return glm::vec3
	*/
	glm::vec3 GetRotation() { return rotation; }

	/**
	* @brief Get the scale of the GameObject.
	* @return glm::vec3
	*/
	glm::vec3 GetScale() { return scale; }


	void Load(Json::Value& compJson, GameObject* ParentGO) override;

	void Save(Json::Value& compJson) override;

	void Copy(GameObject* goToFill) override;

private:
	/**
	* @brief Load a Vector3.
	* @param A Json Value used by the lib jsoncpp to save the data
	* @param A reference to the vector3 to save
	* @return void
	*/
	void SaveVec3(Json::Value& compJson, glm::vec3 vec);

	/**
	* @brief Save a Vector3.
	* @param A Json Value used by the lib jsoncpp to load the data
	* @param A reference to the vector3 to fill
	* @return void
	*/
	void LoadVec3(Json::Value& compJson, glm::vec3& vec);
};
