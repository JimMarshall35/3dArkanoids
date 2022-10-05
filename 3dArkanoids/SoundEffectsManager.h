#pragma once
#include "Event.h"
#include "SoundEffectEventArgs.h"
#include "BallComboEventArgs.h"
#include <memory>
class IAudioPlayer;
class SoundEffectsManager
	:public EventListener<SoundEffectEventArgs>,
	public EventListener<BallComboEventArgs>
{
public:
	SoundEffectsManager(const std::shared_ptr<IAudioPlayer> audioPlayer);
private:
	// Inherited via EventListener
	virtual void OnEvent(SoundEffectEventArgs e) override;
	std::shared_ptr<IAudioPlayer> m_audioPlayer;

	// Inherited via EventListener
	virtual void OnEvent(BallComboEventArgs e) override;
};

