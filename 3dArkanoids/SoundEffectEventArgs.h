#pragma once
enum class SoundEffect {
	HitBat,
	HitBlock,
	HitSide,
	Jump
};
struct SoundEffectEventArgs{
	SoundEffect soundEffect;
	double volume = 1.0;
};