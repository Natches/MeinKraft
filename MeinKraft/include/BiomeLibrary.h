#pragma once

#include <map>
#include "Biome.h"

namespace biomes
{
	class BiomeLibrary
	{
#pragma region Non-Static Attributes
		std::map<uint8_t, Biome> m_biome;
		std::map<uint8_t, BiomeData> m_biomeData;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
	public:
		BiomeLibrary() = default;
		~BiomeLibrary() = default;

		void InitLibrary(const std::string& path = "Data/Biomes.data");
	private:
		void LoadBiomes(std::stringstream& fileStream);
		void LoadNextBiome(std::stringstream& fileStream, const std::string& biomeRegion);
		void LoadBlockList(std::stringstream& fileStream, BiomeData& data);
		void LoadBlockChance(std::stringstream& fileStream, BiomeData& data);
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
	public:
		Biome GetBiome(const uint8_t id)const;
		const BiomeData& GetBiomeData(const uint8_t id)const;
		Biome GetRandomBiome()const;
#pragma endregion 
	};
}

