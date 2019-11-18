#pragma once
#include <vector>

template <class MyType> class ObjectPool
{
protected:
	std::vector<MyType> m_Objects;

public:
	ObjectPool()
	{
	}

	virtual ~ObjectPool()
	{
		while (m_Objects.empty() == false)
		{
			delete m_Objects.back();
			m_Objects.pop_back();
		}

        m_Objects.clear();
	}

	void AddNewObjectToPool(MyType object)
	{
		m_Objects.push_back(object);
	}

	MyType GetObjectFromPool()
	{
		if (m_Objects.empty())
			return nullptr;

		MyType pObject = m_Objects.back();
		m_Objects.pop_back();
		return pObject;
	}

	void ReturnObject(MyType object)
	{
		m_Objects.push_back(object);
	}
};