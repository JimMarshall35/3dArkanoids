#pragma once
#include "ILevelLoader.h"
class MockLevelLoader : public ILevelLoader
{
public:
	
	// Inherited via ILevelLoader
	virtual bool LoadLevel(Array3D<unsigned char>& output, const std::string& filePath) override;

};

