#ifndef UTILITY_INTERFACE_H_
#define UTILITY_INTERFACE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace VDES
{
	class UtilityInterface
	{
    public:
        /**
        * @brief      Get the unix timestamp now
        * @param[in]  none
        * @param[out] none
        * @return     seconds elapsed since 1970/01/01 00:00:00
        */
        static uint64_t GetCurrentTimeStamp(void);

        /**
        * @brief      Get TimeZone
        * @param[in]  none
        * @param[out] none
        * @return     seconds of TimeZone
        * @note
        */
        static time_t GetTimeZone(void);

        /**
        * @brief      Convert the struct tm dateUTC to the unix timestamp
        * @param[in]  dateUTC - UTC date info (time zone = 0)
        * @param[out] none
        * @return     seconds elapsed since 1970/01/01 00:00:00
        * @note
        */
        static time_t MakeGmtime(struct tm *dateUTC);

        static uint8_t GetXOR(const std::string &sentence);

        static uint8_t CalculateChecksum(const std::string &sentence);

        static void AddChecksum(std::string &sentence);

        static bool IsChecksumValid(const std::string &sentence);

        static std::vector<std::string> SplitString(const std::string &src, const std::string &sep = ",");

        static std::vector<std::string> SplitString(const std::string &src, const std::vector<std::string> &seps);

        static std::string CalculateMD5Digest(const std::string &sentence);

        static std::string CalculateMD5Digest(const uint8_t *data, uint32_t size);

        static std::string UTF8ToGBK(const std::string &strUTF8);

        static std::string GBKToUTF8(const std::string &strGBK);

        static void RemoveTailCharacter(std::string &str, char character);

        static bool IsASCIICode(const std::string &characters);

        /*
        * @brief      Delay for a specified number of milliseconds
        * @param[in]  milliSeconds - milliseconds to sleep
        * @param[out] none
        * @return     none
        * @note       
        */
        static void Sleep(const uint32_t milliSeconds);

        static int32_t ConvertComplementCodeToInteger(const uint32_t value, const uint8_t bitsNum);

        static uint32_t ConvertIntegerToComplementCode(const int32_t value, const uint8_t bitsNum);

        template<typename T, typename... Args>
        static std::unique_ptr<T> MakeUnique(Args&&... args)
        {
            return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
        }
    };
}
#endif