#include "MockLevelLoader.h"
#include "PlayfieldDefs.h"
#include "MockLoaderLevelData.h"
#include "Array3d.h"
#include <iostream>

bool MockLevelLoader::LoadLevel(Array3D<unsigned char>& output, const std::string& filePath)
{
	int z = 0;
	int y = 0;
	for (int i = 0; i < MockLevelDataYRows * MockLevelDataZRows; i++) {
		for (int x = 0; x < MockLevelDataXRows; x++) {
			
			output.At(x, y, z) = MockLevelData[i][x];
			//auto val = output.At(x, y, z);

		}
		y++;
		if (y >= MockLevelDataYRows) {
			y = 0;
			z++;
		}
	}
	return true;
}
