#pragma once
#include "Component.h"
#include <glm/glm.hpp>
using namespace glm;
class Transform : public Component {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;



	Transform(GameObject* gameObject) : Component(gameObject) {

		name = "Transform";
	}


	void Load() override {
	
	}
	
	std::string Save() override {

		std::string str = "	" +  name + "\n";
		str += "	Position : {" + ReturnVec(position) + "}\n";
		str += "	Rotation : {" + ReturnVec(rotation) + "}\n";
		str += "	Scale : {" + ReturnVec(scale) + "}\n";
		


		return str;

		
	}

	std::string ReturnVec(glm::vec3 vec) {
		std::string string = std::to_string(vec.x) + " ," + std::to_string(vec.y) + " ," + std::to_string(vec.z);

		return string;
	}
};
