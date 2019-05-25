#include "Block.h"

namespace block
{
	float BlockData::GetGenPercent(uint16_t nb)const
	{
		return (m_genPercent.find(nb) == m_genPercent.end() ? 0.f : m_genPercent.at(nb));
	}

	void BlockData::GenerateAllPercentage()
	{
		uint16_t* i = new uint16_t[m_genPercent.size()];
		std::map<uint16_t, float>::iterator it = m_genPercent.begin();
		for (unsigned int idx = 0; idx < m_genPercent.size(); ++idx, ++it)
		{
			i[idx] = (*it).first;
		}
		unsigned int size = m_genPercent.size();
		for (unsigned int idx = 0; idx < size - 1; ++idx)
		{
			vecs::Vec2 i1(i[idx], m_genPercent[i[idx]]);
			vecs::Vec2 i2(i[idx + 1], m_genPercent[i[idx + 1]]);
			vecs::Vec2 Dir(i2 - i1);
			float a = Dir.y / Dir.x;
			float b = i1.y - (a * i1.x);
			for (float idx = i1.x + 1; idx < i2.x; ++idx)
			{
				m_genPercent.insert(std::make_pair(uint16_t(idx), ((a * idx) + b)));
			}
		}
		delete[] i;
	}
}