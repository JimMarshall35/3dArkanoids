#ifndef ZZFX
#define ZZFX


typedef struct {
    float volume;
    float randomness;
    float frequency;
    float attack;
    float sustain;
    float release;
    enum WAVE_SHAPE {
        SIN,
        TRI,
        SAW,
        TAN,
        NOISE
    }shape;
    int shapeCurve;
    float slide;
    float deltaSlide;
    float pitchJump;
    float pitchJumpTime;
    float repeatTime;
    float noise;
    float modulation;
    unsigned int bitCrush;
    float delay;
    float sustainVolume;
    float decay;
    float tremolo;
} ZzFX_Sound;

typedef struct {
    float* buffer;
    int buffer_size;
} ZzFX_SoundSamples;

ZzFX_SoundSamples ZzFX_BuildSamples(ZzFX_Sound sound);
void ZzFX_FreeBuiltSamples(ZzFX_SoundSamples* samples);
void ZzFX_Init(int randomTableSize, int sampleRate);
void ZzFX_TearDown();
ZzFX_Sound ZzFX_GetDefaultSound();
#endif
