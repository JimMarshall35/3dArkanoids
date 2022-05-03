#pragma once
template <typename T>
class Array3D;

class MasterArrayUpdatedEventArgs {
public:
	MasterArrayUpdatedEventArgs(const Array3D<unsigned char>* newArray, size_t fallingBlockManagerBlockIndex) {
		m_newArray = newArray;
		m_fallingBlockManagerBlockIndex = fallingBlockManagerBlockIndex;
	}

	/// <summary>
	/// getter
	/// </summary>
	/// <returns>m_fallingBlockManagerBlockIndex</returns>
	size_t GetFallingBlockManagerBlockIndex() {
		return m_fallingBlockManagerBlockIndex;
	}

	/// <summary>
	/// getter
	/// </summary>
	/// <returns>m_newArray</returns>
	const Array3D<unsigned char>* GetPlayFieldArray() {
		return m_newArray;
	}

private:

	/// <summary>
	/// pointer to the master Playfield array
	/// that has been changed to a new state
	/// </summary>
	const Array3D<unsigned char>* m_newArray;

	/// <summary>
	/// Index in FallingBlockManager's array
	/// so that the slot can be freed
	/// </summary>
	size_t m_fallingBlockManagerBlockIndex;
};