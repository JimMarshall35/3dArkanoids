#pragma once
class IRenderer;
class Camera;

class Ball
{
public:
	void Draw(const IRenderer* renderer, const Camera& cam) const;
	void Update(double deltaTime);
};

