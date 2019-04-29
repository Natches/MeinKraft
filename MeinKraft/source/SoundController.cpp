#include <string.h>
#include "SoundController.h"
#include "BlockLibrary.h"

#pragma region SoundController
SoundController::SoundController()
{
	BlockSoundData defaultSounds(BLOCK_SOUND_FOLDER + "grass2.ogg", 
								 BLOCK_SOUND_FOLDER + "grass1.ogg", 
								 BLOCK_SOUND_FOLDER + "grass3.ogg");
	m_blockSounds[DEFAULT_ID] = defaultSounds;
}

SoundController::~SoundController()
{

}

void SoundController::Init()
{
	AddBlockSoundData(block::DIRT, BlockSoundData(BLOCK_SOUND_FOLDER + "grass2.ogg", BLOCK_SOUND_FOLDER + "grass1.ogg", BLOCK_SOUND_FOLDER + "grass3.ogg"));
	AddBlockSoundData(block::STONE, BlockSoundData(BLOCK_SOUND_FOLDER + "stone2.ogg", BLOCK_SOUND_FOLDER + "stone1.ogg", BLOCK_SOUND_FOLDER + "stone3.ogg"));
	AddBlockSoundData(block::SNOW, BlockSoundData(BLOCK_SOUND_FOLDER + "snow2.ogg", BLOCK_SOUND_FOLDER + "snow1.ogg", BLOCK_SOUND_FOLDER + "snow3.ogg"));
}

void SoundController::AddBlockSoundData(const uint16_t ID, BlockSoundData& soundData)
{
	m_blockSounds[ID] = soundData;
}

BlockSoundData& SoundController::GetBlockSoundData(uint16_t ID)
{
	return m_blockSounds.find(ID) != m_blockSounds.end() ? m_blockSounds[ID] : m_blockSounds[DEFAULT_ID];
}
#pragma endregion

#pragma region BlockSoundData
BlockSoundData::BlockSoundData()
{
	m_mineSound		= Sound(BLOCK_SOUND_FOLDER + DEFAULT_NAME);
	m_breakSound	= Sound(BLOCK_SOUND_FOLDER + DEFAULT_NAME);
	m_placeSound	= Sound(BLOCK_SOUND_FOLDER + DEFAULT_NAME);
}

BlockSoundData::BlockSoundData(const std::string& miningSound, const std::string& breakSound, const std::string& placeSound)
{
	m_mineSound		= Sound(miningSound);
	m_breakSound	= Sound(breakSound);
	m_placeSound	= Sound(placeSound);
}
#pragma endregion

#pragma region Sound
Sound::Sound()
{
	m_soundPath = BLOCK_SOUND_FOLDER + DEFAULT_NAME;
	Init();
}

Sound::Sound(const std::string& soundPath)
{
	m_soundPath = soundPath;
	Init();
}

void Sound::Init()
{
	m_buffer.loadFromFile(m_soundPath);
}

void Sound::PlaySound(bool isLoop, float volume)
{
	m_sound.setBuffer(m_buffer);
	m_sound.setVolume(volume);

	m_sound.setLoop(isLoop);

	m_sound.play();
}

void Sound::StopSound()
{
	m_sound.stop();
}
#pragma endregion