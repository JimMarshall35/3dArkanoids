#include "SoundEffectsManager.h"
#include "IAudioPlayer.h"

SoundEffectsManager::SoundEffectsManager(const std::shared_ptr<IAudioPlayer> audioPlayer)
	:m_audioPlayer(audioPlayer)
{
}

void SoundEffectsManager::OnEvent(SoundEffectEventArgs e)
{
	switch (e.soundEffect)
	{
	case SoundEffect::HitBat:
		m_audioPlayer->PlayClip("bathit");
		break;
	case SoundEffect::HitBlock:
		m_audioPlayer->PlayClip("destroyblock");
		break;
	case SoundEffect::HitSide:
		m_audioPlayer->PlayClip("hitmetalblock");
		break;
	case SoundEffect::Jump:
		m_audioPlayer->PlayClip("sfx5");
		break;
	default:
		break;
	}
}

void SoundEffectsManager::OnEvent(BallComboEventArgs e)
{
	m_audioPlayer->PlayClip("lasershoot");
}
