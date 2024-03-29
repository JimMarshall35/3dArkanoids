#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include "ISerializable.h"


template<typename ResponseT>
class Array3D : public ISerializable{
private:
	ResponseT* _ptr;
	size_t _w=0, _h=0, _d=0;
	bool _copied = false;
	
public:
	class xySlice {
	public:
		xySlice(int x, int y, const Array3D* array3d)
			:m_array3D(array3d), m_x(x), m_y(y){}

		ResponseT& operator[](size_t z) {
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
	Array3D(const Array3D& old_obj) {
		_w = old_obj._w;
		_h = old_obj._h;
		_d = old_obj._d;
		_ptr = old_obj._ptr;
		old_obj._copied = true;
		
	}
	Array3D(AUTOLIST_CTOR_OPTIONS options) :ISerializable(options) {}
	Array3D(size_t w, size_t h, size_t d);
	~Array3D();
	void allocate(size_t w, size_t h, size_t d);
	ResponseT* getPtr() const { return _ptr; }
	size_t getW() const { return _w; }
	size_t getH() const { return _h; }
	size_t getD() const { return _d; }
	ResponseT& At(size_t x, size_t y, size_t z) const;
	ResponseT& At(const glm::ivec3& at) const { return At(at.x, at.y, at.z); }
	ResponseT& operator [](const glm::ivec3& at) const { return At(at.x, at.y, at.z);}
	xSlice operator[](int xCoord) const { return xSlice(xCoord, this); }
	void free();

	virtual void SaveToFile(std::string filePath)const override;
	virtual void LoadFromFile(std::string filePath) override;
	virtual char* SaveToBuffer(char* destination)const override;
	virtual const char* LoadFromBuffer(const char* source) override;
	virtual bool SetSerializableProperty(const SerializableProperty& p) override;
	virtual int GetNumSerializableProperties() const override;
	virtual const std::vector<SerializableProperty>& GetSerializableProperties() const;
	virtual std::string GetSerializableNodeName() const override;
	virtual size_t GetBinaryFileNumBytes() const override;

	/*
	* pass an opened file stream that is set to the start of a saved array3d.
	* 
	*/
	void StreamAlreadyAllocatedArrayFromFile(std::ifstream& stream, const glm::ivec3& originAt, unsigned int offsetOfArrayFromStartOfFile = 0);
};

template<typename ResponseT>
inline Array3D<ResponseT>::Array3D(size_t w, size_t h, size_t d)
{
	allocate(w, h, d);
}

template<typename ResponseT>
inline Array3D<ResponseT>::~Array3D()
{
	if(_ptr != nullptr && !_copied)
		free();
}

template<typename ResponseT>
inline void Array3D<ResponseT>::allocate(size_t w, size_t h, size_t d)
{
	if (_ptr != nullptr) free();
	_w = w; _h = h; _d = d;
	_ptr = new ResponseT[w * h * d];
	memset(_ptr, 0x00, w * h * d * sizeof(ResponseT));
}

template<typename ResponseT>
inline ResponseT& Array3D<ResponseT>::At(size_t x, size_t y, size_t z) const
{
	size_t index = x * _h * _d + y * _d + z;
	return _ptr[index];
}

template<typename ResponseT>
inline void Array3D<ResponseT>::free()
{
	delete[] _ptr;
}

template<typename ResponseT>
inline void Array3D<ResponseT>::SaveToFile(std::string filePath) const
{
	// allocate buffer to be written to file, layed out as follows:
	// width, height, depth, array data
	const auto outputBufferSize = (sizeof(size_t) * 3) + (sizeof(ResponseT) * _w * _d * _h);
	auto outputBuffer = std::make_unique<char>(outputBufferSize);

	SaveToBuffer(outputBuffer.get());

	// write to file
	std::ofstream file(filePath, std::ios::out | std::ios::binary);
	file.write(outputBuffer.get(), outputBufferSize);
}

template<typename ResponseT>
inline void Array3D<ResponseT>::LoadFromFile(std::string filePath)
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

