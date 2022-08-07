#pragma once
class GameInput;
class IRenderer;
class Camera;

class Bat
{
public:
	Bat(float batwWidth);
	void RecieveInput(const GameInput& input);
	void SetMinAndMaxXPos(double xMin, double xMax);
	void Draw(const IRenderer* renderer, const Camera& cam) const;
private:
	double m_xPos;
	double m_xMinPos;
	double m_xMaxPos;
	float m_batWidth;
	float m_distanceFromFirstRow = 6;
	double m_sensitivity = 0.2;
};

