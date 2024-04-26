#include "Light.h"
#include "GameObject.h"

Light::Light() : Component() 
{
	type = Component::Type::Light;
	name = "Light";
}
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