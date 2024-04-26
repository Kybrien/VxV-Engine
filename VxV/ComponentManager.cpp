#include "ComponentManager.hpp"
#include "Transform.h"
#include "Light.h"
#include "Model.h"
#include "ScriptingComponent.h"

#include "Component.h"
#include "GameObject.h"


#define DECLARE_COMPONENT(_type) \
ComponentMap[#_type] = &createInstance<_type>; \
componentList.push_back(#_type);

ComponentManager* ComponentManager::m_Instance = nullptr;

ComponentManager::ComponentManager()
{
	DECLARE_COMPONENT(Transform);
	DECLARE_COMPONENT(Light);
	DECLARE_COMPONENT(Model);
	DECLARE_COMPONENT(ScriptingComponent);
}

Component* ComponentManager::createComponent(std::string type)
{
	if (ComponentMap.find(type) == ComponentMap.end())
	{
		std::cout << "Component type " << type << " not found" << std::endl;
		return nullptr;
	}

	return ComponentMap[type]();
}