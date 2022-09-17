#pragma once
#include <string>
struct SerializableProperty;
void DebugPrintAllSerializableThings();

void SaveSerializableToSingleBigBinary(std::string filePath);

void LoadSerializableFromSingleBigBinary(std::string filePath);

void SetSerializableProperty(std::string path, const SerializableProperty& value);