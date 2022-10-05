#pragma once
#include <string>
using AudioClipID = unsigned int;
class IAudioPlayer {
public:
	virtual void LoadClip(std::string path, std::string identifier) = 0;
	virtual bool PlayClip(std::string id) = 0;
	virtual bool PlayClip(std::string id, float volume) = 0;
	virtual bool DeleteClip(std::string id) = 0;
};