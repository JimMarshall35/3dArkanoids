#include "VisualEffectsManager.h"
#include "Camera.h"
#include "IRenderer.h"

void VisualEffectsManager::OnEvent(BallComboEventArgs e)
{
	std::cout << "Ball combo! " << e.numBounces << "\n";
	PushStartedComboNotification({ e.ballX,e.ballY,e.ballZ });
}

void VisualEffectsManager::DrawComboNotifications(const IRenderer* renderer, const Camera& camera) const
{
	for (int i = 0; i < COMBO_NOTIFICATION_POOL_SIZE; i++) {
		const auto& notification = m_comboNotificationPool[i];
		if (!m_comboNotificationPool[i].active) {
			continue;
		}
		renderer->DrawBillboard(notification.pos, notification.billboardSize, camera);
	}
}

void VisualEffectsManager::PushStartedComboNotification(const glm::vec3& pos)
{
	ComboNotification* freeNotification = nullptr;
	for (int i = 0; i < COMBO_NOTIFICATION_POOL_SIZE; i++) {
		auto& notification = m_comboNotificationPool[i];
		if (m_comboNotificationPool[i].active) {
			continue;
		}
		else {
			freeNotification = &notification;
		}
	}

	if (freeNotification == nullptr) {
		std::cout << "combo notification bool is full - consider increasing size" << std::endl;
		return;
	}

	freeNotification->active = true;
	freeNotification->pos = pos;
	freeNotification->startPos = pos;
	freeNotification->endPos = freeNotification->pos + NOTIFICATION_ENDPOINT_ADDITION;
	//freeNotification->

}

void VisualEffectsManager::OnEvent(EngineUpdateFrameEventArgs e)
{
	for (int i = 0; i < COMBO_NOTIFICATION_POOL_SIZE; i++) {
		auto& notification = m_comboNotificationPool[i];
		if (!notification.active) {
			continue;
		}
		notification.timer += e.DeltaTime;
		if (notification.timer >= notification.totalTime) {
			notification.active = false;
			notification.timer = 0.0f;
		}
		else {
			notification.pos = glm::mix(notification.startPos, notification.endPos, (notification.timer / notification.totalTime));
		}
		
	}
}
