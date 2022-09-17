#pragma once

#include "AutoList.h"
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "SerializableProperty.h"
#include "ISerializablePropertiesNode.h"



class ISerializable :public ISerializablePropertiesNode, public AutoList<ISerializable> {
public:
	virtual void SaveToFile(std::string filePath) const = 0;
	virtual void LoadFromFile(std::string filePath) = 0;
	virtual char* SaveToBuffer(char* destinaion) const = 0;
	virtual const char* LoadFromBuffer(const char* source) = 0;
	virtual size_t GetBinaryFileNumBytes() const = 0;

protected:
	ISerializable()
		:AutoList<ISerializable>() {
	}
	ISerializable(AUTOLIST_CTOR_OPTIONS options)
		:AutoList<ISerializable>(options) {
		// ctor to be used when you don't want to be auto-listed
	}
};



