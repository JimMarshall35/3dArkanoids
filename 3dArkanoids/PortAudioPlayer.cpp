#include "PortAudioPlayer.h"
#include <iostream>
#include "audiofile.h"
#include "TextFileResourceListParser.h"


AudioRingBuffer<float> PortAudioPlayer::s_soundFxBuffer(SOUNDFX_BUFFER_SIZE_SAMPLES);

PortAudioPlayer::PortAudioPlayer(const AudioPlayerInitialisationData& initData)
{
    auto err = Pa_Initialize();
    if (err != paNoError) {
        std::cout << "port audio initialization error" << std::endl;
        return;
    }
    PaStream* stream;
    err = Pa_OpenDefaultStream(
        &stream,
        0,          /* no input channels */
        NUMCHANNELS,          /* stereo output */
        paFloat32,  /* 32 bit floating point output */
        SAMPLE_RATE,
        paFramesPerBufferUnspecified,        /* frames per buffer, i.e. the number of sample frames that PortAudio will request from the callback. Many apps may want to use paFramesPerBufferUnspecified, which tells PortAudio to pick the best, possibly changing, buffer size.*/
        paSoundFxCallback, /* this is your callback function */
        nullptr /*This is a pointer that will be passed to your callback*/
    );
    if (err != paNoError) {
        std::cout << "Pa_OpenDefaultStream error" << std::endl;
        return;
    }
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cout << "Pa_StartStream error" << std::endl;
        return;
    }
    ZzFX_Init(200, SAMPLE_RATE);
    LoadClipsFromFile(initData.samplesPath);
};

PortAudioPlayer::~PortAudioPlayer()
{
    ZzFX_TearDown();
}

void PortAudioPlayer::LoadClip(std::string path, std::string identifier)
{
    // load from file
    AudioFile<float> af;
    af.load(path);
    assert(af.getSampleRate() == SAMPLE_RATE);
    int numSamples = af.getNumSamplesPerChannel();

    // if source is mono load the same sample into left and right channels
    int secondInterleafChannel = 1;
    if (af.isMono())
        secondInterleafChannel = 0;

    // interleaf the left and right channels and store in _audioDataMap
    auto samples = std::vector<float>(numSamples * 2);
    for (int i = 0; i < numSamples; i++) {
        samples[i * 2] = af.samples[0][i];
        samples[(i * 2) + 1] = af.samples[secondInterleafChannel][i];
    }
    _audioDataMap[identifier] = samples;
}

bool PortAudioPlayer::PlayClip(std::string id)
{
    return PlayClip(id, 1.0f);
}

bool PortAudioPlayer::DeleteClip(std::string id)
{
    if (_audioDataMap.find(id) == _audioDataMap.end()) {
        return false;
    }
    _audioDataMap.erase(id);
    //_idGenerator.DeleteId(id);
    return true;
}

void PortAudioPlayer::LoadClipsFromFile(std::string path)
{
    ParseResourcesTextFile(path, [this](std::string path, std::string name) {
        LoadClip(path, name);
    });
}

int PortAudioPlayer::paSoundFxCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
    float* out = (float*)outputBuffer;

    s_soundFxBuffer.ReadBlock(out, framesPerBuffer * NUMCHANNELS);

    return 0;
}

bool PortAudioPlayer::PlayClip(std::string id, float volume)
{
    if (_audioDataMap.find(id) == _audioDataMap.end()) {
        std::cerr << "clip not found \n";
        return false;
    }
    s_soundFxBuffer.WriteBlock(_audioDataMap[id].data(), _audioDataMap[id].size(), volume);
    return true;
}

