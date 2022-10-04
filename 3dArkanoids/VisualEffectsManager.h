#pragma once
#include "Event.h"
#include "BallComboEventArgs.h"
#include "EngineUpdateFrameEventArgs.h"
#include <glm/glm.hpp>

#define COMBO_NOTIFICATION_POOL_SIZE 10

class IRenderer;
class Camera;

class VisualEffectsManager
	:public EventListener<BallComboEventArgs>,
	public EventListener<EngineUpdateFrameEventArgs>
{
public:
	// Inherited via EventListener
	virtual void OnEvent(BallComboEventArgs e) override;
	// Inherited via EventListener
	virtual void OnEvent(EngineUpdateFrameEventArgs e) override;

	void DrawComboNotifications(const IRenderer* renderer, const Camera& camera) const;
private:
	void PushStartedComboNotification(const glm::vec3& pos, int sizeOfCombo);
private:
	struct ComboNotification {
		glm::vec3 pos;
		glm::vec3 startPos;
		glm::vec3 endPos;
		double timer = 0.0f;
		double totalTime = 1.5f;
		bool active = false;
		glm::vec2 billboardSize = { 10,10 };
		std::string spriteIdentifier;
	};
	ComboNotification m_comboNotificationPool[COMBO_NOTIFICATION_POOL_SIZE];
	const glm::vec3 NOTIFICATION_ENDPOINT_ADDITION = { 0, 0, 30 };
	std::string GetIdentifierForComboNumber(int combo);
};

