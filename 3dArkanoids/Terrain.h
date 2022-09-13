#pragma once
#include "Array3d.h"
#include "typedefs.h"
#include <string>
#include <functional>
#include <fstream>

#define TRIANGULATION_CELL_SIZE 16


typedef std::function<i8(f64, f64, f64)> TerrainDensityFunction;

i8 TestTerrainDensityFunction(f64 x, f64 y, f64 z);

class Terrain
{
public:
	Terrain();
	void GenerateTerrainVoxelFieldFile(std::string filePath, double WorldSpaceUnitsPerVoxelUnit, u32 voxelsWidth, u32 voxelsHeight, u32 voxelsDepth, TerrainDensityFunction getFieldValue);
	bool OpenStreamToTerrainVoxelsFile(std::string filePath);
	void TriangulateCube(std::vector<float>& output, const glm::ivec3& whichCube);
	void StreamCube(const glm::ivec3& index);
private:
	Array3D<i8> m_loadedCellsToTriangulate = Array3D<i8>(AUTOLIST_CTOR_OPTIONS::DO_NOT_ADD_TO_LIST);
	std::ifstream m_voxelsFileStream;
	bool m_steamOpen = false;
};

