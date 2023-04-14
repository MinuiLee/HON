#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
/******************************************************************************/
/*!
\headerfile   AudioManager.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\brief
		Header file for Audio Manager class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <fmod.hpp>
#include <vector>

#include "ResourceManager.h"

typedef unsigned int SOUNDID;
constexpr SOUNDID SFX_LIMIT		 = 3;
constexpr SOUNDID BACKGROUND_LIMIT = 1;

struct SOUNDM;

class AudioManager : public ResourceManager<unsigned, AudioManager>
{
public:
	AudioManager();
	~AudioManager();

	unsigned LoadSound(const std::string & path);
	unsigned * Load(const std::string & path);

	void Update();
	void PlaySoundM(const std::string name, bool loop = false);
	void PauseSound(const std::string name);
	void ResumeSound(const std::string name);
	void StopSound(const std::string name);
	void ToggleMute();
	void SetSFXVolume(float volume);
	void SetBackgroundVolume(float volume);

	bool IsPlaying(const std::string name);
	bool IsMute();
	float GetSFXVolume() { return m_SFXVolume; }
	float GetBgVolume() { return m_BgVolume; }
	std::vector<SOUNDM *> m_Sounds;

private:
	FMOD::System * m_System = nullptr;
	FMOD::ChannelGroup * m_soundEffects = nullptr;
	FMOD::ChannelGroup * m_backgroundSounds = nullptr;
	FMOD::ChannelGroup * m_masterChannel = nullptr;
	bool m_isMute = false;
	float m_SFXVolume = 1.f, m_BgVolume = 1.f;
};

struct SOUNDM
{
	FMOD::Sound * sound = nullptr;
    FMOD::Channel * channel[SFX_LIMIT] = {nullptr, nullptr, nullptr};
	bool IsPlaying					   = false;
};

#endif