#pragma once
#include <string>

template<typename T>
class Array3D;

class ILevelLoader {
public:
	virtual bool LoadLevel(Array3D<unsigned char>& output, const std::string& filePath) = 0;
};