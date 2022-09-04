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
	void SaveToFile(std::string path) const override;
	void LoadFromFile(std::string path) override;
	inline double GetSensitivity() { return m_sensitivity; }
	inline double GetXPos() { return m_xPos; }
	inline double GetDistanceFromFirstRow() { return m_distanceFromFirstRow; }
	inline const glm::vec2& GetDepthAndHeight() { return m_batDepthAndHeight; }
public:
	virtual const std::vector<SerializableProperty>& GetSerializableProperties() override;
	virtual bool SetSerializableProperty(const SerializableProperty& p);
	virtual int GetNumSerializableProperties() const override;
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

