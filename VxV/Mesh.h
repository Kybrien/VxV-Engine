#pragma once


#include "Component.h"
#include "object.hpp"


class Mesh : public Component
{
private:
	Object* mesh;

public:
	Mesh(GameObject* go);

	void Save(Json::Value& compJson) override;
	void Load(Json::Value& compJson, GameObject* ParentGO) override;

	void Copy(GameObject* goToFill) override;

	void SetMesh(std::string meshName);

	Object*& GetMesh() {
		return mesh;
	}
};