	LoadFromBuffer(inputBuf.get());
}

#define ARRAY3D_NUM_SERIALIZABLE_PROPERTIES 4
template<typename ResponseT>
inline const std::vector<SerializableProperty>& Array3D<ResponseT>::GetSerializableProperties() const
{
	static std::vector<SerializableProperty> props(ARRAY3D_NUM_SERIALIZABLE_PROPERTIES);
	props[0].name = "Data";
	props[0].type = SerializablePropertyType::Bytes;
	props[0].data.SizeIfApplicable = sizeof(ResponseT) * _w * _d * _h;
	props[0].data.dataUnion.Bytes = (char*)_ptr;

	props[1].name = "Width";
	props[1].type = SerializablePropertyType::Uint32;
	props[1].data.dataUnion.Uint32 = _w;
	
	props[2].name = "Height";
	props[2].type = SerializablePropertyType::Uint32;
	props[2].data.dataUnion.Uint32 = _h;

	props[3].name = "Depth";
	props[3].type = SerializablePropertyType::Uint32;
	props[3].data.dataUnion.Uint32 = _d;

	return props;
}
template<typename ResponseT>
inline bool Array3D<ResponseT>::SetSerializableProperty(const SerializableProperty& p) {
	if (p.name == "Data") {
		_ptr = (ResponseT*)p.data.dataUnion.Bytes;
		return true;
	}
	else if (p.name == "Width") {
		_w = p.data.dataUnion.Uint32;
		return true;
	}
	else if (p.name == "Height") {
		_h = p.data.dataUnion.Uint32;
		return true;
	}
	else if (p.name == "Depth") {
		_d = p.data.dataUnion.Uint32;
		return true;
	}
	return false;
}

template<typename ResponseT>
inline int Array3D<ResponseT>::GetNumSerializableProperties() const
{
	return ARRAY3D_NUM_SERIALIZABLE_PROPERTIES;
}

template<typename ResponseT>
std::string Array3D<ResponseT>::GetSerializableNodeName() const {
	return "Array3D";
}

template<typename ResponseT>
inline char* Array3D<ResponseT>::SaveToBuffer(char* destination) const
{
	// copy class data to the staging buffer
	memcpy(destination, &_w, sizeof(size_t));
	memcpy(destination + sizeof(size_t), &_h, sizeof(size_t));
	memcpy(destination + sizeof(size_t) * 2, &_d, sizeof(size_t));
	memcpy(destination + sizeof(size_t) * 3, _ptr, sizeof(ResponseT) * _w * _d * _h);
	return destination + sizeof(size_t) * 3 + sizeof(ResponseT) * _w * _d * _h;
}

template<typename ResponseT>
inline const char* Array3D<ResponseT>::LoadFromBuffer(const char* source)
{
	// read width, height and depth, from the file into local vars
	size_t w, h, d;
	memcpy(&w, source, sizeof(size_t));
	memcpy(&h, source + sizeof(size_t), sizeof(size_t));
	memcpy(&d, source + sizeof(size_t) * 2, sizeof(size_t));

	// allocate using the w, h and d obtained from the file
	allocate(w, h, d);

	// copy array data into newly allocated array
	memcpy(_ptr, source + sizeof(size_t) * 3, sizeof(ResponseT) * w * h * d);

	return source + sizeof(size_t) * 3 + sizeof(ResponseT) * w * h * d;

}

template<typename ResponseT>
inline size_t Array3D<ResponseT>::GetBinaryFileNumBytes() const {
	return sizeof(size_t) * 3 + sizeof(ResponseT) * _w * _d * _h;
}

template<typename ResponseT>
void Array3D<ResponseT>::StreamAlreadyAllocatedArrayFromFile(std::ifstream& stream, const glm::ivec3& originAt, unsigned int offsetOfArrayFromStartOfFile) {
	size_t originIndex = originAt.x * _h * _d + originAt.y * _d + originAt.z;
	auto onVoxel = glm::ivec3();
	for (int z = 0; z < _d; z++) {
		for (int y = 0; y < _h; y++) {
			for (int x = 0; x < _w; x++) {
				onVoxel = originAt + glm::ivec3{ x,y,z };
				size_t index = onVoxel.x * _h * _d + onVoxel.y * _d + onVoxel.z;
				stream.seekg(
					offsetOfArrayFromStartOfFile +
					sizeof(size_t) * 3 + // the three fields are saved first see save to buffer function
					index * sizeof(ResponseT),
					std::ios::beg
				);
				stream.read((char*)&At(x, y, z), sizeof(ResponseT));
			}
		}
	}
}
