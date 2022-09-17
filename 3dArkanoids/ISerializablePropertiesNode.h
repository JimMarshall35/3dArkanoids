#pragma once
#include <vector>
#include <string>

class SerializableProperty;
class ISerializablePropertiesNode {
public:
	virtual const std::vector<SerializableProperty>& GetSerializableProperties() const = 0;
	virtual bool SetSerializableProperty(const SerializableProperty& p) = 0;
	virtual int GetNumSerializableProperties() const = 0;
	virtual std::string GetSerializableNodeName() const = 0;
};
