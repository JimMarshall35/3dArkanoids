#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <memory>


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

	void SaveToFile(std::string filePath);
	void LoadFromFile(std::string filePath);
};

template<typename T>
inline Array3D<T>::Array3D(size_t w, size_t h, size_t d)
{
	allocate(w, h, d);
}

template<typename T>
inline Array3D<T>::~Array3D()
{
	if(_ptr != nullptr)
		free();
}

template<typename T>
inline void Array3D<T>::allocate(size_t w, size_t h, size_t d)
{
	if (_ptr != nullptr) free();
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

template<typename T>
inline void Array3D<T>::SaveToFile(std::string filePath)
{
	// allocate buffer to be written to file, layed out as follows:
	// width, height, depth, array data
	const auto outputBufferSize = (sizeof(size_t) * 3) + (sizeof(T) * _w * _d * _h);
	auto outputBuffer = std::make_unique<char>(outputBufferSize);

	// copy class data to the staging buffer
	memcpy(outputBuffer.get()                     , &_w, sizeof(size_t));
	memcpy(outputBuffer.get() + sizeof(size_t)    , &_h, sizeof(size_t));
	memcpy(outputBuffer.get() + sizeof(size_t) * 2, &_d, sizeof(size_t));
	memcpy(outputBuffer.get() + sizeof(size_t) * 3, _ptr, sizeof(T) * _w * _d * _h);

	// write to file
	std::ofstream file(filePath, std::ios::out | std::ios::binary);
	file.write(outputBuffer.get(), outputBufferSize);
}

template<typename T>
inline void Array3D<T>::LoadFromFile(std::string filePath)
{
	// open file
	std::ifstream is(filePath, std::ios::in | std::ios::binary);

	// get length
	is.seekg(0, is.end);
	int fileLength = is.tellg();
	is.seekg(0, is.beg);

	// allocate buffer for the entire files contents and write files contents into it
	auto inputBuf = std::make_unique<char[]>(fileLength);
	is.read(inputBuf.get(), fileLength);

	// read width, height and depth, from the file into local vars
	size_t w, h, d;
	memcpy(&w, inputBuf.get()                     , sizeof(size_t));
	memcpy(&h, inputBuf.get() + sizeof(size_t)    , sizeof(size_t));
	memcpy(&d, inputBuf.get() + sizeof(size_t) * 2, sizeof(size_t));

	// allocate using the w, h and d obtained from the file
	allocate(w, h, d);

	// copy array data into newly allocated array
	memcpy(_ptr, inputBuf.get() + sizeof(size_t) * 3, sizeof(T) * w * h * d);
}
