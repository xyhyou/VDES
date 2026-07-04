#include "BitsManager.h"

#include <bitset>

namespace VDES
{
	class BitsManager::Impl
	{
	public:
		Impl();

		~Impl();
	public:
		std::vector<uint32_t> m_bits;
	};

	BitsManager::Impl::Impl()
	{
	}

	BitsManager::Impl::~Impl()
	{

	}

	/************************************************************************
	 *                        Public Interfaces
	 ************************************************************************/

	BitsManager::BitsManager()
	{
		m_impl = std::unique_ptr<Impl>(new Impl());
	}

	BitsManager::~BitsManager()
	{

	}

	bool BitsManager::Encode(const uint32_t value, const uint32_t bitsNum)
	{
		if (m_impl)
		{
			if (bitsNum <= 1024)
			{
				std::bitset<1024> bits(value);

				for (auto i = 0U; i < bitsNum; i++)
				{
					auto value = (bits[bitsNum - 1 - i] == true ? 1 : 0);
					m_impl->m_bits.push_back(value);
				}
				return true;
			}
		}
		return false;
	}

	bool BitsManager::Encode(const uint32_t value, const uint32_t startBitPos, const uint32_t bitsNum)
	{
		if (m_impl)
		{
			if (bitsNum <= 1024)
			{
				std::bitset<1024> bits(value);
				std::vector<int>  values;

				for (auto i = 0U; i < bitsNum; i++)
				{
					auto value = (bits[bitsNum - 1 - i] == true ? 1 : 0);
					values.push_back(value);
				}
				auto iter = m_impl->m_bits.begin();
				m_impl->m_bits.insert(iter + startBitPos, values.begin(), values.end());
				return true;
			}
		}
		return false;
	}

	void BitsManager::ResetEncoder(void)
	{
		if (m_impl)
		{
			m_impl->m_bits.clear();
		}
	}

	void BitsManager::RemoveTailBits(const uint32_t bitsNum)
	{
		if (m_impl)
		{
			for (auto i = 0U; i < bitsNum; i++)
			{
				m_impl->m_bits.pop_back();
			}
		}
	}

	void BitsManager::RemoveBits(const uint32_t startBitPos, const uint32_t bitNum)
	{
		if (m_impl && bitNum > 0)
		{
			auto iterFirst = m_impl->m_bits.begin() + startBitPos;
			auto iterLast = iterFirst + bitNum;
			m_impl->m_bits.erase(iterFirst, iterLast);
		}
	}

	uint32_t BitsManager::GetBitsNumberToDecode(void) const
	{
		return static_cast<uint32_t>(m_impl->m_bits.size());
	}

	int64_t BitsManager::DecodeToNumerical(const uint32_t startBitPos, const uint32_t bitNum) const
	{
		if (m_impl)
		{
			if (startBitPos + bitNum <= m_impl->m_bits.size())
			{
				std::string bitString;
				for (auto index = 0U; index < bitNum; index++)
				{
					bitString += m_impl->m_bits.at(startBitPos + index) + 0x30;
				}

				std::bitset<1024> bits(bitString);
				return (uint32_t)bits.to_ullong();
			}
		}
		return -1;
	}

	std::vector<uint32_t> &BitsManager::GetBitsEncoded(void) const
	{
		return m_impl->m_bits;
	}

	std::vector<uint8_t> BitsManager::GetBytesEncoded(const uint32_t startBitPos, const int endBitPos) const
	{
		std::vector<uint8_t> bytes;

		if (m_impl)
		{
			int endBitPosTemp = (endBitPos > 0) ? endBitPos : (int)(m_impl->m_bits.size() - 1);

			auto bitsNumTotal = endBitPosTemp - startBitPos + 1;
			auto count = bitsNumTotal / 8;
			for (auto i = 0U; i < count; i++)
			{
				auto value = static_cast<uint8_t>(DecodeToNumerical(startBitPos + i * 8, 8));
				bytes.push_back(value);
			}
			auto bitsNumSurplus = bitsNumTotal % 8;
			if (bitsNumSurplus > 0)
			{
				auto index = endBitPosTemp - bitsNumSurplus + 1;
				auto value = static_cast<uint8_t>(DecodeToNumerical(index, bitsNumSurplus));
				bytes.push_back(value);
			}
		}
		return bytes;
	}
}
