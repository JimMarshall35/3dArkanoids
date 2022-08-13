#pragma once
#include <string>
#include "ISerializable.h"
#include <vector>
class GameInput;
class IRenderer;
class Camera;
#define NUM_BAT_SERIALIZABLE_PROPERTIES 3
class Bat
	:public ISerializable
{
public:
	Bat();
	void RecieveInput(const GameInput& input);
	void SetMinAndMaxXPos(double xMin, double xMax);
	void Draw(const IRenderer* renderer, const Camera& cam) const;
	void SaveToFile(std::string path) const override;
	void LoadFromFile(std::string path) override;
	virtual const std::vector<SerializableProperty>& GetSerializableProperties() override;
	virtual bool SetSerializableProperty(const SerializableProperty& p);
	virtual int GetNumSerializableProperties() const override;
private:
	double m_xPos;
	double m_xMinPos;
	double m_xMaxPos;
	float m_batWidth;
	float m_distanceFromFirstRow = 6;
	double m_sensitivity = 0.2;

	// Inherited via ISerializable
};

