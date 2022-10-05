#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "zzfx.h"

#define PI 3.14159f
#define PI2 (PI*2)

static float gSampleRate = 44100.0;

/* get a random float using a pre generated LUT */
static float* gRandomTable = (void*)0;
static int gRandomTableSize = 0;
static int gNextRandomTableValueIndex = 0;

inline float RandomUsingTable() {
    float r = gRandomTable[gNextRandomTableValueIndex++];
    if (gNextRandomTableValueIndex == gRandomTableSize) {
        gNextRandomTableValueIndex = 0;
    }
    return r;
}
void InitRandomTable_StdLib(int randomTableSize) {
    gRandomTable = malloc(randomTableSize * sizeof(float));
    srand(time(NULL));
    for (int i = 0; i < randomTableSize; i++) {
        gRandomTable[i] = RandomFloat();
    }
}

/* trigonometry LUTs  NOT IMPLEMENTED THOUGH! */
float* gSinTable = (void*)0;
int gSinTableSize = 0;
void PopulateSinLUT() {
    for (int i = 0; i < gSinTableSize; i++) {
        gSinTable[0] = (float)sin(((double)i / (double)gSinTableSize) * PI2);
    }
}

inline int sign(float v) {
    return v > 0 ? 1 : -1;
}

ZzFX_SoundSamples ZzFX_BuildSamples(ZzFX_Sound sound) {
    
    float startSlide = sound.slide *= 500.0f * PI2 / gSampleRate / gSampleRate;
    float startFrequency = sound.frequency *=
        (1 + sound.randomness * 2 * RandomUsingTable() - sound.randomness) * PI2 / gSampleRate;
    
    // scale by sample rate
    sound.attack = sound.attack * gSampleRate + 9; // minimum sample rate to prevent popping
    sound.decay *= gSampleRate;
    sound.sustain *= gSampleRate;
    sound.release *= gSampleRate;
    sound.delay *= gSampleRate;
    sound.deltaSlide *= 500.0f * PI2 / (float)pow((double)gSampleRate, 3.0f);
    sound.modulation *= PI2 / gSampleRate;
    sound.pitchJump *= PI2 / gSampleRate;
    sound.pitchJumpTime *= gSampleRate;
    sound.repeatTime = sound.repeatTime * gSampleRate;
    
    int length = sound.attack + sound.delay + sound.sustain + sound.release + sound.delay;
    ZzFX_SoundSamples built;
    built.buffer_size = length;
    built.buffer = malloc(sizeof(float) * length);

    float s = 0.0f;
    int c = 0;
    int tm = 0;
    int r = 0;
    int j = 1;
    float t = 0;
    float f;
    for (int i = 0; i < length; built.buffer[i++] = s) {
        if (!fmod((double)++c, ((double)sound.bitCrush * 100.0))) {                      // bit crush
            s = sound.shape? sound.shape>1? sound.shape>2? sound.shape>3?         // wave shape
                    sin(pow(fmod((double)t, (double)PI2),3)) :                    // 4 noise
                    max(min(tan(t),1),-1):     // 3 tan
                    1-fmod(fmod((double)2* (double)t/ (double)PI2, (double)2+ (double)2), (double)2):                        // 2 saw
                    1-4*abs(round(t/PI2)-t/PI2):    // 1 triangle
                    sin(t);                              // 0 sin

            s = (sound.repeatTime ?
                    1 - sound.tremolo + sound.tremolo*sin(PI2*i/ sound.repeatTime) // tremolo
                    : 1) *
                sign(s)*pow(abs(s),sound.shapeCurve) *       // curve 0=square, 2=pointy
                sound.volume * (                  // envelope
                i < sound.attack ? i/ sound.attack :                   // attack
                i < sound.attack + sound.decay ?                      // decay
                1-((i- sound.attack)/ sound.decay)*(1- sound.sustainVolume) :  // decay falloff
                i < sound.attack  + sound.decay + sound.sustain ?           // sustain
                    sound.sustainVolume :                           // sustain volume
                i < length - sound.delay ?                      // release
                (length - i - sound.delay)/ sound.release *            // release falloff
                    sound.sustainVolume :                           // release volume
                0);                                       // post release

            s = sound.delay ? s/2 + (sound.delay > i ? 0 :            // delay
                (i<length- sound.delay? 1 : (length-i)/ sound.delay) *  // release delay 
                built.buffer[i- (int)sound.delay|0]/2) : s;                      // sample delay
        }

        f = (sound.frequency += sound.slide += sound.deltaSlide) *          // frequency
            cos(sound.modulation * tm++);                    // modulation
        t += f - f * sound.noise * fmod(1 - (sin(i) + 1) * 1e9, 2);     // noise

        if (j && (++j > sound.pitchJumpTime))       // pitch jump
        {
            sound.frequency += sound.pitchJump;         // apply pitch jump
            startFrequency += sound.pitchJump;    // also apply to start
            j = 0;                          // stop pitch jump time
        }

        if (sound.repeatTime && !fmod(++r , sound.repeatTime)) // repeat
        {
            sound.frequency = startFrequency;     // reset frequency
            sound.slide = startSlide;             // reset slide
            j = j || 1;                     // reset pitch jump time
        }
    }
    return built;
}

float RandomFloat()
{
    float r = (float)rand() / (float)RAND_MAX;
    return r;
}

ZzFX_Sound ZzFX_GetDefaultSound()
{
    ZzFX_Sound r;
    r.volume = 1;
    r.randomness = .05f;
    r.frequency = 220;
    r.attack = 0;
    r.sustain = 0;
    r.release = .1;
    r.shape = SIN;
    r.shapeCurve = 1.0f;
    r.slide = 0;
    r.deltaSlide = 0;
    r.pitchJump = 0;
    r.pitchJumpTime = 0;
    r.repeatTime = 0;
    r.noise = 0;
    r.modulation = 0;
    r.bitCrush = 0;
    r.delay = 0;
    r.sustainVolume = 1;
    r.decay = 0;
    r.tremolo = 0;
    return r;
}


void ZzFX_TearDown()
{
    free(gRandomTable);
}



void ZzFX_Init(int randomTableSize, int sampleRate)
{
    gSampleRate = (float)sampleRate;
    gRandomTableSize = randomTableSize;
    InitRandomTable_StdLib(randomTableSize);
}

void ZzFX_FreeBuiltSamples(ZzFX_SoundSamples* samples) {
    free(samples->buffer);
}