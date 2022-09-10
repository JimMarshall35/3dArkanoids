#pragma once
#include <string>
#include "ISerializable.h"
#include <vector>
#include <glm/glm.hpp>
class GameInput;
class IRenderer;
class Camera;
#define NUM_BAT_SERIALIZABLE_PROPERTIES 3
class Bat
	:public ISerializable
{
public:
	Bat();
	double RecieveInput(const GameInput& input); // returns the change in x position after recieving input
	void SetMinAndMaxXPos(double xMin, double xMax);
	void Draw(const IRenderer* renderer, const Camera& cam) const;
	inline double GetSensitivity() { return m_sensitivity; }
	inline double GetXPos() const { return m_xPos; }
	inline double GetDistanceFromFirstRow() const { return m_distanceFromFirstRow; }
	inline const glm::vec2& GetDepthAndHeight() const { return m_batDepthAndHeight; }
	inline double GetWidth() const { return m_batWidth; }
public:
	void SaveToFile(std::string path) const override;
	void LoadFromFile(std::string path) override;
	virtual char* SaveToBuffer(char* destination) const override;
	virtual const char* LoadFromBuffer(const char* source) override;

	virtual const std::vector<SerializableProperty>& GetSerializableProperties() const override;
	virtual bool SetSerializableProperty(const SerializableProperty& p);
	virtual int GetNumSerializableProperties() const override;
	virtual std::string GetSerializableNodeName() const override;
	virtual size_t GetBinaryFileNumBytes() const override;
private:
	double m_xPos;
	double m_xMinPos;
	double m_xMaxPos;
	float m_batWidth;
	float m_distanceFromFirstRow = 30;
	double m_sensitivity = 0.2;

	glm::vec2 m_batDepthAndHeight = { 3.0f,3.0f };

	// Inherited via ISerializable
};

