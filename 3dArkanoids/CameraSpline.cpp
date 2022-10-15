#include "CameraSpline.h"
#include "Bezier.h"
#include "PlayfieldDefs.h"
#include "IRenderer.h"
#include "Camera.h"

CameraSpline::CameraSpline(Camera* cameraToManage)
{
	m_cameraToManage = cameraToManage;
	m_mapFlyoverCurve = std::make_unique<Bezier>();
	m_mapFlyoverCurve->set_steps(400);
	auto val = 500.0;
	m_mapFlyoverCurve->add_way_point({ 94.96861, -302.21408, 285.76797 });
	m_mapFlyoverCurve->add_way_point({ BLOCK_WIDTH_UNITS * PLAYFIELD_WIDTH_BLOCKS + val, BLOCK_HEIGHT_UNITS * PLAYFIELD_HEIGHT_BLOCKS + val,50 });
	m_mapFlyoverCurve->add_way_point({ -val,BLOCK_HEIGHT_UNITS * PLAYFIELD_HEIGHT_BLOCKS + val,50 });
	m_mapFlyoverCurve->add_way_point({ 94.96861, -302.21408, 285.76797 });
	m_totalFlyoverTime = m_mapFlyoverCurve->total_length() / m_flyoverSpeedDistanceUnitsPerSecond;
}

void CameraSpline::TestVisualiseFlyoverCurve(const Camera& camToDrawUsing, const IRenderer* renderer)
{
	for (int i = 10; i < m_mapFlyoverCurve->node_count() - 10; i++) {
		auto node = m_mapFlyoverCurve->node(i);
		renderer->DrawSphere(
			{ node.x, node.y, node.z },
			{ 4,4,4 },
			camToDrawUsing,
			{ 1.0,0,0,0.2 }
		);
	}
}

void CameraSpline::Reset()
{
	m_flyoverTimeElapsed = 0.0f;
}

inline glm::vec3 cppSplineVecToGlm(const Vector& vec) {
	return glm::vec3(vec.x, vec.y, vec.z);
}
inline Vector glmVecToCppSpline(const glm::vec3& vec) {
	return Vector{ vec.x,vec.y,vec.z };
}

bool CameraSpline::InterpolateCameraAlongCurve(float deltaT)
{
	m_flyoverTimeElapsed += deltaT;
	if (m_flyoverTimeElapsed > m_totalFlyoverTime) {
		m_flyoverTimeElapsed = 0;
		return true;
	}
	auto distanceShouldHaveCovered = m_flyoverTimeElapsed * m_flyoverSpeedDistanceUnitsPerSecond;
	glm::vec3 cameraPos = cppSplineVecToGlm(m_mapFlyoverCurve->node(0));
	glm::vec3 nextNode = cppSplineVecToGlm(m_mapFlyoverCurve->node(1));
	glm::vec3 lastNode = cppSplineVecToGlm(m_mapFlyoverCurve->node(0));
	
	for (int i = 0; i < m_mapFlyoverCurve->node_count(); i++) {
		auto distanceFromStart = m_mapFlyoverCurve->length_from_starting_point(i);
		if (distanceFromStart < distanceShouldHaveCovered) {
			continue;
		}
		if (distanceFromStart == distanceShouldHaveCovered) {
			cameraPos = cppSplineVecToGlm(m_mapFlyoverCurve->node(i));
			lastNode = cameraPos;
			nextNode = cppSplineVecToGlm(m_mapFlyoverCurve->node(i + 1));
			break;
		}
		else if(distanceFromStart > distanceShouldHaveCovered) {
			auto difference = distanceFromStart - m_mapFlyoverCurve->length_from_starting_point(i - 1);
			auto t = distanceShouldHaveCovered - m_mapFlyoverCurve->length_from_starting_point(i - 1);
			auto node0 = cppSplineVecToGlm(m_mapFlyoverCurve->node(i - 1));
			auto node1 = cppSplineVecToGlm(m_mapFlyoverCurve->node(i));
			nextNode = node1;
			lastNode = node0;
			cameraPos = glm::mix(node0, node1, t / difference);
			break;
		}
	}
	auto spotToLookAt = glm::vec3{
		(PLAYFIELD_WIDTH_BLOCKS * BLOCK_WIDTH_UNITS) / 2,
		(PLAYFIELD_HEIGHT_BLOCKS * BLOCK_HEIGHT_UNITS) / 2,
		0
	};
	m_cameraToManage->Position = cameraPos;
	m_cameraToManage->LookCameraAt(spotToLookAt, glm::normalize(nextNode - cameraPos));
	return false;
}
