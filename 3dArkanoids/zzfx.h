#ifndef ZZFX
#define ZZFX


typedef struct {
    double volume;
    double randomness;
    double frequency;
    double attack;
    double sustain;
    double release;
    enum WAVE_SHAPE {
        SIN,
        TRI,
        SAW,
        TAN,
        NOISE
    }shape;
    int shapeCurve;
    double slide;
    double deltaSlide;
    double pitchJump;
    double pitchJumpTime;
    double repeatTime;
    double noise;
    double modulation;
    int bitCrush;
    double delay;
    double sustainVolume;
    double decay;
    double tremolo;
} ZzFX_Sound;

typedef struct {
    float* buffer;
    int buffer_size;
} ZzFX_SoundSamples;

ZzFX_SoundSamples ZzFX_BuildSamples(ZzFX_Sound* sound);
void ZzFX_FreeBuiltSamples(ZzFX_SoundSamples* samples);
void ZzFX_Init(int randomTableSize, int sampleRate);
void ZzFX_TearDown();
ZzFX_Sound ZzFX_GetDefaultSound();
#endif
