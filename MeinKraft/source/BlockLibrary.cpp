#include <sstream>
#include <fstream>
#include "Log.h"
#include "BlockLibrary.h"
#include "PoolThread.h"
#include "TaskTemplate.h"
#include "FileUtils.h"

namespace block
{
	BlockLibrary::~BlockLibrary()
	{
		for (auto data : m_blockData)
		{
			delete[] data.second.m_textureID;
		}
	}

	void BlockLibrary::InitLibrary(const std::string& path)
	{
		std::ifstream file(path);

		if (file.is_open())
		{
			std::stringstream fileStream;
			fileStream << file.rdbuf();
			file.close();
			LoadBlocks(fileStream);
			ThreadLib::PoolThread::getInstance().FinishAllTaskThenContinue();
		}
		else
			Log::GetInstance()->WriteMessage("Failed to open block data file at path : " + path, Log::FAULT, Log::BOTH, __FILE__, __LINE__);
	}

	void BlockLibrary::LoadBlocks(std::stringstream& fileStream)
	{
		std::string word;
		while (fileStream >> word)
		{
			if (word[0] == '[')
				LoadNextBlock(fileStream, word);
		}
	}

	void BlockLibrary::LoadNextBlock(std::stringstream& fileStream, const std::string& blockRegion)
	{
		BlockData data;
		data.m_textureID = new uint16_t[6];
		data.m_flags = BlockData::E_FLAGS(BlockData::NONE);
		std::string word;
		while (fileStream >> word && word != blockRegion)
		{
			if (word == "Max_Durability")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_maxDurability;
			}
			else if (word == "Travel_Cost")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_traversalCost;
			}
			else if (word == "Flags")
			{
				JumpToNext(fileStream);
				getline(fileStream, word);
				LoadFlags(word, data);
			}
			else if (word == "ID")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_id;
			}
			else if (word == "MinHeight")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_minHeight;
			}
			else if (word == "MaxHeight")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_maxHeight;
			}
			else if (word == "VeinSizeMax")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_veinSizeMax;
			}
			else if (word == "VeinSizeMin")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_veinSizeMax;
			}
			else if (word == "[Percent]")
			{
				LoadPercents(data, fileStream);
			}
			else if (word == "TextureIDTop")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_textureID[0];
			}
			else if (word == "TextureIDDown")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_textureID[1];
			}
			else if (word == "TextureIDLeft")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_textureID[2];
			}
			else if (word == "TextureIDRight")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_textureID[3];
			}
			else if (word == "TextureIDForward")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_textureID[4];
			}
			else if (word == "TextureIDBackWard")
			{
				JumpToNext(fileStream);
				fileStream >> data.m_textureID[5];
			}
		}
		m_block.insert(std::make_pair(data.m_id, Block(data.m_id)));
		m_blockData.insert(std::make_pair(data.m_id, data));
		ThreadLib::PoolThread::getInstance().AddTask(new TaskThreadLib::TaskTemplated::TaskTemplate<>([&, data] { m_blockData[data.m_id].GenerateAllPercentage(); }, false));
	}

	void BlockLibrary::LoadFlags(const std::string& line, BlockData& data)
	{
		if (line.find("BREAKABLE") != std::string::npos)
		{
			data.m_flags = (BlockData::E_FLAGS)(data.m_flags | BlockData::BREAKABLE);
		}
		if (line.find("ILLUMINATE") != std::string::npos)
		{
			data.m_flags = (BlockData::E_FLAGS)(data.m_flags | BlockData::ILLUMINATE);
		}
		if (line.find("TRANSPARENT") != std::string::npos)
		{
			data.m_flags = (BlockData::E_FLAGS)(data.m_flags | BlockData::TRANSPARENT);
		}
	}

	void BlockLibrary::LoadPercents(BlockData& data, std::stringstream& fileStream)
	{
		std::string word;
		while (fileStream >> word && word.find(';') == std::string::npos)
		{
			if (word == "{")
			{
				uint16_t y;
				float percent;
				fileStream >> y;
				JumpToNext(fileStream);
				fileStream >> percent;
				data.m_genPercent.insert(std::make_pair(y, percent));
			}
		}
	}
	Block BlockLibrary::GetBlock(const uint16_t id)const
	{
		return (m_block.find(id) == m_block.end() ? m_block.at(-1) : m_block.at(id));
	}
	const BlockData& BlockLibrary::GetBlockData(const uint16_t id)const
	{
		return (m_blockData.find(id) == m_blockData.end() ? m_blockData.at(-1) : m_blockData.at(id));
	}
}
