#pragma once
#include <string>
#include <glm/glm.hpp>
template<typename T>
class Array3D;
/*
message EditBlockResult{
	enum Result{
		SPACE_EMPTY = 0;
		BLOCK_AT_SPACE = 1;
		FAILURE_POINT_OUT_OF_BOUNDS = 2;
		OTHER_FAILURE = 3;
	}
	Result result = 1;
	int32 blockCode = 2;
	string errorMessage = 3;
}
*/

enum SetBoardDescriptionResultCPP {
	SUCCESS = 0,
	FAILURE_TOO_MANY_BYTES_FOR_WHD = 1,
	FAILURE_TOO_FEW_BYTES_FOR_WHD = 2
};


enum EditBlockResultCode {
	SPACE_EMPTY = 0,
	BLOCK_AT_SPACE = 1,
	FAILURE_POINT_OUT_OF_BOUNDS = 2,
	OTHER_FAILURE = 3,
	NO_CHANGE = 4,
	INVALID_NEW_BYTE = 5
};
struct EditBlockResultCPP {
	EditBlockResultCode resultCode;
	unsigned char blockCode;
	std::string errorMessage;
};

struct BlockTypeDescription {
	unsigned char GameEngineBlockCode;
	glm::vec3 Rgb;
};

class ILevelEditorServerGame {
	public:
		virtual const Array3D<unsigned char>& GetBoardState() = 0;
		virtual EditBlockResultCode AddOrChangeBlock(const glm::ivec3& point, unsigned char newVal, unsigned char& oldBlockVal) = 0;
		virtual EditBlockResultCode RemoveBlock(const glm::ivec3& point) = 0;
		virtual SetBoardDescriptionResultCPP SetBoardState(const Array3D<unsigned char>& newState) = 0;
		virtual EditBlockResultCode BlockAtLocation(const glm::ivec3& point, unsigned char& blockCode) = 0;
		virtual std::vector<BlockTypeDescription> GetPossibleBlocks() = 0;
};