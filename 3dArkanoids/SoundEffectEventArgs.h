#pragma once
enum class SoundEffect {
	HitBat,
	HitBlock
};
struct SoundEffectEventArgs{
	SoundEffect soundEffect;
	double volume = 1.0;
};