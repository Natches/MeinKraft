#pragma once
#include <string>
#include <map>
#include <SFML/Audio.hpp>

#define DEFAULT_ID			-1
#define DEFAULT_NAME		std::string("grass1.ogg")
#define SOUND_FOLDER		std::string("Data/Sound/")
#define BLOCK_SOUND_FOLDER	std::string(SOUND_FOLDER + "dig/")
#define MUSIC_SOUND_FOLDER	std::string(SOUND_FOLDER + "music/")
#define SOUND_EFFECT_VOLUME 50.f
#define MUSIC_VOLUME		100.f

#pragma region Sound
struct Sound
{
	Sound();
	Sound(const std::string& soundPath);

	void Init();

	void PlaySound(bool isLoop = false, float volume = 100.f);
	void StopSound();

	std::string		m_soundPath;
	sf::SoundBuffer m_buffer;
	sf::Sound		m_sound;
};
#pragma endregion

#pragma region BlockSoundData
struct BlockSoundData
{
	BlockSoundData();
	BlockSoundData(const std::string& miningSound, const std::string& breakSound, const std::string& placeSound);

	Sound m_mineSound;
	Sound m_breakSound;
	Sound m_placeSound;
};
#pragma endregion

#pragma region SoundController
class SoundController final
{
public:
	SoundController();
	~SoundController();

public:
	void			AddBlockSoundData(const uint16_t ID, BlockSoundData soundData);
	void			Init();

	BlockSoundData&		GetBlockSoundData(uint16_t ID);
	inline sf::Music&	GetBackgroundMusic() { return m_backgroundMusic; }

private:
	std::map<uint16_t, BlockSoundData>	m_blockSounds;
	sf::Music							m_backgroundMusic;
};
#pragma endregion