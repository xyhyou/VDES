#include "AISBitsManager.h"
#include "UtilityInterface.h"

#include <map>
#include <unordered_map>
#include <iomanip>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace VDES
{
	static std::vector<char> sixBitCodeToStandardASCIITable = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':',
		';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E',
		'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', '`', 'a', 'b', 'c',
		'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w'
	};

	static std::map<char, uint32_t> sixBitASCIIMap = {
		{'@', 0},  {'A', 1},  {'B', 2},  {'C', 3},  {'D', 4},
		{'E', 5},  {'F', 6},  {'G', 7},  {'H', 8},  {'I', 9},
		{'J', 10}, {'K', 11}, {'L', 12}, {'M', 13}, {'N', 14},
		{'O', 15}, {'P', 16}, {'Q', 17}, {'R', 18}, {'S', 19},
		{'T', 20}, {'U', 21}, {'V', 22}, {'W', 23}, {'X', 24},
		{'Y', 25}, {'Z', 26}, {'[', 27}, {'\\', 28},{']', 29},
		{'^', 30}, {'_', 31}, {' ', 32}, {'!', 33}, {'\"', 34},
		{'#', 35}, {'$', 36}, {'%', 37}, {'&', 38}, {'\'', 39},
		{'(', 40}, {')', 41}, {'*', 42}, {'+', 43}, {',', 44},
		{'-', 45}, {'.', 46}, {'/', 47}, {'0', 48}, {'1', 49},
		{'2', 50}, {'3', 51}, {'4', 52}, {'5', 53}, {'6', 54},
		{'7', 55}, {'8', 56}, {'9', 57}, {':', 58}, {';', 59},
		{'<', 60}, {'=', 61}, {'>', 62}, {'?', 63}
	};


	/**
	* @brief  GB2312 区位码编码范围：A1A1－FEFE，其中汉字编码范围：B0A1-F7FE。
	*/
	static uint32_t GetZoneBitCode(const uint8_t characterHigh, const uint8_t characterLow)
	{
		uint32_t a1 = characterHigh - 0xA0;
		uint32_t a2 = characterLow - 0xA0;

		std::ostringstream ostr;
		ostr << a1 << std::setw(2) << std::setfill('0') << a2;
		return atoi(ostr.str().c_str());
	}


	/**
	* @brief	   区位码转换为13 bit编码
	* @param[in]   zoneBitCode - 区位码
	* @param[out]  none
	* @return      13bit编码
	*/
	static uint32_t ConvertZoneBitCodeTo13BitsCode(const uint32_t zoneBitCode)
	{
		std::ostringstream ostr;

		auto zoneBitCodeTemp = zoneBitCode;
		ostr << zoneBitCodeTemp;
		auto str = ostr.str();
		auto subStr = str.substr(0, 2);
		int  areaCode = atoi(subStr.c_str());

		if (zoneBitCode > 1600)
		{
			zoneBitCodeTemp = zoneBitCode - 1600 - (areaCode - 16) * 6;
		}
		else
		{
			zoneBitCodeTemp = zoneBitCode + 3700;
		}

		auto value = ((zoneBitCodeTemp & 0xFFF) | 0x1000);
		return value;
	}

	/**
	* @brief	   13位中文编码转换为区域码
	* @param[in]   code - 13位的中文编码值
	* @param[out]  none
	* @return      区域码
	*/
	static uint32_t Convert13BitsCodeToZoneBitCode(uint32_t code)
	{
		uint32_t zoneBitCode;

		code = code & 0x0FFF;
		if (code > 3800)
		{
			zoneBitCode = code - 3700;
		}
		else
		{
			if (code % 94 == 0)
			{
				zoneBitCode = code + 1600 + (code / 94 - 1) * 6;
			}
			else
			{
				zoneBitCode = code + 1600 + (code / 94) * 6;
			}
		}
		return zoneBitCode;
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	class MapValueFinder
	{
	public:

		MapValueFinder(const uint32_t value) : m_value(value) {}

		bool operator()(const std::map<char, uint32_t>::value_type &pair)
		{
			return pair.second == m_value;
		}
	private:
		uint32_t m_value;
	};

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	AISBitsManager::AISBitsManager()
	{
	}

	AISBitsManager::~AISBitsManager()
	{

	}

	bool AISBitsManager::Encode(const std::string &text, const int DAC = 1, const int FI = 0)
	{
		if ((DAC == 1 && FI == 0) || (DAC == 412 && FI == 45))
		{
			// International function message 0: Text using 6-bit ASCII
			for (auto &character : text)
			{
				auto iter = sixBitASCIIMap.find(character);
				if (iter != sixBitASCIIMap.end())
				{
					BitsManager::Encode(iter->second, 6);
				}
			}
			return true;
		}
		else if (DAC == 413 && FI == 1) // 14 bits code with chinese
		{
			for (auto index = 0U; index < text.length(); index++)
			{
				auto character = static_cast<uint8_t>(text.at(index));
				if (character >= 0x80) // chinese code
				{
					auto characterNext = static_cast<uint8_t>(text.at(index + 1));

					character &= 0x7F;
					characterNext &= 0x7F;

					uint8_t a, b;

					if (character >= 0x40)
					{
						a = character - 0x40;
						b = characterNext;
					}
					else
					{
						a = (character - 0x30) * 4 + characterNext / 0x20;
						b = characterNext % 0x20;
					}
					a |= 0x40;
					BitsManager::Encode(a, 7);
					BitsManager::Encode(b, 7);
					index++;
				}
				else
				{
					auto iter = sixBitASCIIMap.find(character);
					if (iter != sixBitASCIIMap.end())
					{
						BitsManager::Encode(iter->second, 7);
					}
				}
			}
			return true;
		}
		else if (DAC == 413 && FI == 2) // 13 bits code with chinese
		{
			for (auto index = 0U; index < text.length(); index++)
			{
				auto character = static_cast<uint8_t>(text.at(index));
				if (character >= 0x80) // chinese code
				{
					auto characterNext = static_cast<uint8_t>(text.at(index + 1));

					auto code = GetZoneBitCode(character, characterNext);

					auto value = ConvertZoneBitCodeTo13BitsCode(code);
					BitsManager::Encode(value, 13);
					index++;
				}
				else
				{
					auto iter = sixBitASCIIMap.find(character);
					if (iter != sixBitASCIIMap.end())
					{
						BitsManager::Encode(iter->second, 7);
					}
				}
			}
			return true;
		}
		return false;
	}

	uint32_t AISBitsManager::GetFillBitsNumberToEncode(void) const
	{
		auto surplusBitNum = GetBitsNumberToDecode() % 6;

		if (surplusBitNum != 0)
		{
			return 6 - surplusBitNum;
		}
		return 0;
	}

	std::string AISBitsManager::GetEncodedVDMPayload(void) const
	{
		std::string payload;

		auto fillNum = GetFillBitsNumberToEncode();
		auto bits = GetBitsEncoded();
		for (auto i = 0U; i < fillNum; i++)
		{
			bits.push_back(0);
		}

		auto bitsNum = bits.size();
		int  blockIndex = 0;
		while (bitsNum > 0)
		{
			std::string bitString;
			for (auto i = 0; i < 6; i++)
			{
				bitString += bits.at(blockIndex + i) + 0x30;
				bitsNum--;
			}
			blockIndex += 6;

			std::bitset<6> sixBitCode(bitString);
			auto code = sixBitCode.to_ulong();
			payload += sixBitCodeToStandardASCIITable.at(code);
		}
		return payload;
	}

	std::vector<std::string> AISBitsManager::BuildPacket(const bool isVDM /*= true*/, const char channel /*= 'A'*/)
	{
		static int sequentialID = 0;

		std::vector<std::string> packets;

		auto payload = GetEncodedVDMPayload();
		auto totalNum = 0;

		if (payload.length() <= 61)
		{
			totalNum = 1;
		}
		else
		{
			totalNum = (int)(payload.length() / 60 + ((payload.length() % 60 == 0) ? 0 : 1));
		}

		if (totalNum > 0)
		{
			std::ostringstream ostr;

			auto fillNums = GetFillBitsNumberToEncode();

			if (totalNum == 1)
			{
				ostr << "!AI" << (isVDM ? "VDM" : "VDO") << ",1,1,," << channel << ","
					<< payload << "," << fillNums;

				auto checksum = UtilityInterface::GetXOR(ostr.str());
				ostr << "*" << std::hex << std::setw(2) << std::uppercase << std::setfill('0')
					<< static_cast<int>(checksum) << "\r\n";
				packets.push_back(ostr.str());
			}
			else
			{
				for (auto i = 0; i < totalNum; i++)
				{
					ostr.str("");
					ostr.clear();
					ostr << "!AI" << (isVDM ? "VDM" : "VDO") << "," << totalNum << "," << i + 1 << ","
						<< sequentialID << "," << channel << "," << payload.substr(60 * i, 60) << ","
						<< ((i == totalNum - 1) ? fillNums : 0);

					auto checksum = UtilityInterface::GetXOR(ostr.str());
					ostr << "*" << std::hex << std::setw(2) << std::uppercase << std::setfill('0')
						<< static_cast<int>(checksum) << "\r\n";
					packets.push_back(ostr.str());
				}
				if (++sequentialID > 9)
				{
					sequentialID = 0;
				}
			}
		}
		return packets;
	}

	bool AISBitsManager::SetVDMPayloadToDecode(const std::string &payload, const uint32_t fillBitsNum)
	{
		ResetEncoder();

		for (auto &character : payload)
		{
			auto iter = std::find(sixBitCodeToStandardASCIITable.begin(),
				sixBitCodeToStandardASCIITable.end(), character);
			if (iter == sixBitCodeToStandardASCIITable.end())
			{
				return false;
			}
			auto sixCodeValue = static_cast<uint32_t>(iter - sixBitCodeToStandardASCIITable.begin());
			BitsManager::Encode(sixCodeValue, 6);
		}

		RemoveTailBits(fillBitsNum);

		return true;
	}

	std::string AISBitsManager::DecodeToString(const uint32_t startBitPos, const uint32_t bitNum,
		const int DAC, const int FI) const
	{
		auto &bits = GetBitsEncoded();

		auto func = [&](uint32_t &indexStart, const uint32_t bitsNum) -> uint8_t {
			std::string bitString;
			for (auto i = 0U; i < bitsNum; i++)
			{
				bitString += bits.at(indexStart + i) + 0x30;
			}
			indexStart += bitsNum;
			std::bitset<10> bitsTemp(bitString);
			return static_cast<uint8_t>(bitsTemp.to_ulong());
			};

		if (startBitPos + bitNum <= bits.size())
		{
			uint32_t index = startBitPos;

			if ((DAC == 1 && FI == 0) || (DAC == 412 && FI == 45))
			{
				/**
				* FIXME: the bit number should be the multiple of 6
				* if the param bitNum is not, we may take the other
				* full six bit character
				*/
				int  bitNumTemp = bitNum;
				auto surplusBitsNum = bitNum % 6;

				if (surplusBitsNum != 0)
				{
					bitNumTemp -= surplusBitsNum;
				}

				std::string text;

				while (bitNumTemp >= 6)
				{
					auto sixCodeValue = func(index, 6);
					auto iter = std::find_if(sixBitASCIIMap.begin(), sixBitASCIIMap.end(),
						MapValueFinder(sixCodeValue));
					if (iter != sixBitASCIIMap.end())
					{
						text += iter->first;
					}
					bitNumTemp -= 6;
				}
				return text;
			}
			else if ((DAC == 413 && FI == 1) || (DAC == 413 && FI == 3)) 
			{
				// 14 bits code with chinese
				uint32_t index = startBitPos;
				int		 bitNumTemp = bitNum;
				auto	 surplusBitsNum = bitNum % 7;

				if (surplusBitsNum != 0)
				{
					bitNumTemp -= surplusBitsNum;
				}

				std::string text;

				while (bitNumTemp >= 7)
				{
					auto value = func(index, 7);
					bitNumTemp -= 7;

					if (value >= 0x40)
					{
						if (bitNumTemp >= 7)
						{
							uint8_t a = value & 0x3F;
							// Obtain the second chinese character
							uint8_t b = func(index, 7);
							bitNumTemp -= 7;

							uint8_t A, B;

							if (b >= 0x20)
							{
								A = a + 0x40;
								B = b;
							}
							else
							{
								A = 0x30 + a / 4;
								B = b + (a & 3) * 0x20;
							}

							A |= 0x80;
							B |= 0x80;
							text += A;
							text += B;
						}
					}
					else
					{
						// six bits ASCII code
						auto iter = std::find_if(sixBitASCIIMap.begin(), sixBitASCIIMap.end(),
							MapValueFinder(value));
						if (iter != sixBitASCIIMap.end())
						{
							text += iter->first;
						}
					}
				}
				return text;
			}
			else if ((DAC == 413 && FI == 2) || (DAC == 413 && FI == 4)) // 13 bits code with chinese
			{
				uint32_t    index = startBitPos;
				int			bitNumTemp = bitNum;
				bool        valid = false;
				std::string text;
				static std::unordered_map<uint32_t, uint32_t> validBitNums;

				/*
				* The bitNum of 13 bit's code should be: bitNum = 13a+7b
				*/
				if (validBitNums.size() == 0)
				{
					/*
					* Calculate the valid bit num value.
					* The maximum of the application data of the message 8
					* should be the 952 bits
					*/
					for (auto i = 0U; i <= 952 / 13; i++)
					{
						for (auto j = 0U; j <= 952 / 7; j++)
						{
							auto sum = 13 * i + 7 * j;
							if (sum <= 952)
							{
								if (validBitNums.find(sum) == validBitNums.end())
								{
									validBitNums[sum] = sum;
								}
							}
							else
							{
								break;
							}
						}
					}
				}

				if (validBitNums.find(bitNum) != validBitNums.end())
				{
					while (bitNumTemp >= 7)
					{
						auto value = DecodeToNumerical(index, 7);
						if (value < 0x40)
						{
							// six bits ASCII code
							auto iter = std::find_if(sixBitASCIIMap.begin(), sixBitASCIIMap.end(),
								MapValueFinder((uint32_t)value));
							if (iter != sixBitASCIIMap.end())
							{
								text += iter->first;
							}
							bitNumTemp -= 7;
							index += 7;
						}
						else
						{
							// 13 bits chinese code
							auto value = (uint32_t)DecodeToNumerical(index, 13);
							auto zoneBitCode = Convert13BitsCodeToZoneBitCode(value);
							std::ostringstream ostr;

							ostr << zoneBitCode;
							auto    strTemp = ostr.str();
							uint8_t characterHigh, characterLow;

							if (strTemp.length() < 4)
							{
								characterHigh = atoi(strTemp.substr(0, 1).c_str());
								characterLow = atoi(strTemp.substr(1, 2).c_str());
							}
							else
							{
								characterHigh = atoi(strTemp.substr(0, 2).c_str());
								characterLow = atoi(strTemp.substr(2, 2).c_str());
							}

							characterHigh += 0xA0;
							characterLow += 0xA0;

							/*
							* Need to check whether the code is standard 13 bit code.
							*/
							if ((characterHigh >= 0xA1 && characterHigh <= 0xA3) ||
								(characterHigh >= 0xB0 && characterHigh <= 0xD7))
							{
								text += characterHigh;
								text += characterLow;

								bitNumTemp -= 13;
								index += 13;
							}
							else
							{
								// It's the custom definition
								text.clear();
								break;
							}
						}
					}
					if (bitNumTemp != 0 && bitNum == 160)
					{
						// Previous custom definition.
						text.clear();
						bitNumTemp = bitNum;
						index = startBitPos;
						while (bitNumTemp > 0)
						{
							auto value = (uint8_t)DecodeToNumerical(index, 8);
							text += value;
							bitNumTemp -= 8;
							index += 8;
						}
					}
					return text;
				}
			}
		}
		return "";
	}
}

