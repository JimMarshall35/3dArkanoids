#pragma once
#include "Array3d.h"
#include <glm/glm.hpp>
#include <vector>

template<typename T>
class Array3DMap{
private:
	Array3D<T> m_backingArray;
	size_t m_w;
	size_t m_h;
	size_t m_d;
	std::vector<glm::ivec3> m_keys;
	size_t m_numKeys = 0;
public:
	void allocate(size_t w, size_t h, size_t d);
	void add(const glm::ivec3& key, const T& value);
	T& get(const glm::ivec3& key);
	bool hasKey(const glm::ivec3& key);
	const std::vector<glm::ivec3>& GetKeysReference()const;
	size_t GetNumKeys();
};

template<typename T>
inline void Array3DMap<T>::allocate(size_t w, size_t h, size_t d)
{
	m_w = w;
	m_h = h;
	m_d = d;
	m_backingArray.allocate(w, h, d);
	m_keys.resize(w * h * d);
}

template<typename T>
inline void Array3DMap<T>::add(const glm::ivec3& key, const T& value)
{
	if (!hasKey(key)) {
		m_keys[m_numKeys++] = key;
	}
	m_backingArray.At(key.x, key.y, key.z) = value;
}

template<typename T>
inline T& Array3DMap<T>::get(const glm::ivec3& key)
{
	return m_backingArray.At(key.x, key.y, key.z);
}

template<typename T>
inline bool Array3DMap<T>::hasKey(const glm::ivec3& key)
{
	for (int i = 0; i < m_numKeys; i++) {
		if (m_keys[i] == key) {
			return true;
		}
	}
	return false;
}

template<typename T>
inline const std::vector<glm::ivec3>& Array3DMap<T>::GetKeysReference() const
{
	return m_keys;
}

template<typename T>
inline size_t Array3DMap<T>::GetNumKeys()
{
	return m_numKeys;
}
