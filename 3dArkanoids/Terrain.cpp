#include "Terrain.h"
#include "SimplexNoise.h"
#include "transvoxel.h"
#include <memory>

Terrain::Terrain()
{
	m_loadedCellsToTriangulate.allocate(TRIANGULATION_CELL_SIZE + 2, TRIANGULATION_CELL_SIZE + 2, TRIANGULATION_CELL_SIZE + 2);
}

struct VoxelTerrainFileHeader {
	double worldSpaceUnitsPerVoxelUnit;
};

void Terrain::GenerateTerrainVoxelFieldFile(std::string filePath, double worldSpaceUnitsPerVoxelUnit, u32 voxelsWidth, u32 voxelsHeight, u32 voxelsDepth, TerrainDensityFunction getFieldValue)
{
	auto buffer = Array3D<i8>(AUTOLIST_CTOR_OPTIONS::DO_NOT_ADD_TO_LIST);
	buffer.allocate(voxelsWidth, voxelsHeight, voxelsDepth);

	for(int z = 0; z < voxelsDepth; z++)
		for (int y = 0; y < voxelsHeight; y++)
			for (int x = 0; x < voxelsWidth; x++) {
				buffer[x][y][z] = getFieldValue(
					(double)x * worldSpaceUnitsPerVoxelUnit,
					(double)y * worldSpaceUnitsPerVoxelUnit,
					(double)z * worldSpaceUnitsPerVoxelUnit);
			}
	
	auto preparedFileSize = buffer.GetBinaryFileNumBytes() + sizeof(VoxelTerrainFileHeader);

	auto preparedFileBuffer = std::make_unique<char[]>(preparedFileSize);


	VoxelTerrainFileHeader header = { worldSpaceUnitsPerVoxelUnit };
	memcpy(preparedFileBuffer.get(), &header, sizeof(VoxelTerrainFileHeader));
	buffer.SaveToBuffer(preparedFileBuffer.get() + sizeof(VoxelTerrainFileHeader));

	std::ofstream file(filePath, std::ios::out | std::ios::binary);
	file.write(preparedFileBuffer.get(), preparedFileSize);

}

bool Terrain::OpenStreamToTerrainVoxelsFile(std::string filePath)
{
	try
	{
		std::ifstream is(filePath, std::ios::in | std::ios::binary);
		m_steamOpen = true;
		return true;

	}
	catch(...){
		return false;

	}
}

void Terrain::TriangulateCube(std::vector<float>& output, const glm::ivec3& whichCube)
{

	for (int z = 1; z <= 16; z++){
		for (int y = 1; y <= 16; y++) {
			for (int x = 1; x <= 16; x++) {
				i8 corner[] = {
					m_loadedCellsToTriangulate[x    ][y    ][z    ], // 0
					m_loadedCellsToTriangulate[x + 1][y    ][z    ], // 1
					m_loadedCellsToTriangulate[x    ][y + 1][z    ], // 2
					m_loadedCellsToTriangulate[x + 1][y + 1][z    ], // 3
					m_loadedCellsToTriangulate[x    ][y    ][z + 1], // 4
					m_loadedCellsToTriangulate[x + 1][y    ][z + 1], // 5
					m_loadedCellsToTriangulate[x    ][y + 1][z + 1], // 6
					m_loadedCellsToTriangulate[x + 1][y + 1][z + 1], // 7
				};
				unsigned long caseCode = ((corner[0] >> 7) & 0x01)
					| ((corner[1] >> 6) & 0x02)
					| ((corner[2] >> 5) & 0x04)
					| ((corner[3] >> 4) & 0x08)
					| ((corner[4] >> 3) & 0x10)
					| ((corner[5] >> 2) & 0x20)
					| ((corner[6] >> 1) & 0x40)
					| (corner[7] & 0x80);
				if ((caseCode ^ ((corner[7] >> 7) & 0xFF)) != 0)
				{
					// Cell has a nontrivial triangulation.

					// The regularCellClass table maps an 8-bit regular Marching Cubes case index to
					// an equivalence class index. Even though there are 18 equivalence classes in our
					// modified Marching Cubes algorithm, a couple of them use the same exact triangulations,
					// just with different vertex locations. We combined those classes for this table so
					// that the class index ranges from 0 to 15.
					auto c = regularCellClass[caseCode];

					// The RegularCellData structure holds information about the triangulation
					// used for a single equivalence class in the modified Marching Cubes algorithm,
					// described in Section 3.2.
					const RegularCellData& cellData = regularCellData[c];

					// The regularVertexData table gives the vertex locations for every one of the 256 possible
					// cases in the modified Marching Cubes algorithm. Each 16-bit value also provides information
					// about whether a vertex can be reused from a neighboring cell. See Section 3.3 for details.
					// The low byte contains the indexes for the two endpoints of the edge on which the vertex lies,
					// as numbered in Figure 3.7. The high byte contains the vertex reuse data shown in Figure 3.8.
					const unsigned short* vertData = regularVertexData[caseCode];
					u8 edgeCodes[12];
					u8 cornerIndices[12];
					auto vertCount = cellData.GetVertexCount();
					for (int i = 0; i < vertCount; i++) {
						edgeCodes[i] = vertData[i] >> 8;
						cornerIndices[i] = vertData[i] & 0xff;
					}

				}
			}
		}
	}
}

void Terrain::StreamCube(const glm::ivec3& index)
{
	m_loadedCellsToTriangulate.StreamAlreadyAllocatedArrayFromFile(m_voxelsFileStream, index * TRIANGULATION_CELL_SIZE);
}

i8 TestTerrainDensityFunction(f64 x, f64 y, f64 z)
{
	static SimplexNoise noise(64);
	using namespace glm;

	f32 density = -z + 64;
	density += noise.fractal(4, x, y, z);
	
	return i8(clamp(density,-127.0f,127.0f));
}
