#pragma once
#include "IAudioPlayer.h"
#include "portaudio.h"
#include "AudioRingBuffer.h"
#include "IdGenerator.h"
extern "C" {
#include "zzfx.h"

}

#define SAMPLE_RATE 44100
#define SOUNDFX_BUFFER_SIZE_SECONDS 5
#define NUMCHANNELS 2
#define SOUNDFX_BUFFER_SIZE_SAMPLES SOUNDFX_BUFFER_SIZE_SECONDS * SAMPLE_RATE * NUMCHANNELS


using AudioSamplesMap = std::map<std::string, std::vector<float>>;
using SamplesIdPair = std::pair<std::string, std::vector<float>>;

struct AudioPlayerInitialisationData {
    std::string samplesPath;
};

class PortAudioPlayer : public IAudioPlayer
{
public:
    PortAudioPlayer(const AudioPlayerInitialisationData& initData);
    ~PortAudioPlayer();
    // Inherited via IAudioPlayer
    virtual void LoadClip(std::string path, std::string identifier) override;
    virtual bool DeleteClip(std::string id) override;
    virtual bool PlayClip(std::string id) override;

private:
    void LoadClipsFromFile(std::string path);
    static int paSoundFxCallback(const void* inputBuffer, 
                                 void* outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void* userData);
    
private:
    static AudioRingBuffer<float> s_soundFxBuffer;
    AudioSamplesMap _audioDataMap;
    

    // Inherited via IAudioPlayer
    virtual bool PlayClip(std::string id, float volume) override;

};

