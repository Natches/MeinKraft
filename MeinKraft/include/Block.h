#pragma once

#include <map>
#include "GameObject.h"
#include "Vec3.h"

namespace block
{
	class Block
	{
	public:
		enum E_OCCL_FLAGS :unsigned char
		{
			NONE = 0 << 0,
			OCCLR = 1 << 0,
			OCCLL = 1 << 1,
			OCCLU = 1 << 2,
			OCCLD = 1 << 3,
			OCCLF = 1 << 4,
			OCCLB = 1 << 5,
			SKY_OCCLUSION = 1 << 6,
			BREAK = 1 << 7,
			ALL = OCCLR | OCCLL | OCCLU | OCCLD | OCCLF | OCCLB,
			ALL_NO_TOP = OCCLR | OCCLL | OCCLD | OCCLF | OCCLB,
			ALL_NO_DOWN = OCCLR | OCCLL | OCCLU | OCCLF | OCCLB,
			ALL_NO_LEFT = OCCLR | OCCLU | OCCLD | OCCLF | OCCLB,
			ALL_NO_RIGHT = OCCLL | OCCLU | OCCLD | OCCLF | OCCLB,
			ALL_NO_FORWARD = OCCLR | OCCLL | OCCLU | OCCLD | OCCLB,
			ALL_NO_BACK = OCCLR | OCCLL | OCCLU | OCCLD | OCCLF
		};

#pragma region Non-Static Attributes
	private:
		uint16_t        m_id;
		E_OCCL_FLAGS	m_occlFlags;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
	public:
		Block(uint16_t ID = -1):m_id(ID) {};
		virtual ~Block() = default;
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
		inline uint16_t				GetID()				const noexcept { return m_id; }
		inline bool					IsOccludedRight()	const { return (m_occlFlags & OCCLR) == OCCLR; }
		inline bool					IsOccludedLeft()	const { return (m_occlFlags & OCCLL) == OCCLL; }
		inline bool					IsOccludedUp()		const { return (m_occlFlags & OCCLU) == OCCLU; }
		inline bool					IsOccludedDown()	const { return (m_occlFlags & OCCLD) == OCCLF; }
		inline bool					IsOccludedForward()	const { return (m_occlFlags & OCCLF) == OCCLF; }
		inline bool					IsOccludedBack()	const { return (m_occlFlags & OCCLB) == OCCLB; }
		inline bool					IsOccludedAll()		const { return (m_occlFlags & ALL) == ALL; }
		inline bool					IsSkyOccluded()		const { return (m_occlFlags & SKY_OCCLUSION) == SKY_OCCLUSION; }
		inline bool					IsBreaking()		const { return (m_occlFlags & BREAK) == BREAK; }
		inline E_OCCL_FLAGS			GetOCCLState()		const { return m_occlFlags; }
		inline bool					operator==	(const Block& b) { return (m_id == b.GetID() && m_occlFlags == b.GetOCCLState()); }

		inline void					SetOcclState(const E_OCCL_FLAGS flag) { m_occlFlags = flag; }
		inline void					AddOcclState(const E_OCCL_FLAGS flag) { m_occlFlags = E_OCCL_FLAGS(m_occlFlags | flag); }
		inline void					RemoveOcclState(const E_OCCL_FLAGS flag) { m_occlFlags = E_OCCL_FLAGS(m_occlFlags & ~flag); }
#pragma endregion 
	};

	class BlockData
	{
		friend class BlockLibrary;
		enum E_FLAGS : unsigned char
		{
			NONE = 0 << 0,
			BREAKABLE = 1 << 0,
			ILLUMINATE = 1 << 1,
			TRANSPARENT = 1 << 2,
		};
	public:
		enum E_FACE : unsigned char
		{
			TOP,
			DOWN,
			LEFT,
			RIGHT,
			FORWARD,
			BACKWARD
		};

	public:
		inline float GetMaxDurability()				const noexcept { return m_maxDurability; }
		float GetGenPercent( uint16_t nb)			const;
		inline uint16_t GetID()						const noexcept { return m_id; }
		inline uint16_t GetTextureID(E_FACE face)	const noexcept { return m_textureID[face]; }
		inline uint16_t GetTraverSalCost()			const noexcept { return m_traversalCost; }
		inline uint8_t  GetMinHeight()				const noexcept { return m_minHeight; }
		inline uint8_t  GetMaxHeight()				const noexcept { return m_maxHeight; }
		inline uint8_t  GetVeinSizeMin()			const noexcept { return m_veinSizeMin; }
		inline uint8_t  GetVeinSizeMax()			const noexcept { return m_veinSizeMin; }
		inline bool		IsBreakable()				const noexcept { return (m_flags & E_FLAGS::BREAKABLE) == E_FLAGS::BREAKABLE; }
		inline bool		IsIlluminating()			const noexcept { return (m_flags & E_FLAGS::ILLUMINATE) == E_FLAGS::ILLUMINATE; }
		inline bool		IsTransparent()				const noexcept { return (m_flags & E_FLAGS::TRANSPARENT) == E_FLAGS::TRANSPARENT; }
	private:
		void GenerateAllPercentage();
	private:
		float						m_maxDurability;
		std::map<uint16_t, float>	m_genPercent;
		uint16_t					m_id;
		uint16_t*					m_textureID;
		uint16_t					m_traversalCost;
		uint8_t						m_minHeight;
		uint8_t						m_maxHeight;
		uint8_t						m_veinSizeMin;
		uint8_t						m_veinSizeMax;
		E_FLAGS						m_flags;
	};
}
