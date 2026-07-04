#ifndef AIS_BITS_MANAGER_H_
#define AIS_BITS_MANAGER_H_

#include "BitsManager.h"

namespace VDES
{
	class AISBitsManager : public BitsManager
	{
	public:
		using BitsManager::Encode;

		AISBitsManager();

		virtual ~AISBitsManager();

		bool Encode(const std::string &text, const int DAC, const int FI);

		uint32_t GetFillBitsNumberToEncode(void) const;

		std::string GetEncodedVDMPayload(void) const;

		std::vector<std::string> BuildPacket(const bool isVDM = true, const char channel = 'A');

		bool SetVDMPayloadToDecode(const std::string &payload, const uint32_t fillBitsNum);

		std::string DecodeToString(const uint32_t startBitPos, const uint32_t bitNum,
			const int DAC = 1, const int FI = 0) const;
	};
}

#endif
