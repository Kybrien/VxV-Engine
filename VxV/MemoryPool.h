#pragma once
#include <new>
#include <vector>
#include "GameObject.h"

template<typename T, class Strategy> class MemoryPool
{
private:
	Strategy m_strategy;
public:
	MemoryPool(int size = 0) : m_strategy(size)
	{

	}

	template<typename... Args>
	T* Alloc(Args... args)
	{
		//if (sizeof(T) != sizeof(type)) {
		//	throw std::bad_alloc();
		//}
		T* temp = reinterpret_cast<T*>(m_strategy.Allocate());
		return new (temp) T(args...);
	}

	void Free(void* p)
	{
		m_strategy.Deallocate(p);
	}

	void FreeAll()
	{
		m_strategy.DeallocateAll();
	}
};

template<class Strategy> class MemoryPool<GameObject, Strategy>
{
private:
	Strategy m_strategy;
public:
	MemoryPool(int size = 0) : m_strategy(size)
	{

	}

	// Create a GameObject from the memory pool
	GameObject* CreateGameObject(std::string name = "GO", bool PrefabLoading = false, Prefab* prefab = nullptr, bool loading = false)
	{
		GameObject* temp = reinterpret_cast<GameObject*>(m_strategy.Allocate());
		return new (temp) GameObject(name, PrefabLoading, prefab, loading);
	}

	void Free(void* p)
	{
		m_strategy.Deallocate(p);
	}

	void FreeAll()
	{
		m_strategy.DeallocateAll();
	}
};
//template <typename T> class MemPool_Malloc
//{
//public:
//	void init() {}
//
//	void* Allocate()
//	{
//		return malloc(sizeof(T));
//	}
//
//	void Deallocate(void* p)
//	{
//		free(p);
//	}
//};

template <typename T> class MemPool_Linear
{
private:
	typedef unsigned int size_type;
private:
	std::vector<uint8_t> m_buffer;// the memory, the objects are stored here
	std::vector<bool> m_state;// true if the memory chunk is occupied, false if it's free
	size_type N;// number of memory chunks

public:
	MemPool_Linear(unsigned int size) : m_buffer(sizeof(T)* size), m_state(size), N(size)
	{
		// initially all memory chunks are free
		for (size_type i = 0; i < N; i++)
		{
			m_state[i] = false;
			std::cout<< "Memory Chunk " << i << " is free" << std::endl;
		}
	}

	void* Allocate()
	{
		size_type old_size = m_buffer.size();
		// search linearily through the array to find an unused
		// memory chunk, take it and mark it occupied
		for (size_type i = 0; i < N; i++)
		{
			if (!m_state[i])
			{
				m_state[i] = true;
				return &m_buffer[sizeof(T) * i];
			}
		}
		ResizePool();
		return &m_buffer[sizeof(T) * old_size];
	}

	void Deallocate(void* p)
	{
		// search all memory chunks to find the one to be freed,
		// then mark it as not occupied.
		for (size_type i = 0; i < N; i++)
		{
			if (&m_buffer[sizeof(T) * i] == p)
			{
				m_state[i] = false;
				break;
			}
		}
	}

	void DeallocateAll()
	{
		for (size_type i = 0; i < N; i++)
		{
			m_state[i] = false;
		}
	}

	void ResizePool()
	{
		std::cout << "Resizing Pool" << std::endl;
		size_type i = m_buffer.size() * 2;
		m_buffer.resize(i);
		m_state.resize(m_state.size() * 2);
	}
};

//template <typename T> class MemPool_Heap
//{
//private:
//	typedef unsigned int size_type; // convinience
//private:
//	size_type N; // number of memory chunks
//	size_type available; // number of memory chunks available
//	T* a[N + 1]; // book keeping, first place not used
//	uint8_t buf[sizeof(T) * N]; // the memory itself, here will the objects be stored
//private:
//
//	inline void swap(size_type i, size_type j)
//	{
//		T* t = a[i];
//		a[i] = a[j];
//		a[j] = t;
//	}
//
//	void up()
//	{
//		for (size_type n = available; n > 1; ) {
//			size_type i = n / 2;
//			if (!(a[i] && a[n]))
//				swap(i, n);
//			n = i;
//		}
//	}
//
//	void down()
//	{
//		size_type i = 1;
//		size_type c = 2 * i;
//		while (c <= available) {
//			if (c + 1 <= available)
//				if (!(a[c] && a[c + 1]))
//					++c;
//			if (!(a[i] && a[c]))
//				swap(i, c);
//			i = c;
//			c = 2 * i;
//		}
//	}
//public:
//	MemPool_Heap() : N(0), a
//	{
//		// number of available memory chunks is the size of the memory pool, naturally
//		available = N;
//
//		// first node is not used, convinience
//		a[0] = NULL;
//
//		// all other memory chunks are free, the pointers are initialized
//		for (size_type i = 1; i <= N; ++i) {
//			a[i] = reinterpret_cast<T*>(&buf[sizeof(T) * (i - 1)]);
//		}
//	}
//
//	void* allocate()
//	{
//		// allocation not possible if the memory pool has no more space
//		if (available <= 0) throw std::bad_alloc();
//
//		// the first memory chunk is always on index 1, this is guaranteed
//		// by the heap
//		T* t = a[1];
//
//		// move the last free node to front
//		a[1] = a[available];
//
//		// mark the used chunk
//		a[available] = NULL;
//
//		// number of free chunks decreased
//		--available;
//
//		// book keeping for the heap
//		down();
//
//		// return a pointer to the allocated space
//		return t;
//	}
//
//	void deallocate(void* p)
//	{
//		// invalid pointers are ignored
//		if (!p || available >= N) return;
//
//		// memory is freed, more space within the pool
//		++available;
//
//		// the freed node is, well, free. status and pointer are restored.
//		a[available] = reinterpret_cast<T*>(p);
//
//		// book keeping for the heap
//		up();
//	}
//};

