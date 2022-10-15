#pragma once
#include <memory>
#include "Curve.h"

class Camera;

class IRenderer;
class CameraSpline
{
public:
	CameraSpline(Camera* cameraToManage);
	void TestVisualiseFlyoverCurve(const Camera& camToDrawUsing, const IRenderer* renderer);
	void Reset();
	/// <summary>
	/// interpolate over a curve
	/// </summary>
	/// <param name="deltaT"></param>
	/// <returns>true if finished, false if not</returns>
	bool InterpolateCameraAlongCurve(float deltaT);
private:
	Camera* m_cameraToManage;
	std::unique_ptr<Curve> m_mapFlyoverCurve;
	float m_flyoverTimeElapsed = 0.0f;
	float m_totalFlyoverTime;
	float m_flyoverSpeedDistanceUnitsPerSecond = 350;
};

