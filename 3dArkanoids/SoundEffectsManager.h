#pragma once
#include "Event.h"
#include "SoundEffectEventArgs.h"
#include <memory>
class IAudioPlayer;
class SoundEffectsManager
	:public EventListener<SoundEffectEventArgs>
{
public:
	SoundEffectsManager(const std::shared_ptr<IAudioPlayer> audioPlayer);
private:
	// Inherited via EventListener
	virtual void OnEvent(SoundEffectEventArgs e) override;
	std::shared_ptr<IAudioPlayer> m_audioPlayer;
};

