#pragma once
#include <vector>
#include <iostream>

template <class T>
class ObjectPool
{
private:
	std::vector<T*> m_pool;
	std::vector<T*> m_activeObjects;

public:
	ObjectPool(int poolSize)
	{
		for (int i = 0; i < poolSize; i++) {
			m_pool.push_back(new T());
		}
	}
	~ObjectPool()
	{
		for (T* obj : m_pool) {
			delete obj;
		}
		m_pool.clear();
	}

	T* GetObjectFromPool() {
		if (m_pool.empty()) {
			// Si la pool est vide, créer un nouvel objet
			T* newObj = new T();
			m_activeObjects.push_back(newObj);
			return newObj;
		}
		else {
			// Sinon, réutiliser un objet existant dans le pool
			T* obj = m_pool.back();
			m_pool.pop_back();
			m_activeObjects.push_back(obj);
			return obj;
		}
	}
};

