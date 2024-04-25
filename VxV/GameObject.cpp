#include "GameObject.h"

#include "PrefabManager.h"
#include "SceneManager.h"
#include "Transform.h"
#include "ScriptingComponent.h"
#include "Model.h"

#include <vector>

GameObject::GameObject(std::string name_, bool PrefabLoading, Prefab* prefab_, bool copying)
{

	parent = nullptr;
	prefab = prefab_;

	if (!PrefabLoading)
	{

		Manager* manager = Manager::GetInstance();
		SceneManager* sceneManager = manager->GetManager<SceneManager>();
		currentScene = sceneManager->GetCurrentScene();
		std::vector<GameObject*> goList = currentScene->GetAllGameObjects();

		bool idFound = false;
		int i = 0;

		while (!idFound)
		{
			bool idFree = true;
			for (GameObject* go : goList)
			{
				if (go->GetId() == i)
				{
					idFree = false;
					break;
				}
			}
			if (idFree)
			{
				id = i;
				idFound = true;
			}
			else
				i++;
		}

		currentScene->AddGameObject(this);
	}

	if (prefab == nullptr)
	{
		name = name_;
		AddComponent<Transform>();
	}

	else if (!copying)
	{
		AddComponent<Transform>();
		Prefab::Copy(this, (prefab_->getGameObject()));
	}
}

void GameObject::Load(Json::Value root, GameObject* goParent, bool PrefabLoading) {
	Manager* manager = Manager::GetInstance();
	SceneManager* sceneManager = manager->GetManager<SceneManager>();

	GameObject* go = sceneManager->gameObjectPool.CreateGoFromPool("", PrefabLoading, nullptr, true);
	go->SetId(root["Id"].asInt());
	go->name = root["Name"].asString();
	go->isChild = root["Is Child"].asBool();

	if (root.isMember("PrefabName"))
	{
		for (Prefab* prefab_ : Manager::GetInstance()->GetManager<PrefabManager>()->GetPrefabs())
		{
			if (prefab_->name == root["PrefabName"].asString())
			{
				go->prefab = prefab_;
			}
		}
	}

	for (const Json::Value compJson : root["Components"])
	{
		go->LoadComponent(compJson, go);
	}

	if (root.isMember("GameObjectsChild"))
	{
		for (const Json::Value gameObjectJson : root["GameObjectsChild"])
		{
			Load(gameObjectJson, go);
		}
	}

	if (go->isChild)
	{
		goParent->AddChild(go);
	}
}

void GameObject::Save(Json::Value& root)
{
	Json::Value gameObjectJson;

	gameObjectJson["Id"] = id;
	gameObjectJson["Name"] = name;
	gameObjectJson["Is Child"] = isChild;

	if (prefab != nullptr)
	{
		gameObjectJson["PrefabName"] = prefab->name;
	}

	gameObjectJson["Components"] = Json::Value(Json::arrayValue);

	for (Component* comp : components)
	{
		Json::Value compJson;
		comp->Save(compJson);
		gameObjectJson["Components"].append(compJson);
	}

	if (!childObjects.empty())
	{
		gameObjectJson["GameObjectsChild"] = Json::Value(Json::arrayValue);

		for (GameObject* goChild : childObjects)
		{
			goChild->Save(gameObjectJson);
		}
	}

	if (isChild)
	{
		root["GameObjectsChild"].append(gameObjectJson);
	}
	else
	{
		root["GameObjects"].append(gameObjectJson);
	}
}

void GameObject::Delete(GameObject* go)
{
	if (go->isChild)
	{
		go->GetParent()->GetChildByName(go->name);
	}
}

void GameObject::AddChild(GameObject* go)
{
	go->GetComponent<Transform>()->position.x -= GetComponent<Transform>()->position.x;
	go->GetComponent<Transform>()->position.y -= GetComponent<Transform>()->position.y;
	go->GetComponent<Transform>()->position.z -= GetComponent<Transform>()->position.z;
	go->origin = origin;

	go->isChild = true;
	go->AddParent(this);

	// Enlever de la liste des gameObjects (only parents)
	for (int i = currentScene->GetGameObjects().size() - 1; i >= 0; --i)
	{
		GameObject* goList = currentScene->GetGameObjects()[i];
		if (goList == go)
		{
			auto item = currentScene->GetGameObjects().begin() + i;
			currentScene->GetGameObjects().erase(item);
			break;
		}
	}

	childObjects.push_back(go);
}

std::vector<GameObject*> GameObject::GetChilds()
{
	return childObjects;
}

GameObject* GameObject::GetChildByName(std::string name)
{
	for (GameObject* go : childObjects)
	{
		if (go->name == name)
		{
			return go;
		}
	}

	return nullptr;
}

void GameObject::RemoveChild(GameObject* goChild)
{
	for (int i = 0; i < childObjects.size(); i++)
	{
		GameObject* go = childObjects[i];
		if (go == goChild)
		{
			go->parent = nullptr;
			childObjects.erase(childObjects.begin() + i);
			break;
		}
	}
}

void GameObject::LoadComponent(Json::Value compJson, GameObject* parentGo)
{
	switch (compJson["Type"].asInt())
	{
	case 0: break;
	case 1: GetComponent<Transform>()->Load(compJson, parentGo);
		break;
	case 2:
		AddComponent<Model>();
		GetComponent<Model>()->Load(compJson, parentGo);
		break;
	case 3: AddComponent<ScriptingComponent>();
		GetComponent<ScriptingComponent>()->Load(compJson, parentGo);
		break;
	}
}

std::vector<Component*> GameObject::GetComponents()
{
	return components;
}
