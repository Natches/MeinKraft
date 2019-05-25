#include <sstream>
#include <fstream>
#include "Log.h"
#include "BiomeLibrary.h"
#include "FileUtils.h"

namespace biomes
{
	void biomes::BiomeLibrary::InitLibrary(const std::string & path)
	{
		std::ifstream file(path);

		if (file.is_open())
		{
			std::stringstream fileStream;
			fileStream << file.rdbuf();
			file.close();
			LoadBiomes(fileStream);
		}
		else
			Log::GetInstance()->WriteMessage("Failed to open block data file at path : " + path, Log::FAULT, Log::BOTH, __FILE__, __LINE__);
	}

	void biomes::BiomeLibrary::LoadBiomes(std::stringstream& fileStream)
	{
		std::string word;
		while (fileStream >> word)
		{
			if (word[0] == '[')
				LoadNextBiome(fileStream, word);
		}
	}

	void biomes::BiomeLibrary::LoadNextBiome(std::stringstream& fileStream, const std::string & biomeRegion)
	{
		BiomeData data;
		std::string word;
		while (fileStream >> word && word != biomeRegion)
		{
			int intToChar = 0;
			if (word == "ID")
			{
				JumpToNext(fileStream);
				fileStream >> intToChar;
				data.m_id = intToChar;
			}
			else if (word == "MaxHeight")
			{
				JumpToNext(fileStream);
				fileStream >> intToChar;
				data.m_maxHeight = intToChar;
			}
			else if (word == "MinHeightX")
			{
				JumpToNext(fileStream);
				fileStream >> intToChar;
				data.m_minHeightX = intToChar;
			}
			else if (word == "MinHeightZ")
			{
				JumpToNext(fileStream);
				fileStream >> intToChar;
				data.m_minHeightZ = intToChar;
			}
			else if (word == "[BlockList]")
			{
				LoadBlockList(fileStream, data);
			}
		}
		m_biome.insert(std::make_pair(data.m_id, Biome(data.m_id)));
		m_biomeData.insert(std::make_pair(data.m_id, data));
	}

	void BiomeLibrary::LoadBlockList(std::stringstream& fileStream, BiomeData& data)
	{
		std::string word;
		while (fileStream >> word && word.find(';') == std::string::npos)
		{
			if (word == "{")
			{
				uint16_t id;
				float chance;
				fileStream >> id;
				JumpToNext(fileStream);
				fileStream >> chance;
				data.m_block[id] = std::make_pair(block::Block(id), chance);
			}
		}
	}

	Biome biomes::BiomeLibrary::GetBiome(const uint8_t id)const
	{
		return (m_biome.find(id) == m_biome.end() ? m_biome.at(0) : m_biome.at(id));
	}

	const BiomeData& biomes::BiomeLibrary::GetBiomeData(const uint8_t id)const
	{
		return (m_biomeData.find(id) == m_biomeData.end() ? m_biomeData.at(0) : m_biomeData.at(id));
	}

	Biome biomes::BiomeLibrary::GetRandomBiome()const
	{
		return GetBiome(uint8_t(rand() % m_biome.size()));
	}
}