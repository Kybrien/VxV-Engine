#include "Light.h"
#include "GameObject.h"

Light::Light(GameObject* gameObject) : Component(gameObject)
{
	type = Component::Type::Light;
	linkedGameObject = gameObject;
}

void Light::Save(Json::Value& compJson) { 
	compJson["Type"] = type;
}

void Light::Copy(GameObject* goToFill) {
	goToFill->AddComponent<Light>();
}

void Light::SetColor(glm::vec3 _color) {
	color = _color;
}

glm::vec3 Light::GetColor() {
	return color;
}

void Light::SetPower(float _power) {
	power = _power;
}

float Light::GetPower() {
	return power;
}