#pragma once
#include <string.h>
#include <glm/glm.hpp>


template<typename T>
class Array3D {
private:
	T* _ptr;
	size_t _w, _h, _d;

	
public:
	class xySlice {
	public:
		xySlice(int x, int y, const Array3D* array3d)
			:m_array3D(array3d), m_x(x), m_y(y){}

		T& operator[](size_t z) {
			return m_array3D->At(m_x, m_y, z);
		}
	private:
		const Array3D* m_array3D;
		int m_x;
		int m_y;
	};

	class xSlice {
	public:
		xSlice(int x, const Array3D* array3d)
			:m_array3D(array3d), m_x(x) {}
		
		xySlice operator[](int y) { return xySlice(m_x, y, m_array3D); }
	private:
		const Array3D* m_array3D;
		int m_x;
	};

	Array3D() {}
	Array3D(size_t w, size_t h, size_t d);
	~Array3D();
	void allocate(size_t w, size_t h, size_t d);
	T* getPtr() const { return _ptr; }
	size_t getW() const { return _w; }
	size_t getH() const { return _h; }
	size_t getD() const { return _d; }
	T& At(size_t x, size_t y, size_t z) const;
	T& At(const glm::ivec3& at) const { return At(at.x, at.y, at.z); }
	T& operator [](const glm::ivec3& at) const { return At(at.x, at.y, at.z);}
	xSlice operator[](int xCoord) const { return xSlice(xCoord, this); }
	void free();
};

template<typename T>
inline Array3D<T>::Array3D(size_t w, size_t h, size_t d)
{
	allocate(w, h, d);
}

template<typename T>
inline Array3D<T>::~Array3D()
{
	free();
}

template<typename T>
inline void Array3D<T>::allocate(size_t w, size_t h, size_t d)
{
	_w = w; _h = h; _d = d;
	_ptr = new T[w * h * d];
	memset(_ptr, 0x00, w * h * d * sizeof(T));
}

template<typename T>
inline T& Array3D<T>::At(size_t x, size_t y, size_t z) const
{
	size_t index = x * _h * _d + y * _d + z;
	return _ptr[index];
}

template<typename T>
inline void Array3D<T>::free()
{
	delete[] _ptr;
}
