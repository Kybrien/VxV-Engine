#pragma once
#include <vector>
#include <string>
#include <map>

class Component;
class GameObject;

typedef std::vector<std::string> Victor;

template<typename T> Component* createInstance() { return new T; }

class ComponentManager
{
public:
	ComponentManager();
	static ComponentManager* GetInstance()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new ComponentManager();
		}
		return m_Instance;
	}

	Victor GetComponentNameList() const
	{
		return componentList;
	}


	Component* createComponent(std::string type);

private:
	static ComponentManager* m_Instance;


	Victor componentList;

	typedef std::map<std::string, Component* (*)()> map_type;
	map_type ComponentMap;
};

