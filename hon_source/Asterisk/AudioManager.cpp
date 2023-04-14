/******************************************************************************/
/*!
\file   AudioManager.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "AudioManager.h"

AudioManager::AudioManager() : ResourceManager(fs::current_path().string() + "\\Media\\Sound")
{
	FMOD::System_Create(&m_System);

	int driverCount = 0;
	m_System->getNumDrivers(&driverCount);

	if (driverCount == 0) { throw "The driver's number is 0\n"; }

	m_System->init(32, FMOD_INIT_NORMAL, nullptr);

	m_System->createChannelGroup("soundEffects", &m_soundEffects);
	m_System->createChannelGroup("backgroundSound", &m_backgroundSounds);
	m_masterChannel->addGroup(m_soundEffects);
	m_masterChannel->addGroup(m_backgroundSounds);
	m_System->getMasterChannelGroup(&m_masterChannel);
}

AudioManager::~AudioManager()
{
	for (auto sound : m_Sounds)
		sound->sound->release();

	m_System->release();
	m_System->close();
}

unsigned * AudioManager::Load(const std::string & path)
{
	const auto location = new unsigned(LoadSound(path));

	return location;
}


unsigned AudioManager::LoadSound(const std::string & path)
{
	auto location = m_Sounds.size(); // where new sound will be stored

	SOUNDM * sound = new SOUNDM;
	m_Sounds.push_back(sound);

	if (m_System->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_Sounds[location]->sound) !=
		FMOD_OK)
	{
		m_Sounds.pop_back();
	}

	return static_cast<unsigned>(location);
}

void AudioManager::Update() { m_System->update(); }

void AudioManager::PlaySoundM(const std::string name, bool loop)
{
	const auto id = *this->GetResource(name);

	// Check Channel
	bool // isPaused,
		isPlaying	= false;
	unsigned int num = name.find("BGM") != std::string::npos ? BACKGROUND_LIMIT : SFX_LIMIT;

	unsigned int new_id;
	for (new_id = 0; new_id < num; new_id++)
	{
		m_Sounds[id]->channel[new_id]->isPlaying(&isPlaying);
		if (!isPlaying)
		{
			m_Sounds[id]->IsPlaying = false;
			break;
		}
	}
	FMOD::Sound *& r_sound = m_Sounds[id]->sound;
	if (!loop)
		r_sound->setMode(FMOD_LOOP_OFF);
	else
	{
		r_sound->setMode(FMOD_LOOP_NORMAL);
		r_sound->setLoopCount(-1);
	}

	// Play sound
	if (name.find("BGM") != std::string::npos && !m_Sounds[id]->IsPlaying)
	{
		m_System->playSound(r_sound, nullptr, false, &m_Sounds[id]->channel[new_id]);
		m_Sounds[id]->IsPlaying = true;
		m_Sounds[id]->channel[new_id]->setChannelGroup(m_backgroundSounds);
	}
	else if (!m_Sounds[id]->IsPlaying)
	{
		m_System->playSound(r_sound, nullptr, false, &m_Sounds[id]->channel[new_id]);
		m_Sounds[id]->IsPlaying = true;
		m_Sounds[id]->channel[new_id]->setChannelGroup(m_soundEffects);
	}
}

void AudioManager::PauseSound(const std::string name)
{
	const auto id	  = *this->GetResource(name);
	FMOD_RESULT result = m_Sounds[id]->channel[0]->setPaused(true);

	if (result != FMOD_OK) std::cout << "Failed to pause";
}

bool AudioManager::IsPlaying(const std::string name)
{
	const auto id = *this->GetResource(name);
	return m_Sounds[id]->IsPlaying;
}

void AudioManager::ResumeSound(const std::string name)
{
	const auto id	  = *this->GetResource(name);
	FMOD_RESULT result = m_Sounds[id]->channel[0]->setPaused(false);

	if (result != FMOD_OK) std::cout << "Failed to resume";
}

void AudioManager::StopSound(const std::string name)
{
	const auto id = *this->GetResource(name);
	FMOD_RESULT result = m_Sounds[id]->channel[0]->stop();

	if (result != FMOD_OK) std::cout << "Failed to stop";
}

void AudioManager::ToggleMute()
{
	m_isMute = !m_isMute;
	m_masterChannel->setMute(m_isMute);
}

bool AudioManager::IsMute() { return m_isMute; }

void AudioManager::SetSFXVolume(float volume)
{
	m_soundEffects->setVolume(volume);
	m_SFXVolume = volume;
}

void AudioManager::SetBackgroundVolume(float volume)
{
	m_backgroundSounds->setVolume(volume);
	m_BgVolume = volume;
}
