#include "ASMManager.h"

#include <map>
#include <cstdlib>

#include "ASMDTD.h"
#include "Coordinate.h"
#include "VDES.h"
#include "AISBitsManager.h"
#include "VDESConfigure.h"
#include "UtilityInterface.h"
#include "spdlog.h"

namespace VDES
{
    class ASMManager::Impl
    {
    public:
        Impl(ASMManager *parent = nullptr);

        ~Impl();

        uint64_t DecodeTime(const AISBitsManager &manager, const uint32_t startBitPos,
                            const uint32_t bitsNum);

        void HandlePayload(const std::string &payload, const uint8_t fillBitNum);

        void ParseADM(const std::string &sentence);

        void ParseASM(const std::string &sentence);

        void ParseASMDAC412FI26(const AISBitsManager &manager);

        void ParseASMDAC412FI27(const AISBitsManager &manager);

        void ParseASMDAC412FI28(const AISBitsManager &manager);

        void ParseASMDAC412FI29(const AISBitsManager &manager);

        void ParseASMDAC412FI30(const AISBitsManager &manager);

        void ParseASMDAC412FI32(const AISBitsManager &manager);

        void ParseASMDAC412FI35(const AISBitsManager &manager);

        void ParseASMDAC412FI36(const AISBitsManager &manager);

        void ParseASMDAC412FI31(const AISBitsManager &manager);

        void ParseASMDAC412FI38(const AISBitsManager &manager);

        void ParseASMDAC412FI39(const AISBitsManager &manager);

        void ParseASMDAC412FI40(const AISBitsManager &manager);

        void ParseASMDAC412FI33(const AISBitsManager &manager);

        void ParseASMDAC412FI34(const AISBitsManager &manager);

        void ParseASMDAC412FI41(const AISBitsManager &manager);

        void ParseASMDAC412FI45(const AISBitsManager &manager);

        void ParseASMDAC412FI37(const AISBitsManager &manager);

        void ParseASMDAC412FI42(const AISBitsManager &manager);

        void ParseASMDAC412FI43(const AISBitsManager &manager);

        void ParseASMDAC412FI44(const AISBitsManager &manager);

        void ParseChannelBoundaryHelper(const AISBitsManager &manager, uint8_t edgeType, uint8_t fi);

        void ParseASMDAC413FI3(const AISBitsManager &manager);

        void ParseASMDAC413FI4(const AISBitsManager &manager);

        void ParseASMDAC413FI5(const AISBitsManager &manager);

        void ParseASMDAC413FI7(const AISBitsManager &manager);

        void ParseASMDAC413FI8(const AISBitsManager &manager);

        void ParseASMDAC413FI9(const AISBitsManager &manager);

        void ParseASMDAC413FI10(const AISBitsManager &manager);

        void ParseASMDAC412FI47(const AISBitsManager &manager);

        void ParseASMDAC412FI48(const AISBitsManager &manager);

        void ParseASMDAC412FI50(const AISBitsManager &manager);

        void ParseASMDAC412FI51(const AISBitsManager &manager);

        void ParseASMDAC412FI52(const AISBitsManager &manager);

        void ParseASMDAC412FI55(const AISBitsManager &manager);

        double DecodeCoordinate(const AISBitsManager &manager, const uint32_t startBitPos, const uint32_t bitsNum);

    public:
        using ParseFunction   = std::function<void(const AISBitsManager &manager)>;
        using ASMParserMap    = std::map<uint32_t, ParseFunction>;
        using MessageParseMap = std::map<uint8_t, ParseFunction>;

        ASMManager      *m_parent;
        std::string     m_payloadASM;
        int             m_sequentialID;
        uint64_t        m_timestampReceived;
        MessageParseMap m_messageParserMap;
        ASMParserMap    m_asmParserMap;
        uint32_t        m_mmsiSource;
        uint32_t        m_mmsiDestination;
    };

    ASMManager::Impl::Impl(ASMManager *parent)
    {
        m_timestampReceived  = 0;
        m_sequentialID       = -1;
        m_parent             = parent;
        m_mmsiSource         = 0;
        m_mmsiDestination    = 0;

        //m_messageParserMap.insert(std::make_pair(2, std::bind(&Impl::ParseMessage2, this, std::placeholders::_1)));

        /************************ Application Specific Message **************************/


        m_asmParserMap.insert(std::make_pair(41226, std::bind(&Impl::ParseASMDAC412FI26, this, std::placeholders::_1)));
        
        m_asmParserMap.insert(std::make_pair(41227, std::bind(&Impl::ParseASMDAC412FI27, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41228, std::bind(&Impl::ParseASMDAC412FI28, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41229, std::bind(&Impl::ParseASMDAC412FI29, this, std::placeholders::_1)));
        
        m_asmParserMap.insert(std::make_pair(41230, std::bind(&Impl::ParseASMDAC412FI30, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41231, std::bind(&Impl::ParseASMDAC412FI31, this, std::placeholders::_1)));

		m_asmParserMap.insert(std::make_pair(41232, std::bind(&Impl::ParseASMDAC412FI32, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41233, std::bind(&Impl::ParseASMDAC412FI33, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41234, std::bind(&Impl::ParseASMDAC412FI34, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41235, std::bind(&Impl::ParseASMDAC412FI35, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41236, std::bind(&Impl::ParseASMDAC412FI36, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41237, std::bind(&Impl::ParseASMDAC412FI37, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41238, std::bind(&Impl::ParseASMDAC412FI38, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41239, std::bind(&Impl::ParseASMDAC412FI39, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41240, std::bind(&Impl::ParseASMDAC412FI40, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41241, std::bind(&Impl::ParseASMDAC412FI41, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41242, std::bind(&Impl::ParseASMDAC412FI42, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41243, std::bind(&Impl::ParseASMDAC412FI43, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41244, std::bind(&Impl::ParseASMDAC412FI44, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41245, std::bind(&Impl::ParseASMDAC412FI45, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41247, std::bind(&Impl::ParseASMDAC412FI47, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41248, std::bind(&Impl::ParseASMDAC412FI48, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41250, std::bind(&Impl::ParseASMDAC412FI50, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41251, std::bind(&Impl::ParseASMDAC412FI51, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41252, std::bind(&Impl::ParseASMDAC412FI52, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41255, std::bind(&Impl::ParseASMDAC412FI55, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41303, std::bind(&Impl::ParseASMDAC413FI3, this, std::placeholders::_1)));
        
        m_asmParserMap.insert(std::make_pair(41304, std::bind(&Impl::ParseASMDAC413FI4, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41305, std::bind(&Impl::ParseASMDAC413FI5, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41307, std::bind(&Impl::ParseASMDAC413FI7, this, std::placeholders::_1)));

        m_asmParserMap.insert(std::make_pair(41308, std::bind(&Impl::ParseASMDAC413FI8, this, std::placeholders::_1)));  

        m_asmParserMap.insert(std::make_pair(41309, std::bind(&Impl::ParseASMDAC413FI9, this, std::placeholders::_1)));  

        m_asmParserMap.insert(std::make_pair(41310, std::bind(&Impl::ParseASMDAC413FI10, this, std::placeholders::_1)));  
    }

    ASMManager::Impl::~Impl()
    {

    }

    uint64_t ASMManager::Impl::DecodeTime(const AISBitsManager &manager, const uint32_t startBitPos,
                                          const uint32_t bitsNum)
    {
        if (bitsNum == 20 || bitsNum == 16 || bitsNum == 22)
        {
            uint8_t  month = 0, day = 0, hour = 0, minute = 0, second = 0;
            uint32_t pos = startBitPos;

            if (bitsNum == 20)
            {
                month = static_cast<uint8_t>(manager.DecodeToNumerical(startBitPos, 4));
                pos += 4;
            }

            if (bitsNum == 22)
            {
                day = static_cast<uint8_t>(manager.DecodeToNumerical(pos, 5));
                hour = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 5, 5));
                minute = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 10, 6));
                second = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 16, 6));
            }
            else
            {
                day = static_cast<uint8_t>(manager.DecodeToNumerical(pos, 5));
                hour = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 5, 5));
                minute = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 10, 6));
            }
            
            auto timestamp = UtilityInterface::GetCurrentTimeStamp();
            tm   timeUTC = { 0 };
            timeUTC = *gmtime((time_t *)&timestamp);

            if (month != 0 || day != 0)
            {
                if (month >= 1 && month <= 12)
                {
                    timeUTC.tm_mon = month - 1;
                }
                if (day >= 1 && day <= 31)
                {
                    timeUTC.tm_mday = day;
                }
                if (hour >= 0 && hour <= 23)
                {
                    timeUTC.tm_hour = hour;
                }
                if (minute >= 0 && minute <= 59)
                {
                    timeUTC.tm_min = minute;
                }
                if (second >= 0 && second <= 59)
                {
                    timeUTC.tm_sec = second;
                }
                auto timestamp = UtilityInterface::MakeGmtime(&timeUTC);
                return timestamp;
            }
        }
        return 0;
    }

    void ASMManager::Impl::HandlePayload(const std::string &payload, const uint8_t fillBitNum)
    {
        AISBitsManager aisBitsManager;

        aisBitsManager.SetVDMPayloadToDecode(payload, fillBitNum);

        auto DAC = aisBitsManager.DecodeToNumerical(0, 10);
        auto FI = aisBitsManager.DecodeToNumerical(10, 6);
        auto appID = static_cast<uint32_t>(DAC * 100 + FI);
        SPDLOG_DEBUG("HadleASM: DAC = {}, FI = {}, appID = {}", DAC, FI, appID);
        if (m_asmParserMap.find(appID) != m_asmParserMap.end())
        {
            m_asmParserMap[appID](aisBitsManager);
        }           
    }

    void ASMManager::Impl::ParseADM(const std::string &sentence)
    {
       
    }

    /*
    *          1  2  3   4  5 6 7    8          9        10  11
    *          |  |  |   |  | | |    |          |        |   |
    *  $--ASM,x.x,xx,xx,x.x,x,x,x,xxxxxxxxxx,xxxxxxxxxx,s--s,x*hh<CR><LF>
    *  Field Number:
    *  1)  Time of reception, UTC
    *  2)  Total number of sentences needed, 01 to 99
    *  3)  Sentence number, 01 to 99
    *  4)  Sequential message identifier, 0 to 9
    *  5)  VDL channel
    *  6)  Message ID
    *  7)  Repeat indicator
    *  8)  Source ID
    *  9)  Destination ID
    * 10)  Encapsulated ASM message data
    * 11)  Number of fill-bits, 0 to 5
    */
    void ASMManager::Impl::ParseASM(const std::string &sentence)
    {
        std::vector<std::string> seps {",", "*"};

        auto strList = UtilityInterface::SplitString(sentence, seps);
        if (13 == strList.size())
        {
            if (!strList.at(2).empty() && !strList.at(3).empty())
            {
                auto totalNum = atoi(strList.at(2).c_str());
                auto sentenceNum = atoi(strList.at(3).c_str());
                int  sequentialID = -1;

                if (!strList.at(4).empty())
                {
                    sequentialID = atoi(strList.at(4).c_str());
                }
                if (sentenceNum == 1)
                {
                    m_payloadASM.clear();
                    m_timestampReceived = 0;
                    m_sequentialID = sequentialID;
                    m_mmsiSource = 0;
                    if (!strList.at(8).empty())
                    {
                        m_mmsiSource = strtoul(strList.at(8).c_str(), nullptr, 10);
                    }
                    m_mmsiDestination = 0;
                    if (strList.size() > 9 && !strList.at(9).empty())
                    {
                        m_mmsiDestination = strtoul(strList.at(9).c_str(), nullptr, 10);
                    }
                }

                if (m_sequentialID == sequentialID)
                {
                    m_payloadASM += strList.at(10);
                }

                if (totalNum == sentenceNum)
                {
                    auto fillBitNum = atoi(strList.at(11).c_str());
                    
                    HandlePayload(m_payloadASM, fillBitNum);
                } 
            }
        }
    }

    void ASMManager::Impl::ParseASMDAC412FI26(const AISBitsManager &manager)
    {
        ASM_DAC412_FI26 asmInfo;
        ASM_DAC412_FI26::LocationInfo location;

        asmInfo.DAC = 412;
        asmInfo.FI = 26;

        asmInfo.DAC         = static_cast<uint16_t>(manager.DecodeToNumerical(0, 10));
        asmInfo.FI          = static_cast<uint8_t>(manager.DecodeToNumerical(10, 6));
        asmInfo.hourPublish = static_cast<uint8_t>(manager.DecodeToNumerical(16, 5));
        asmInfo.hoursOffsetFCST = static_cast<uint8_t>(manager.DecodeToNumerical(21, 7));
       
        auto totalBitsNum = manager.GetBitsNumberToDecode();
        auto surplusLocationNum = (totalBitsNum - 110) / 64;
        auto index = 28;

        // Obtatin the first location info
        location.weatherCode = static_cast<uint8_t>(manager.DecodeToNumerical(index, 5));
        auto value = manager.DecodeToNumerical(index + 5, 15);
        auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 15);
        location.coordinate.SetLongitude(longitude / 60.0);

        value = manager.DecodeToNumerical(index + 20, 14);
        auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 14);
        location.coordinate.SetLatitude(latitude / 60.0);

        location.windSpeed = static_cast<uint8_t>(manager.DecodeToNumerical(index + 34, 7));
        location.windDirection = static_cast<uint16_t>(manager.DecodeToNumerical(index + 41, 9));
        value = manager.DecodeToNumerical(index + 50, 11);
        location.temperature = static_cast<int16_t>(UtilityInterface::ConvertComplementCodeToInteger(value, 11));
        
        value = manager.DecodeToNumerical(index + 61, 10);
        location.airPressure = static_cast<uint16_t>(value);

        location.visibility = static_cast<uint8_t>(manager.DecodeToNumerical(index + 71, 8));
        asmInfo.locationInfos.push_back(location);

        index = 107;
        for (auto i = 0U; i < surplusLocationNum; i++)
        {
            auto &locationPrev = asmInfo.locationInfos.back();

            location.weatherCode = static_cast<uint8_t>(manager.DecodeToNumerical(index, 5));
            auto value = manager.DecodeToNumerical(index + 5, 7);
            auto increment = UtilityInterface::ConvertComplementCodeToInteger(value, 7);
            auto longitude = locationPrev.coordinate.GetLongitude() + increment / 60.0;
            
            value = manager.DecodeToNumerical(index + 12, 7);
            auto incrementLat = UtilityInterface::ConvertComplementCodeToInteger(value, 7);
            auto latitude = locationPrev.coordinate.GetLatitude() + incrementLat / 60.0;

            location.coordinate.SetLongitude(longitude);
            location.coordinate.SetLatitude(latitude);

            location.windSpeed = static_cast<uint8_t>(manager.DecodeToNumerical(index + 19, 7));
            location.windDirection = static_cast<uint16_t>(manager.DecodeToNumerical(index + 26, 9));
            value = manager.DecodeToNumerical(index + 35, 11);
            location.temperature = static_cast<int16_t>(UtilityInterface::ConvertComplementCodeToInteger(value, 11));

            value = manager.DecodeToNumerical(index + 46, 10);
            location.airPressure = static_cast<uint16_t>(value);
            location.visibility = static_cast<uint8_t>(manager.DecodeToNumerical(index + 56, 8));
            asmInfo.locationInfos.push_back(location);
            index += 64;
        }
        asmInfo.infoSource = static_cast<uint8_t>(manager.DecodeToNumerical(index, 3));

        m_parent->asmNotify(std::make_shared<ASM_DAC412_FI26>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI27(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_27 asmInfo;
        ASM_DAC_412_FI_27::SeaAreaInfo seaAreaInfo;

        asmInfo.DAC = 412;
        asmInfo.FI = 27;

        asmInfo.hourPublish = static_cast<uint8_t>(manager.DecodeToNumerical(16, 5));
        asmInfo.hoursOffsetFCST = static_cast<uint8_t>(manager.DecodeToNumerical(21, 7));

        auto totalBitsNum = manager.GetBitsNumberToDecode();
        auto areaNum = (totalBitsNum - 31) / 36;

        auto index = 28;
        for (auto i = 0U; i < areaNum; i++)
        {
            auto value = manager.DecodeToNumerical(index, 7);
            seaAreaInfo.areaCode = value;

            value = manager.DecodeToNumerical(index + 7, 5);
            seaAreaInfo.weatherCode = value;

            value = manager.DecodeToNumerical(index + 12, 9);
            seaAreaInfo.windDirection = value;

            value = manager.DecodeToNumerical(index + 21, 5);
            seaAreaInfo.windScaleLow = value;

            value = manager.DecodeToNumerical(index + 26, 5);
            seaAreaInfo.windScaleHigh = value;

            value = manager.DecodeToNumerical(index + 31, 5);
            seaAreaInfo.visibility = value;

            asmInfo.seaAreaInfos.push_back(seaAreaInfo);

            index += 36;
        }
        asmInfo.infoSource = static_cast<uint8_t>(manager.DecodeToNumerical(index, 3));

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_27>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI28(const AISBitsManager &manager)
    {
        ASM_DAC412_FI28 asmInfo;
        ASM_DAC412_FI28::LocationInfo location;

        asmInfo.DAC = 412;
        asmInfo.FI = 28;

        asmInfo.hourPublish = static_cast<uint8_t>(manager.DecodeToNumerical(16, 5));
        asmInfo.hoursOffsetFCST = static_cast<uint8_t>(manager.DecodeToNumerical(21, 7));

        auto index = 28;

        // Obtatin the first location info
      
        auto value = manager.DecodeToNumerical(index, 15);
        auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 15);
        location.coordinate.SetLongitude(longitude / 60.0);

        value = manager.DecodeToNumerical(index + 15, 14);
        auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 14);
        location.coordinate.SetLatitude(latitude / 60.0);

        location.flowVelocity = manager.DecodeToNumerical(index + 29, 10);
        location.flowDirection = static_cast<uint16_t>(manager.DecodeToNumerical(index + 39, 9));
        
        location.waveHeight = static_cast<uint8_t>(manager.DecodeToNumerical(index + 48, 8));
        location.waveDirection = static_cast<uint16_t>(manager.DecodeToNumerical(index + 56, 9));
        location.temperature = static_cast<uint16_t>(manager.DecodeToNumerical(index + 65, 10));

        asmInfo.locationInfos.push_back(location);

        index = 103;
        auto surplusLocationNum = (manager.GetBitsNumberToDecode() - index - 3) / 60;

        for (auto i = 0U; i < surplusLocationNum; i++)
        {
            auto &locationPrev = asmInfo.locationInfos.back();
            
            auto value = manager.DecodeToNumerical(index, 7);
            auto increment = UtilityInterface::ConvertComplementCodeToInteger(value, 7);
            auto longitude = locationPrev.coordinate.GetLongitude() + increment / 60.0;
            location.coordinate.SetLongitude(longitude);

            value = manager.DecodeToNumerical(index + 7, 7);
            auto incrementLat = UtilityInterface::ConvertComplementCodeToInteger(value, 7);
            auto latitude = locationPrev.coordinate.GetLatitude() + incrementLat / 60.0;
            location.coordinate.SetLatitude(latitude);

            location.flowVelocity = manager.DecodeToNumerical(index + 14, 10);
            location.flowDirection = static_cast<uint16_t>(manager.DecodeToNumerical(index + 24, 9));

            location.waveHeight = static_cast<uint8_t>(manager.DecodeToNumerical(index + 33, 8));
            location.waveDirection = static_cast<uint16_t>(manager.DecodeToNumerical(index + 41, 9));
            location.temperature = static_cast<uint16_t>(manager.DecodeToNumerical(index + 50, 10));

            asmInfo.locationInfos.push_back(location);

            index += 60;
        }
        asmInfo.infoSource = static_cast<uint8_t>(manager.DecodeToNumerical(index, 3));

        m_parent->asmNotify(std::make_shared<ASM_DAC412_FI28>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI29(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_29 asmInfo;
        ASM_DAC_412_FI_29::SeaAreaInfo areaInfo;

        asmInfo.DAC = 412;
        asmInfo.FI = 29;

        asmInfo.hourPublish = static_cast<uint8_t>(manager.DecodeToNumerical(16, 5));
        asmInfo.hoursOffsetFCST = static_cast<uint8_t>(manager.DecodeToNumerical(21, 7));

        auto index = 28;

        auto areaNum = (manager.GetBitsNumberToDecode() - index - 3) / 72;

        for (auto i = 0U; i < areaNum; i++)
        {
            areaInfo.areaCode = static_cast<uint8_t>(manager.DecodeToNumerical(index, 5));
            areaInfo.temperatureLow = static_cast<uint16_t>(manager.DecodeToNumerical(index + 5, 9));
            areaInfo.temperatureHigh = static_cast<uint8_t>(manager.DecodeToNumerical(index + 14, 8));
            areaInfo.flowDirectionAvg = static_cast<uint32_t>(manager.DecodeToNumerical(index + 22, 9));
            areaInfo.flowDirctionMax = static_cast<uint32_t>(manager.DecodeToNumerical(index + 31, 9));
            areaInfo.flowVelocityAvg = static_cast<uint8_t>(manager.DecodeToNumerical(index + 40, 8));
            areaInfo.flowVelocityMax = static_cast<uint8_t>(manager.DecodeToNumerical(index + 48, 8));
            areaInfo.waveHeight = static_cast<uint8_t>(manager.DecodeToNumerical(index + 56, 8));
            areaInfo.swellHeight = static_cast<uint8_t>(manager.DecodeToNumerical(index + 64, 8));
            asmInfo.seaAreaInfos.push_back(areaInfo);
            index += 72;
        }

        asmInfo.infoSource = static_cast<uint8_t>(manager.DecodeToNumerical(index, 3));

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_29>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI30(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_30 asmInfo;
        ASM_DAC_412_FI_30::WaterAreaInfo areaInfo;

        asmInfo.DAC = 412;
        asmInfo.FI = 30;

        asmInfo.hourPublish = static_cast<uint8_t>(manager.DecodeToNumerical(16, 5));
        asmInfo.hoursOffsetFCST = static_cast<uint8_t>(manager.DecodeToNumerical(21, 7));

        auto index = 28;

        auto areaNum = (manager.GetBitsNumberToDecode() - index - 3) / 93;

        for (auto i = 0U; i < areaNum; i++)
        {
            areaInfo.areaCode = static_cast<uint8_t>(manager.DecodeToNumerical(index, 8));
            areaInfo.temperatureLow = static_cast<uint16_t>(manager.DecodeToNumerical(index + 8, 9));
            areaInfo.temperatureHigh = static_cast<uint8_t>(manager.DecodeToNumerical(index + 17, 8));
            areaInfo.waveHeightLow = static_cast<uint8_t>(manager.DecodeToNumerical(index + 25, 8));
            areaInfo.waveHeightHigh = static_cast<uint8_t>(manager.DecodeToNumerical(index + 33, 8));

            areaInfo.timestampTideHigh = DecodeTime(manager, index + 41, 16);
            areaInfo.tideHigh = static_cast<uint16_t>(manager.DecodeToNumerical(index + 57, 10));
            
            areaInfo.timestampTideLow = DecodeTime(manager, index + 67, 16);
            areaInfo.tideLow = static_cast<uint16_t>(manager.DecodeToNumerical(index + 83, 10));
            asmInfo.waterAreaInfos.push_back(areaInfo);
            index += 93;
        }

        asmInfo.infoSource = static_cast<uint8_t>(manager.DecodeToNumerical(index, 3));

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_30>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI32(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_32 asmInfo;
        ASM_DAC_412_FI_32::TideInfo tideInfo;

        asmInfo.DAC = 412;
        asmInfo.FI = 32;

        asmInfo.hourPublish = static_cast<uint8_t>(manager.DecodeToNumerical(16, 5));

        auto value = manager.DecodeToNumerical(21, 15);
        auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 15);
        tideInfo.coordinate.SetLongitude(longitude / 60.0);

        value = manager.DecodeToNumerical(36, 14);
        auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 14);
        tideInfo.coordinate.SetLatitude(latitude / 60.0);

        value = manager.DecodeToNumerical(50, 9);
        tideInfo.tidalDatum = static_cast<uint16_t>(value);

        tideInfo.timestampTideHigh = DecodeTime(manager, 59, 16);

        value = manager.DecodeToNumerical(75, 10);
        tideInfo.tideHigh = static_cast<uint16_t>(value);

        tideInfo.timestampTideLow = DecodeTime(manager, 85, 16);
        value = manager.DecodeToNumerical(101, 9);
        tideInfo.tideLow = static_cast<uint16_t>(value);

        asmInfo.tideInfos.push_back(tideInfo);

        auto index = 110;
        auto surplusTideNum = (manager.GetBitsNumberToDecode() - index - 3) / 74;

        for (auto i = 0U; i < surplusTideNum; i++)
        {
            Coordinate coordinate;
            auto &tideInfoPrev = asmInfo.tideInfos.back();
            auto coordinatePrev = tideInfoPrev.coordinate;

            auto value = manager.DecodeToNumerical(index, 7);
            auto increment = UtilityInterface::ConvertComplementCodeToInteger(value, 7);
            coordinate.SetLongitude(coordinatePrev.GetLongitude() + increment / 60.0);

            value = manager.DecodeToNumerical(index + 7, 7);
            increment = UtilityInterface::ConvertComplementCodeToInteger(value, 7);
            coordinate.SetLatitude(coordinatePrev.GetLatitude() + increment / 60.0);
            tideInfo.coordinate = coordinate;

            value = manager.DecodeToNumerical(index + 14, 9);
            tideInfo.tidalDatum = static_cast<uint16_t>(value);

            tideInfo.timestampTideHigh = DecodeTime(manager, index + 23, 16);
            value = manager.DecodeToNumerical(index + 39, 10);
            tideInfo.tideHigh = static_cast<uint16_t>(value);

            tideInfo.timestampTideLow = DecodeTime(manager, index + 49, 16);
            value = manager.DecodeToNumerical(index + 65, 9);
            tideInfo.tideLow = static_cast<uint16_t>(value);

            asmInfo.tideInfos.push_back(tideInfo);
            index += 74;
        }

        asmInfo.infoSource = static_cast<uint8_t>(manager.DecodeToNumerical(index, 3));

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_32>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI35(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_35 asmInfo;

        asmInfo.DAC = 412;
        asmInfo.FI = 35;

        asmInfo.MRN = manager.DecodeToNumerical(16, 17);
        asmInfo.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(33, 2));
        asmInfo.type = static_cast<uint8_t>(manager.DecodeToNumerical(35, 5));
        asmInfo.geometryType = static_cast<uint8_t>(manager.DecodeToNumerical(40, 2));

        uint32_t index = 42;

        if (asmInfo.geometryType == 0)
        {
            auto lonVal = manager.DecodeToNumerical(42, 28);
            auto latVal = manager.DecodeToNumerical(70, 27);
            asmInfo.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
            asmInfo.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
            asmInfo.range = static_cast<uint8_t>(manager.DecodeToNumerical(97, 7));
            index = 104;
        }
        else if (asmInfo.geometryType == 1)
        {
            auto lonVal = manager.DecodeToNumerical(42, 28);
            auto latVal = manager.DecodeToNumerical(70, 27);
            asmInfo.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
            asmInfo.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
            asmInfo.sector.startAngle = static_cast<uint16_t>(manager.DecodeToNumerical(97, 9));
            asmInfo.sector.endAngle = static_cast<uint16_t>(manager.DecodeToNumerical(106, 9));
            asmInfo.sector.range = static_cast<uint8_t>(manager.DecodeToNumerical(115, 7));
            index = 122;
        }
        else if (asmInfo.geometryType == 2 || asmInfo.geometryType == 3)
        {
            Coordinate coord;
            auto lonVal = manager.DecodeToNumerical(42, 22);
            auto latVal = manager.DecodeToNumerical(64, 21);
            coord.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 22) / 6000.0);
            coord.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 21) / 6000.0);
            asmInfo.polygon.push_back(coord);

            index = 85;
            auto surplusCoordinateNum = (manager.GetBitsNumberToDecode() - index - 26) / 33;

            for (auto i = 0U; i < surplusCoordinateNum; i++)
            {
                const auto &coordinatePrev = asmInfo.polygon.back();
                auto valueLon = manager.DecodeToNumerical(index, 17);
                auto valueLat = manager.DecodeToNumerical(index + 17, 16);

                Coordinate nextCoord;
                nextCoord.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(valueLon, 17) / 6000.0 + coordinatePrev.GetLongitude());
                nextCoord.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(valueLat, 16) / 6000.0 + coordinatePrev.GetLatitude());
                asmInfo.polygon.push_back(nextCoord);
                index += 33;
            }
        }

        asmInfo.timestampActivate = DecodeTime(manager, index, 20);
        asmInfo.duration = static_cast<uint8_t>(manager.DecodeToNumerical(index + 20, 2));
        asmInfo.cautionCode = static_cast<uint8_t>(manager.DecodeToNumerical(index + 22, 4));

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_35>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI36(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_36 asmInfo;
        
        asmInfo.DAC = 412;
        asmInfo.FI = 36;

        asmInfo.MRN = manager.DecodeToNumerical(16, 17);
        asmInfo.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(33, 2));
        asmInfo.type = static_cast<uint8_t>(manager.DecodeToNumerical(35, 6));
        asmInfo.mmsi = manager.DecodeToNumerical(41, 30);
        asmInfo.geometryType = static_cast<uint8_t>(manager.DecodeToNumerical(71, 2));

        uint32_t index = 73;

        if (asmInfo.geometryType == 0)
        {
            auto lonVal = manager.DecodeToNumerical(73, 22);
            auto latVal = manager.DecodeToNumerical(95, 21);
            asmInfo.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 22) / 6000.0);
            asmInfo.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 21) / 6000.0);
            asmInfo.range = manager.DecodeToNumerical(116, 7) * 0.1; // radius in nm
            index = 123;
        }
        else if (asmInfo.geometryType == 1)
        {
            auto lonVal = manager.DecodeToNumerical(73, 22);
            auto latVal = manager.DecodeToNumerical(95, 21);
            asmInfo.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 22) / 6000.0);
            asmInfo.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 21) / 6000.0);
            asmInfo.sector.startAngle = static_cast<uint16_t>(manager.DecodeToNumerical(116, 9));
            asmInfo.sector.endAngle = static_cast<uint16_t>(manager.DecodeToNumerical(125, 9));
            asmInfo.sector.range = manager.DecodeToNumerical(134, 7) * 0.1; // radius in nm
            index = 141;
        }
        else if (asmInfo.geometryType == 2 || asmInfo.geometryType == 3)
        {
            Coordinate coord;
            auto lonVal = manager.DecodeToNumerical(73, 22);
            auto latVal = manager.DecodeToNumerical(95, 21);
            coord.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 22) / 6000.0);
            coord.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 21) / 6000.0);
            asmInfo.polygon.push_back(coord);

            index = 116;
            auto surplusCoordinateNum = (manager.GetBitsNumberToDecode() - index - 45) / 33;

            for (auto i = 0U; i < surplusCoordinateNum; i++)
            {
                const auto &coordinatePrev = asmInfo.polygon.back();
                auto valueLon = manager.DecodeToNumerical(index, 17);
                auto valueLat = manager.DecodeToNumerical(index + 17, 16);

                Coordinate nextCoord;
                nextCoord.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(valueLon, 17) / 6000.0 + coordinatePrev.GetLongitude());
                nextCoord.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(valueLat, 16) / 6000.0 + coordinatePrev.GetLatitude());
                asmInfo.polygon.push_back(nextCoord);
                index += 33;
            }
        }

        asmInfo.timeType = static_cast<uint8_t>(manager.DecodeToNumerical(index, 1));
        asmInfo.timestampStart = DecodeTime(manager, index + 1, 20);
        asmInfo.timestampEnd = DecodeTime(manager, index + 21, 20);
        asmInfo.cautionCode = static_cast<uint8_t>(manager.DecodeToNumerical(index + 41, 4));
      
        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_36>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI31(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_31 asmInfo;

        asmInfo.DAC = 412;
        asmInfo.FI = 31;

        asmInfo.warningType = static_cast<uint8_t>(manager.DecodeToNumerical(16, 4));

        uint32_t index = 20;
        if (asmInfo.warningType == 1)
        {
            auto totalBitsNum = manager.GetBitsNumberToDecode();
            if (totalBitsNum >= 224)
            {
                asmInfo.cyclone.MRN = manager.DecodeToNumerical(index, 17);
                asmInfo.cyclone.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(index + 17, 2));
                index += 19;

                auto numPoints = (totalBitsNum - 224) / 116 + 1;
                for (auto i = 0U; i < numPoints; i++)
                {
                    ASM_DAC_412_FI_31::TropicalCyclonePathPoint pt;
                    pt.timestamp = DecodeTime(manager, index, 16);

                    auto lonVal = manager.DecodeToNumerical(index + 16, 22);
                    auto lonInt = UtilityInterface::ConvertComplementCodeToInteger(lonVal, 22);
                    pt.centerLongitude = lonInt / 6000.0;

                    auto latVal = manager.DecodeToNumerical(index + 38, 21);
                    auto latInt = UtilityInterface::ConvertComplementCodeToInteger(latVal, 21);
                    pt.centerLatitude = latInt / 6000.0;

                    pt.cycloneType = static_cast<uint8_t>(manager.DecodeToNumerical(index + 59, 3));
                    pt.radiusWindScale7 = static_cast<uint16_t>(manager.DecodeToNumerical(index + 62, 10));
                    pt.radiusWindScale10 = static_cast<uint16_t>(manager.DecodeToNumerical(index + 72, 8));
                    pt.radiusWindScale12 = static_cast<uint16_t>(manager.DecodeToNumerical(index + 80, 7));
                    pt.moveSpeed = static_cast<uint8_t>(manager.DecodeToNumerical(index + 87, 6));
                    pt.moveDirection = static_cast<uint16_t>(manager.DecodeToNumerical(index + 93, 9));
                    pt.maxWindScale = static_cast<uint8_t>(manager.DecodeToNumerical(index + 102, 5));
                    pt.centerPressure = static_cast<uint16_t>(manager.DecodeToNumerical(index + 107, 9));

                    asmInfo.cyclone.pathPoints.push_back(pt);
                    index += 116;
                }
            }
        }
        else if (asmInfo.warningType >= 2 && asmInfo.warningType <= 4)
        {
            auto totalBitsNum = manager.GetBitsNumberToDecode();
            auto numElements = (totalBitsNum - 89) / 28;
            for (auto i = 0U; i < numElements; i++)
            {
                ASM_DAC_412_FI_31::GeneralWarningElement elem;
                elem.MRN = manager.DecodeToNumerical(index, 17);
                elem.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(index + 17, 2));
                elem.seaAreaCode = static_cast<uint8_t>(manager.DecodeToNumerical(index + 19, 7));
                elem.warningLevel = static_cast<uint8_t>(manager.DecodeToNumerical(index + 26, 2));
                asmInfo.generalWarnings.push_back(elem);
                index += 28;
            }
        }
        else if (asmInfo.warningType == 5)
        {
            auto totalBitsNum = manager.GetBitsNumberToDecode();
            auto numElements = (totalBitsNum - 89) / 32;
            for (auto i = 0U; i < numElements; i++)
            {
                ASM_DAC_412_FI_31::StormSurgeElement elem;
                elem.MRN = manager.DecodeToNumerical(index, 17);
                elem.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(index + 17, 2));
                elem.cityCode = static_cast<uint8_t>(manager.DecodeToNumerical(index + 19, 6));
                elem.surgeHeight = manager.DecodeToNumerical(index + 25, 5);
                elem.warningLevel = static_cast<uint8_t>(manager.DecodeToNumerical(index + 30, 2));
                asmInfo.stormSurges.push_back(elem);
                index += 32;
            }
        }
        else if (asmInfo.warningType == 6)
        {
            asmInfo.iceWarning.MRN = manager.DecodeToNumerical(20, 17);
            asmInfo.iceWarning.regionCode = static_cast<uint8_t>(manager.DecodeToNumerical(37, 7));
            asmInfo.iceWarning.warningLevel = static_cast<uint8_t>(manager.DecodeToNumerical(44, 2));
            index = 51; // 20 + 31 bits
        }

        asmInfo.publishTime = DecodeTime(manager, index, 20);
        asmInfo.startTime = DecodeTime(manager, index + 20, 20);
        asmInfo.endTime = DecodeTime(manager, index + 40, 20);
        asmInfo.warningDuration = static_cast<uint8_t>(manager.DecodeToNumerical(index + 60, 6));
        asmInfo.infoSource = static_cast<uint8_t>(manager.DecodeToNumerical(index + 66, 3));

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_31>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI37(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_37 asmInfo;

        asmInfo.DAC = 412;
        asmInfo.FI = 37;

        asmInfo.MRN = manager.DecodeToNumerical(16, 17);
        asmInfo.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(33, 2));
        asmInfo.mmsi = manager.DecodeToNumerical(35, 30);
        
        auto value = manager.DecodeToNumerical(65, 22);
        auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 22);
        asmInfo.node1.SetLongitude(longitude / 6000.0);

        value = manager.DecodeToNumerical(87, 21);
        auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 21);
        asmInfo.node1.SetLatitude(latitude / 6000.0);

        value = manager.DecodeToNumerical(108, 22);
        longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 22);
        asmInfo.node2.SetLongitude(longitude / 6000.0);

        value = manager.DecodeToNumerical(130, 21);
        latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 21);
        asmInfo.node2.SetLatitude(latitude / 6000.0);

        asmInfo.towingMethod = static_cast<uint8_t>(manager.DecodeToNumerical(151, 2));
        asmInfo.length = manager.DecodeToNumerical(153, 12);
        asmInfo.width = static_cast<uint8_t>(manager.DecodeToNumerical(165, 7));
        asmInfo.speed = manager.DecodeToNumerical(172, 8);
        asmInfo.timestampStart = DecodeTime(manager, 180, 20);
        asmInfo.timestampEnd = DecodeTime(manager, 200, 20);
        asmInfo.cautionCode = static_cast<uint8_t>(manager.DecodeToNumerical(220, 4));
        
        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_37>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI38(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_38 asmInfo;

        asmInfo.DAC = 412;
        asmInfo.FI = 38;

        asmInfo.MRN = manager.DecodeToNumerical(16, 17);
        asmInfo.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(33, 2));
        asmInfo.type = static_cast<uint8_t>(manager.DecodeToNumerical(35, 4));
        asmInfo.geoType = static_cast<uint8_t>(manager.DecodeToNumerical(39, 2));
        
        uint32_t index = 41;

        if (asmInfo.geoType == 0)
        {
            auto value = manager.DecodeToNumerical(41, 22);
            auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 22);
            asmInfo.coordinate.SetLongitude(longitude / 6000.0);

            value = manager.DecodeToNumerical(63, 21);
            auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 21);
            asmInfo.coordinate.SetLatitude(latitude / 6000.0);
            asmInfo.range = manager.DecodeToNumerical(84, 7) * 0.1;
            index = 91;
        }
        else if (asmInfo.geoType == 1)
        {
            auto value = manager.DecodeToNumerical(41, 22);
            auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 22);
            asmInfo.coordinate.SetLongitude(longitude / 6000.0);

            value = manager.DecodeToNumerical(63, 21);
            auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 21);
            asmInfo.coordinate.SetLatitude(latitude / 6000.0);

            asmInfo.sector.startAngle = static_cast<uint16_t>(manager.DecodeToNumerical(84, 9));
            asmInfo.sector.endAngle = static_cast<uint16_t>(manager.DecodeToNumerical(93, 9));
            asmInfo.range = manager.DecodeToNumerical(102, 7) * 0.1;
            asmInfo.sector.range = asmInfo.range;
            index = 109;
        }
        else if (asmInfo.geoType == 2 || asmInfo.geoType == 3)
        {
            Coordinate coordinate; 
            auto value = manager.DecodeToNumerical(41, 22);
            auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 22);
            coordinate.SetLongitude(longitude / 6000.0);

            value = manager.DecodeToNumerical(63, 21);
            auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 21);
            coordinate.SetLatitude(latitude / 6000.0);
            asmInfo.polygon.push_back(coordinate);

            index = 84;

            auto surplusCoordinateNum = (manager.GetBitsNumberToDecode() - index - 45) / 33;

            for (auto i = 0U; i < surplusCoordinateNum; i++)
            {
                auto &coordinatePrev = asmInfo.polygon.back();
                auto valLon = manager.DecodeToNumerical(index, 17);
                auto offsetLon = UtilityInterface::ConvertComplementCodeToInteger(valLon, 17);
                coordinate.SetLongitude(offsetLon / 6000.0 + coordinatePrev.GetLongitude());

                auto valLat = manager.DecodeToNumerical(index + 17, 16);
                auto offsetLat = UtilityInterface::ConvertComplementCodeToInteger(valLat, 16);
                coordinate.SetLatitude(offsetLat / 6000.0 + coordinatePrev.GetLatitude());

                asmInfo.polygon.push_back(coordinate);
                index += 33;
            }
        }

        asmInfo.timeType = static_cast<uint8_t>(manager.DecodeToNumerical(index, 1));
        asmInfo.timestampStart = DecodeTime(manager, index + 1, 20);
        asmInfo.timestampEnd = DecodeTime(manager, index + 21, 20);
        asmInfo.cautionCode = static_cast<uint8_t>(manager.DecodeToNumerical(index + 41, 4));

        SPDLOG_DEBUG("Impl::ParseASMDAC412FI38: geoType={}", asmInfo.geoType);
        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_38>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI39(const AISBitsManager &manager)
    {
        if (manager.GetBitsNumberToDecode() < 136)
        {
            SPDLOG_WARN("Impl::ParseASMDAC412FI39: bit count is less than 136 bits: {}", manager.GetBitsNumberToDecode());
            return;
        }

        ASM_DAC_412_FI_39 asmInfo;

        asmInfo.DAC               = 412;
        asmInfo.FI                = 39;
        asmInfo.MRN               = manager.DecodeToNumerical(16, 17);
        asmInfo.fragment          = static_cast<uint8_t>(manager.DecodeToNumerical(33, 2));
        asmInfo.distressType      = static_cast<uint8_t>(manager.DecodeToNumerical(35, 4));
        asmInfo.statusDescription = static_cast<uint16_t>(manager.DecodeToNumerical(39, 16));
        asmInfo.judgment          = static_cast<uint8_t>(manager.DecodeToNumerical(55, 3));

        auto lonVal = manager.DecodeToNumerical(58, 22);
        auto longitude = UtilityInterface::ConvertComplementCodeToInteger(lonVal, 22);
        asmInfo.coordinate.SetLongitude(longitude / 6000.0);

        auto latVal = manager.DecodeToNumerical(80, 21);
        auto latitude = UtilityInterface::ConvertComplementCodeToInteger(latVal, 21);
        asmInfo.coordinate.SetLatitude(latitude / 6000.0);

        asmInfo.locationInstruction = static_cast<uint8_t>(manager.DecodeToNumerical(101, 1));
        asmInfo.distressTime        = DecodeTime(manager, 102, 20);
        asmInfo.duration            = static_cast<uint8_t>(manager.DecodeToNumerical(122, 6));
        asmInfo.cautionCode         = static_cast<uint8_t>(manager.DecodeToNumerical(128, 4));

        SPDLOG_DEBUG("Impl::ParseASMDAC412FI39: distressType={}, duration={}", asmInfo.distressType, asmInfo.duration);
        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_39>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI40(const AISBitsManager &manager)
    {
        if (manager.GetBitsNumberToDecode() < 87)
        {
            SPDLOG_WARN("Impl::ParseASMDAC412FI40: bit count is too small: {}", manager.GetBitsNumberToDecode());
            return;
        }

        ASM_DAC_412_FI_40 asmInfo;

        asmInfo.DAC = 412;
        asmInfo.FI = 40;

        asmInfo.MRN = manager.DecodeToNumerical(16, 17);
        asmInfo.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(33, 2));
        asmInfo.type = static_cast<uint8_t>(manager.DecodeToNumerical(35, 5));
        asmInfo.geoType = static_cast<uint8_t>(manager.DecodeToNumerical(40, 2));

        uint32_t index = 42;

        if (asmInfo.geoType == 0)
        {
            auto value = manager.DecodeToNumerical(42, 22);
            auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 22);
            asmInfo.coordinate.SetLongitude(longitude / 6000.0);

            value = manager.DecodeToNumerical(64, 21);
            auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 21);
            asmInfo.coordinate.SetLatitude(latitude / 6000.0);

            asmInfo.range = manager.DecodeToNumerical(85, 7) * 0.1;
            index = 92;
        }
        else if (asmInfo.geoType == 1)
        {
            auto value = manager.DecodeToNumerical(42, 22);
            auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 22);
            asmInfo.coordinate.SetLongitude(longitude / 6000.0);

            value = manager.DecodeToNumerical(64, 21);
            auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 21);
            asmInfo.coordinate.SetLatitude(latitude / 6000.0);

            asmInfo.sector.startAngle = static_cast<uint16_t>(manager.DecodeToNumerical(85, 9));
            asmInfo.sector.endAngle = static_cast<uint16_t>(manager.DecodeToNumerical(94, 9));
            asmInfo.range = manager.DecodeToNumerical(103, 7) * 0.1;
            asmInfo.sector.range = asmInfo.range;
            index = 110;
        }
        else if (asmInfo.geoType == 2)
        {
            Coordinate coordinate;
            auto value = manager.DecodeToNumerical(42, 22);
            auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 22);
            coordinate.SetLongitude(longitude / 6000.0);

            value = manager.DecodeToNumerical(64, 21);
            auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 21);
            coordinate.SetLatitude(latitude / 6000.0);
            asmInfo.polygon.push_back(coordinate);

            index = 85;

            auto surplusCoordinateNum = (manager.GetBitsNumberToDecode() - index - 45) / 33;

            for (auto i = 0U; i < surplusCoordinateNum; i++)
            {
                auto &coordinatePrev = asmInfo.polygon.back();
                auto valLon = manager.DecodeToNumerical(index, 17);
                auto offsetLon = UtilityInterface::ConvertComplementCodeToInteger(valLon, 17);
                coordinate.SetLongitude(offsetLon / 6000.0 + coordinatePrev.GetLongitude());

                auto valLat = manager.DecodeToNumerical(index + 17, 16);
                auto offsetLat = UtilityInterface::ConvertComplementCodeToInteger(valLat, 16);
                coordinate.SetLatitude(offsetLat / 6000.0 + coordinatePrev.GetLatitude());

                asmInfo.polygon.push_back(coordinate);
                index += 33;
            }

            if (asmInfo.polygon.size() == 2)
            {
                auto pNE = asmInfo.polygon[0];
                auto pSW = asmInfo.polygon[1];
                asmInfo.polygon.clear();
                asmInfo.polygon.push_back(pNE);
                asmInfo.polygon.push_back(Coordinate(pSW.GetLatitude(), pNE.GetLongitude()));
                asmInfo.polygon.push_back(pSW);
                asmInfo.polygon.push_back(Coordinate(pNE.GetLatitude(), pSW.GetLongitude()));
            }
        }

        asmInfo.timeType = static_cast<uint8_t>(manager.DecodeToNumerical(index, 1));
        asmInfo.timestampStart = DecodeTime(manager, index + 1, 20);
        asmInfo.timestampEnd = DecodeTime(manager, index + 21, 20);
        asmInfo.cautionCode = static_cast<uint8_t>(manager.DecodeToNumerical(index + 41, 4));

        SPDLOG_DEBUG("Impl::ParseASMDAC412FI40: geoType={}", asmInfo.geoType);
        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_40>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI33(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_33 asmInfo;

        asmInfo.DAC = 412;
        asmInfo.FI = 33;

        asmInfo.status = static_cast<uint8_t>(manager.DecodeToNumerical(16, 5));

        uint32_t L = 0;
        switch (asmInfo.status)
        {
        case 1:  L = 112; break;
        case 2:
        case 3:
        case 6:  L = 79;  break;
        case 4:
        case 7:  L = 135; break;
        case 5:  L = 83;  break;
        case 8:  L = 102; break;
        case 9:  L = 84;  break;
        case 10:
        case 11:
        case 12:
        case 13: L = 74;  break;
        case 14: L = 119; break;
        case 15: L = 174; break;
        default: L = 0;   break;
        }

        uint32_t n = 0;
        if (L > 0 && manager.GetBitsNumberToDecode() >= 25)
        {
            n = (manager.GetBitsNumberToDecode() - 25) / L;
        }

        for (uint32_t i = 0; i < n; ++i)
        {
            uint32_t pos = 21 + i * L;
            ASM_DAC_412_FI_33::Element elem;

            switch (asmInfo.status)
            {
            case 1:
                {
                    elem.MRN = manager.DecodeToNumerical(pos, 17);
                    elem.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 17, 2));
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 19, 5));
                    auto lonVal = manager.DecodeToNumerical(pos + 24, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 52, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                    elem.rhythmNameCode = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 79, 5));
                    elem.rhythmParamCode = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 84, 5));
                    elem.bodyColor = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 89, 5));
                    elem.lightColor = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 94, 4));
                    elem.lightPeriod = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 98, 4));
                    elem.range = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 102, 5));
                    elem.morseCode = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 107, 5));
                }
                break;
            case 2:
            case 3:
            case 6:
                {
                    elem.MRN = manager.DecodeToNumerical(pos, 17);
                    elem.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 17, 2));
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 19, 5));
                    auto lonVal = manager.DecodeToNumerical(pos + 24, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 52, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                }
                break;
            case 4:
            case 7:
                {
                    elem.MRN = manager.DecodeToNumerical(pos, 17);
                    elem.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 17, 2));
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 19, 5));
                    auto prevLon = manager.DecodeToNumerical(pos + 24, 28);
                    auto prevLat = manager.DecodeToNumerical(pos + 52, 27);
                    elem.prevCoordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(prevLon, 28) / 600000.0);
                    elem.prevCoordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(prevLat, 27) / 600000.0);
                    elem.isRoughPosition = manager.DecodeToNumerical(pos + 79, 1) == 1;
                    auto lonVal = manager.DecodeToNumerical(pos + 80, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 108, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                }
                break;

            case 5:
                {
                    elem.MRN = manager.DecodeToNumerical(pos, 17);
                    elem.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 17, 2));
                    elem.failureType = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 19, 4));
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 23, 5));
                    auto lonVal = manager.DecodeToNumerical(pos + 28, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 56, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                }
                break;
            case 8:
                {
                    elem.MRN = manager.DecodeToNumerical(pos, 17);
                    elem.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 17, 2));
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 19, 5));
                    auto lonVal = manager.DecodeToNumerical(pos + 24, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 52, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                    elem.rhythmNameCode = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 79, 5));
                    elem.rhythmParamCode = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 84, 5));
                    elem.lightColor = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 89, 4));
                    elem.lightPeriod = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 93, 4));
                    elem.range = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 97, 5));
                }
                break;
            case 9:
                {
                    elem.MRN = manager.DecodeToNumerical(pos, 17);
                    elem.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 17, 2));
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 19, 5));
                    auto lonVal = manager.DecodeToNumerical(pos + 24, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 52, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                    elem.morseCode = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 79, 5));
                }
                break;
            case 10:
            case 11:
            case 12:
            case 13:
                {
                    elem.MRN = manager.DecodeToNumerical(pos, 17);
                    elem.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 17, 2));
                    auto lonVal = manager.DecodeToNumerical(pos + 19, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 47, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                }
                break;
            case 14:
                {
                    elem.MRN = manager.DecodeToNumerical(pos, 17);
                    elem.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 17, 2));
                    elem.timestampStart = DecodeTime(manager, pos + 19, 20);
                    elem.timestampEnd = DecodeTime(manager, pos + 39, 20);
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 59, 5));
                    auto lonVal = manager.DecodeToNumerical(pos + 64, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 92, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                }
                break;

            case 15:
                {
                    elem.MRN = manager.DecodeToNumerical(pos, 17);
                    elem.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 17, 2));
                    elem.timestampStart = DecodeTime(manager, pos + 19, 20);
                    elem.timestampEnd = DecodeTime(manager, pos + 39, 20);
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 59, 5));
                    auto prevLon = manager.DecodeToNumerical(pos + 64, 28);
                    auto prevLat = manager.DecodeToNumerical(pos + 92, 27);
                    elem.prevCoordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(prevLon, 28) / 600000.0);
                    elem.prevCoordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(prevLat, 27) / 600000.0);
                    auto lonVal = manager.DecodeToNumerical(pos + 119, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 147, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                }
                break;
            default:
                break;
            }

            asmInfo.elements.push_back(elem);
        }

        uint32_t posEnd = 21 + n * L;
        if (manager.GetBitsNumberToDecode() >= posEnd + 4)
        {
            asmInfo.precaution = static_cast<uint8_t>(manager.DecodeToNumerical(posEnd, 4));
        }

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_33>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI34(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_34 asmInfo;
        
        asmInfo.DAC = 412;
        asmInfo.FI = 34;

        asmInfo.atonAttribute = static_cast<uint8_t>(manager.DecodeToNumerical(16, 3));
        asmInfo.status = static_cast<uint8_t>(manager.DecodeToNumerical(19, 4));

        uint32_t L = 0;
        switch (asmInfo.status)
        {
        case 1:  L = 115; break;
        case 2:
        case 5:
        case 6:  L = 110; break;
        case 3:
        case 4:  L = 166; break;
        default: L = 0;   break;
        }

        uint32_t n = 0;
        if (L > 0 && manager.GetBitsNumberToDecode() >= 27)
        {
            n = (manager.GetBitsNumberToDecode() - 27) / L;
        }

        for (uint32_t i = 0; i < n; ++i)
        {
            uint32_t pos = 23 + i * L;
            ASM_DAC_412_FI_34::Element elem;

            switch (asmInfo.status)
            {
            case 1:
                {
                    elem.mmsi = manager.DecodeToNumerical(pos, 30);
                    elem.MRN = manager.DecodeToNumerical(pos + 30, 17);
                    elem.fragmentDescription = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 47, 2));
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 49, 6));
                    auto lonVal = manager.DecodeToNumerical(pos + 55, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 83, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                    elem.range = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 110, 5));
                }
                break;

            case 2:
            case 5:
            case 6:
                {
                    elem.mmsi = manager.DecodeToNumerical(pos, 30);
                    elem.MRN = manager.DecodeToNumerical(pos + 30, 17);
                    elem.fragmentDescription = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 47, 2));
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 49, 6));
                    auto lonVal = manager.DecodeToNumerical(pos + 55, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 83, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                }
                break;

            case 3:
            case 4:
                {
                    elem.mmsi = manager.DecodeToNumerical(pos, 30);
                    elem.MRN = manager.DecodeToNumerical(pos + 30, 17);
                    elem.fragmentDescription = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 47, 2));
                    elem.type = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 49, 6));
                    auto prevLon = manager.DecodeToNumerical(pos + 55, 28);
                    auto prevLat = manager.DecodeToNumerical(pos + 83, 27);
                    elem.prevCoordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(prevLon, 28) / 600000.0);
                    elem.prevCoordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(prevLat, 27) / 600000.0);
                    elem.isRoughPosition = manager.DecodeToNumerical(pos + 110, 1) == 1;
                    auto lonVal = manager.DecodeToNumerical(pos + 111, 28);
                    auto latVal = manager.DecodeToNumerical(pos + 139, 27);
                    elem.coordinate.SetLongitude(UtilityInterface::ConvertComplementCodeToInteger(lonVal, 28) / 600000.0);
                    elem.coordinate.SetLatitude(UtilityInterface::ConvertComplementCodeToInteger(latVal, 27) / 600000.0);
                }
                break;

            default:
                break;
            }

            asmInfo.elements.push_back(elem);
        }

        uint32_t posEnd = 23 + n * L;
        if (manager.GetBitsNumberToDecode() >= posEnd + 4)
        {
            asmInfo.precaution = static_cast<uint8_t>(manager.DecodeToNumerical(posEnd, 4));
        }

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_34>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI41(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_41 asmInfo;
        ASM_DAC_412_FI_41::BridgeSpan span;

        asmInfo.DAC = 412;
        asmInfo.FI = 41;

        asmInfo.MRN = manager.DecodeToNumerical(16, 17);
        asmInfo.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(33, 2));
        asmInfo.flowVelocity = static_cast<uint16_t>(manager.DecodeToNumerical(35, 9));
        asmInfo.flowDirection = static_cast<uint16_t>(manager.DecodeToNumerical(44, 9));
        asmInfo.publicationTime = DecodeTime(manager, 53, 22);

        auto index = 75;

        auto spanNum = (manager.GetBitsNumberToDecode() - index) / 88;

        for (auto i = 0U; i < spanNum; i++)
        {
            auto value = manager.DecodeToNumerical(index, 28);
            auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 28);
            span.center.SetLongitude(longitude / 600000.0);

            value = manager.DecodeToNumerical(index + 28, 27);
            auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 27);
            span.center.SetLatitude(latitude / 600000.0);

            span.height = manager.DecodeToNumerical(index + 55, 10);
            span.width = manager.DecodeToNumerical(index + 65, 10);
            span.passAbility = static_cast<uint8_t>(manager.DecodeToNumerical(index + 75, 2));
            span.directionToPass = static_cast<uint16_t>(manager.DecodeToNumerical(index + 77, 9));
            span.enableMeeting = manager.DecodeToNumerical(index + 86, 1) != 0;
            span.enableOvertaking = manager.DecodeToNumerical(index + 87, 1) != 0;
            asmInfo.bridgeSpans.push_back(span);
            index += 88;
        }

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_41>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI45(const AISBitsManager &manager)
    {
        if (manager.GetBitsNumberToDecode() < 92)
        {
            SPDLOG_WARN("Payload too short for DAC 412 FI 45: {} bits", manager.GetBitsNumberToDecode());
            return;
        }

        ASM_DAC_412_FI_45 asmInfo;
        asmInfo.DAC = 412;
        asmInfo.FI = 45;

        asmInfo.type = static_cast<uint8_t>(manager.DecodeToNumerical(16, 4));
        asmInfo.isContinous = manager.DecodeToNumerical(20, 1) != 0;

        auto index = 21;

        // Decode first point
        ASM_DAC_412_FI_45::NetInfo firstNet;
        firstNet.MRN = static_cast<uint32_t>(manager.DecodeToNumerical(index, 20));
        firstNet.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(index + 20, 2));

        auto value = manager.DecodeToNumerical(index + 22, 25);
        value = UtilityInterface::ConvertComplementCodeToInteger(value, 25);
        firstNet.coordinate.SetLongitude(value / 60000.0);

        value = manager.DecodeToNumerical(index + 47, 24);
        value = UtilityInterface::ConvertComplementCodeToInteger(value, 24);
        firstNet.coordinate.SetLatitude(value / 60000.0);

        asmInfo.nets.push_back(firstNet);

        index = 92;
        auto surplusCoordinateNum = (manager.GetBitsNumberToDecode() - index) / 51;

        for (auto i = 0U; i < surplusCoordinateNum; i++)
        {
            auto &netPrev = asmInfo.nets.back();
            ASM_DAC_412_FI_45::NetInfo net;

            net.MRN = static_cast<uint32_t>(manager.DecodeToNumerical(index, 20));
            net.fragmentDesc = static_cast<uint8_t>(manager.DecodeToNumerical(index + 20, 2));

            value = manager.DecodeToNumerical(index + 22, 15);
            value = UtilityInterface::ConvertComplementCodeToInteger(value, 15);
            auto longitude = value / 60000.0 + netPrev.coordinate.GetLongitude();
            net.coordinate.SetLongitude(longitude);

            value = manager.DecodeToNumerical(index + 37, 14);
            value = UtilityInterface::ConvertComplementCodeToInteger(value, 14);
            auto latitude = value / 60000.0 + netPrev.coordinate.GetLatitude();
            net.coordinate.SetLatitude(latitude);

            asmInfo.nets.push_back(net);
            index += 51;
        }

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_45>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI42(const AISBitsManager &manager)
    {
        ASM_DAC_412_FI_42 asmInfo;
        Coordinate        coordinate;

        asmInfo.DAC = 412;
        asmInfo.FI = 42;

        asmInfo.MRN = manager.DecodeToNumerical(16, 17);
        asmInfo.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(33, 2));
        
        auto value = manager.DecodeToNumerical(35, 28);
        auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 28);
        coordinate.SetLongitude(longitude / 600000.0);

        value = manager.DecodeToNumerical(63, 27);
        auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 27);
        coordinate.SetLatitude(latitude / 600000.0);

        asmInfo.coordinates.push_back(coordinate);

        auto index = 90; 

        auto surplusCoordinateNum = (manager.GetBitsNumberToDecode() - index - 9) / 47;

        for (auto i = 0U; i < surplusCoordinateNum; i++)
        {
            auto &coordinatePrev = asmInfo.coordinates.back();
            
            value = manager.DecodeToNumerical(index, 24);
            auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 24);
            double lonVal = longitude / 600000.0 + coordinatePrev.GetLongitude();

            value = manager.DecodeToNumerical(index + 24, 23);
            auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 23);
            double latVal = latitude / 600000.0 + coordinatePrev.GetLatitude();
            
            coordinate.SetLongitude(lonVal);
            coordinate.SetLatitude(latVal);
            asmInfo.coordinates.push_back(coordinate);
            index += 47;
        }

        asmInfo.width = manager.DecodeToNumerical(index, 9);

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_42>(asmInfo));
    }

    void ASMManager::Impl::ParseChannelBoundaryHelper(const AISBitsManager &manager, uint8_t edgeType, uint8_t fi)
    {
        ASM_DAC_412_FI_43 asmInfo;
        Coordinate        coordinate;

        asmInfo.DAC = 412;
        asmInfo.FI = fi;

        asmInfo.MRN = manager.DecodeToNumerical(16, 17);
        asmInfo.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(33, 2));
        asmInfo.edgeType = edgeType;

        auto value = manager.DecodeToNumerical(35, 28);
        auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 28);
        coordinate.SetLongitude(longitude / 600000.0);

        value = manager.DecodeToNumerical(63, 27);
        auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 27);
        coordinate.SetLatitude(latitude / 600000.0);

        asmInfo.coordinates.push_back(coordinate);

        auto index = 90; 

        auto surplusCoordinateNum = (manager.GetBitsNumberToDecode() - index) / 47;

        for (auto i = 0U; i < surplusCoordinateNum; i++)
        {
            auto &coordinatePrev = asmInfo.coordinates.back();
            
            value = manager.DecodeToNumerical(index, 24);
            auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 24);
            double lonVal = longitude / 600000.0 + coordinatePrev.GetLongitude();

            value = manager.DecodeToNumerical(index + 24, 23);
            auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 23);
            double latVal = latitude / 600000.0 + coordinatePrev.GetLatitude();
            
            coordinate.SetLongitude(lonVal);
            coordinate.SetLatitude(latVal);
            asmInfo.coordinates.push_back(coordinate);
            index += 47;
        }

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_43>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI43(const AISBitsManager &manager)
    {
        ParseChannelBoundaryHelper(manager, 0, 43);
    }

    void ASMManager::Impl::ParseASMDAC412FI44(const AISBitsManager &manager)
    {
        ParseChannelBoundaryHelper(manager, 1, 44);
    }

    void ASMManager::Impl::ParseASMDAC413FI3(const AISBitsManager &manager)
    {
        ASM_DAC_413_FI_3 asmInfo;

        asmInfo.DAC = 413;
        asmInfo.FI = 3;
        asmInfo.source = m_mmsiSource;
        asmInfo.destination = m_mmsiDestination;

        auto bitNum = manager.GetBitsNumberToDecode() - 16;
        if (bitNum >= 7)
        {
            auto text = manager.DecodeToString(16, bitNum, 413, 3);
            UtilityInterface::RemoveTailCharacter(text, '@');
            asmInfo.content = UtilityInterface::GBKToUTF8(text);
        }

        m_parent->asmNotify(std::make_shared<ASM_DAC_413_FI_3>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC413FI4(const AISBitsManager &manager)
    {
        ASM_DAC_413_FI_4 asmInfo;
        asmInfo.DAC = 413;
        asmInfo.FI = 4;
        asmInfo.source = m_mmsiSource;
        asmInfo.destination = m_mmsiDestination;

        auto bitNum = manager.GetBitsNumberToDecode() - 16;
        if (bitNum >= 7)
        {
            auto text = manager.DecodeToString(16, bitNum, 413, 4);
            UtilityInterface::RemoveTailCharacter(text, '@');
            asmInfo.content = UtilityInterface::GBKToUTF8(text);
            SPDLOG_DEBUG(asmInfo.content);
        }

        m_parent->asmNotify(std::make_shared<ASM_DAC_413_FI_4>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC413FI5(const AISBitsManager &manager)
    {
        ASM_DAC_413_FI_5 asmInfo;

        asmInfo.DAC = 413;
        asmInfo.FI = 5;

        asmInfo.promptContent = static_cast<uint8_t>(manager.DecodeToNumerical(16, 6));
        asmInfo.displayDuration = static_cast<uint8_t>(manager.DecodeToNumerical(22, 4));

        auto bitNum = manager.GetBitsNumberToDecode() - 26;
        if (bitNum >= 7)
        {
            auto text = manager.DecodeToString(26, bitNum, 413, 1);
            UtilityInterface::RemoveTailCharacter(text, '@');
            auto content = UtilityInterface::GBKToUTF8(text);
            asmInfo.text = content;
            SPDLOG_DEBUG(asmInfo.text);
        }

        m_parent->asmNotify(std::make_shared<ASM_DAC_413_FI_5>(asmInfo));
    }

    double ASMManager::Impl::DecodeCoordinate(const AISBitsManager &manager, const uint32_t startBitPos,
                                              const uint32_t bitsNum)
    {
        auto val = manager.DecodeToNumerical(startBitPos, bitsNum);
        return UtilityInterface::ConvertComplementCodeToInteger(val, bitsNum) / 600000.0;
    }

    void ASMManager::Impl::ParseASMDAC413FI7(const AISBitsManager &manager)
    {
        ASM_DAC_413_FI_7 asmInfo;
        asmInfo.DAC = 413;
        asmInfo.FI = 7;

        asmInfo.MRN = manager.DecodeToNumerical(16, 20);
        asmInfo.mainDAC = static_cast<uint16_t>(manager.DecodeToNumerical(36, 10));
        asmInfo.mainFI = static_cast<uint8_t>(manager.DecodeToNumerical(46, 6));

        if (asmInfo.mainDAC == 412 && (asmInfo.mainFI == 42 || asmInfo.mainFI == 43 || asmInfo.mainFI == 44))
        {
            uint32_t index = 52;
            if (manager.GetBitsNumberToDecode() >= index)
            {
                uint32_t surplusNum = (manager.GetBitsNumberToDecode() - index) / 47;
                for (uint32_t i = 0; i < surplusNum; ++i)
                {
                    Coordinate delta;
                    double lonInc = DecodeCoordinate(manager, index, 24);
                    double latInc = DecodeCoordinate(manager, index + 24, 23);
                    delta.SetLongitude(lonInc);
                    delta.SetLatitude(latInc);
                    asmInfo.coordinates.push_back(delta);
                    index += 47;
                }
            }
        }
        else if (asmInfo.mainDAC == 412 && asmInfo.mainFI == 31)
        {
            uint32_t index = 52;
            auto totalBits = manager.GetBitsNumberToDecode();
            if (totalBits >= index)
            {
                // Decode as Tropical Cyclone points (116 bits each)
                uint32_t cycloneNum = (totalBits - index) / 116;
                uint32_t tempIndex = index;
                for (uint32_t i = 0; i < cycloneNum; ++i)
                {
                    ASM_DAC_412_FI_31::TropicalCyclonePathPoint pt;
                    pt.timestamp = DecodeTime(manager, tempIndex, 16);
                    auto lonVal = manager.DecodeToNumerical(tempIndex + 16, 22);
                    auto lonInt = UtilityInterface::ConvertComplementCodeToInteger(lonVal, 22);
                    pt.centerLongitude = lonInt / 6000.0;
                    auto latVal = manager.DecodeToNumerical(tempIndex + 38, 21);
                    auto latInt = UtilityInterface::ConvertComplementCodeToInteger(latVal, 21);
                    pt.centerLatitude = latInt / 6000.0;
                    pt.cycloneType = static_cast<uint8_t>(manager.DecodeToNumerical(tempIndex + 59, 3));
                    pt.radiusWindScale7 = static_cast<uint16_t>(manager.DecodeToNumerical(tempIndex + 62, 10));
                    pt.radiusWindScale10 = static_cast<uint16_t>(manager.DecodeToNumerical(tempIndex + 72, 8));
                    pt.radiusWindScale12 = static_cast<uint16_t>(manager.DecodeToNumerical(tempIndex + 80, 7));
                    pt.moveSpeed = static_cast<uint8_t>(manager.DecodeToNumerical(tempIndex + 87, 6));
                    pt.moveDirection = static_cast<uint16_t>(manager.DecodeToNumerical(tempIndex + 93, 9));
                    pt.maxWindScale = static_cast<uint8_t>(manager.DecodeToNumerical(tempIndex + 102, 5));
                    pt.centerPressure = static_cast<uint16_t>(manager.DecodeToNumerical(tempIndex + 107, 9));
                    asmInfo.cyclonePathPoints.push_back(pt);
                    tempIndex += 116;
                }

                // Decode as Gale warnings (28 bits each)
                uint32_t galeNum = (totalBits - index) / 28;
                tempIndex = index;
                for (uint32_t i = 0; i < galeNum; ++i)
                {
                    ASM_DAC_412_FI_31::GeneralWarningElement elem;
                    elem.MRN = manager.DecodeToNumerical(tempIndex, 17);
                    elem.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(tempIndex + 17, 2));
                    elem.seaAreaCode = static_cast<uint8_t>(manager.DecodeToNumerical(tempIndex + 19, 7));
                    elem.warningLevel = static_cast<uint8_t>(manager.DecodeToNumerical(tempIndex + 26, 2));
                    asmInfo.galeWarnings.push_back(elem);
                    tempIndex += 28;
                }

                // Decode as Storm Surge warnings (32 bits each)
                uint32_t surgeNum = (totalBits - index) / 32;
                tempIndex = index;
                for (uint32_t i = 0; i < surgeNum; ++i)
                {
                    ASM_DAC_412_FI_31::StormSurgeElement elem;
                    elem.MRN = manager.DecodeToNumerical(tempIndex, 17);
                    elem.fragment = static_cast<uint8_t>(manager.DecodeToNumerical(tempIndex + 17, 2));
                    elem.cityCode = static_cast<uint8_t>(manager.DecodeToNumerical(tempIndex + 19, 6));
                    elem.surgeHeight = manager.DecodeToNumerical(tempIndex + 25, 5);
                    elem.warningLevel = static_cast<uint8_t>(manager.DecodeToNumerical(tempIndex + 30, 2));
                    asmInfo.stormSurges.push_back(elem);
                    tempIndex += 32;
                }
            }
        }
        m_parent->asmNotify(std::make_shared<ASM_DAC_413_FI_7>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC413FI8(const AISBitsManager &manager)
    {
        ASM_DAC_413_FI_8 asmInfo;

        asmInfo.DAC          = 413;
        asmInfo.FI           = 8;
        asmInfo.MRN          = manager.DecodeToNumerical(16, 20);
        asmInfo.mainDAC      = static_cast<uint16_t>(manager.DecodeToNumerical(36, 10));
        asmInfo.mainFI       = static_cast<uint8_t>(manager.DecodeToNumerical(46, 6));
        asmInfo.encodingType = static_cast<uint8_t>(manager.DecodeToNumerical(52, 2));
        asmInfo.textLength   = static_cast<uint16_t>(manager.DecodeToNumerical(54, 11));

        uint32_t bitNum = 0;
        int decodeDAC = 413;
        int decodeFI = 1;
        if (asmInfo.encodingType == 0)
        {
            bitNum = asmInfo.textLength * 6;
            decodeDAC = 1;
            decodeFI = 0;
        }
        else if (asmInfo.encodingType == 1)
        {
            bitNum = asmInfo.textLength * 7;
            decodeDAC = 413;
            decodeFI = 1;
        }
        else if (asmInfo.encodingType == 2)
        {
            bitNum = asmInfo.textLength;
            decodeDAC = 413;
            decodeFI = 2;
        }

        if (bitNum > 0 && manager.GetBitsNumberToDecode() >= 65 + bitNum)
        {
            auto textGBK = manager.DecodeToString(65, bitNum, decodeDAC, decodeFI);
            UtilityInterface::RemoveTailCharacter(textGBK, '@');
            auto textUTF8 = UtilityInterface::GBKToUTF8(textGBK);
            asmInfo.description = textUTF8;
        }
        m_parent->asmNotify(std::make_shared<ASM_DAC_413_FI_8>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC413FI9(const AISBitsManager &manager)
    {
        ASM_DAC_413_FI_9 asmInfo;
        asmInfo.DAC = 413;
        asmInfo.FI = 9;

        uint32_t L = 36;
        uint32_t n = 0;
        if (manager.GetBitsNumberToDecode() >= 52)
        {
            n = (manager.GetBitsNumberToDecode() - 16) / L;
        }
        else if (manager.GetBitsNumberToDecode() >= 16)
        {
            n = 0;
        }

        for (uint32_t i = 0; i < n; ++i)
        {
            uint32_t pos = 16 + i * L;
            ASM_DAC_413_FI_9::Element elem;
            elem.dac = static_cast<uint16_t>(manager.DecodeToNumerical(pos, 10));
            elem.fi = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 10, 6));
            elem.mrn = manager.DecodeToNumerical(pos + 16, 20);
            asmInfo.elements.push_back(elem);
        }

        m_parent->asmNotify(std::make_shared<ASM_DAC_413_FI_9>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC413FI10(const AISBitsManager &manager)
    {
        if (manager.GetBitsNumberToDecode() < 96)
        {
            return;
        }

        ASM_DAC_413_FI_10 asmInfo;
        asmInfo.DAC = 413;
        asmInfo.FI = 10;

        asmInfo.targetDAC = static_cast<uint16_t>(manager.DecodeToNumerical(16, 10));
        asmInfo.targetFI = static_cast<uint8_t>(manager.DecodeToNumerical(26, 6));
        asmInfo.startMRN = manager.DecodeToNumerical(32, 20);
        asmInfo.endMRN = manager.DecodeToNumerical(52, 20);

        m_parent->asmNotify(std::make_shared<ASM_DAC_413_FI_10>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI47(const AISBitsManager &manager)
    {
        if (manager.GetBitsNumberToDecode() < 77)
        {
            return;
        }

        ASM_DAC_412_FI_47 asmInfo;
        Coordinate        coordinate;

        asmInfo.DAC = 412;
        asmInfo.FI = 47;
        asmInfo.source = m_mmsiSource;

        auto rawTime = manager.DecodeToNumerical(16, 6);
        asmInfo.effectiveTime = rawTime * 0.5;

        // Longitude #1 (28 bits)
        auto value = manager.DecodeToNumerical(22, 28);
        auto longitude = UtilityInterface::ConvertComplementCodeToInteger(value, 28);
        coordinate.SetLongitude(longitude / 600000.0);

        // Latitude #1 (27 bits)
        value = manager.DecodeToNumerical(50, 27);
        auto latitude = UtilityInterface::ConvertComplementCodeToInteger(value, 27);
        coordinate.SetLatitude(latitude / 600000.0);

        asmInfo.coordinates.push_back(coordinate);

        auto index = 77;
        auto surplusCoordinateNum = (manager.GetBitsNumberToDecode() - index) / 47;

        for (auto i = 0U; i < surplusCoordinateNum; i++)
        {
            auto &coordinatePrev = asmInfo.coordinates.back();

            value = manager.DecodeToNumerical(index, 24);
            auto longitudeOffset = UtilityInterface::ConvertComplementCodeToInteger(value, 24);
            double lonVal = longitudeOffset / 600000.0 + coordinatePrev.GetLongitude();

            value = manager.DecodeToNumerical(index + 24, 23);
            auto latitudeOffset = UtilityInterface::ConvertComplementCodeToInteger(value, 23);
            double latVal = latitudeOffset / 600000.0 + coordinatePrev.GetLatitude();

            Coordinate coord;
            coord.SetLongitude(lonVal);
            coord.SetLatitude(latVal);
            asmInfo.coordinates.push_back(coord);

            index += 47;
        }
        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_47>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI48(const AISBitsManager &manager)
    {
        if (manager.GetBitsNumberToDecode() < 112)
        {
            return;
        }

        ASM_DAC_412_FI_48 asmInfo;
        asmInfo.DAC          = 412;
        asmInfo.FI           = 48;
        asmInfo.source       = m_mmsiSource;
        asmInfo.destination  = m_mmsiDestination;
        asmInfo.routeVersion = static_cast<uint8_t>(manager.DecodeToNumerical(16, 6));
        asmInfo.startTime    = DecodeTime(manager, 22, 22);

        // Waypoint #1
        ASM_DAC_412_FI_48::Waypoint wp1;
        
        auto valueLon = manager.DecodeToNumerical(44, 28);
        auto longitude = UtilityInterface::ConvertComplementCodeToInteger(valueLon, 28);
        wp1.coordinate.SetLongitude(longitude / 600000.0);

        auto valueLat = manager.DecodeToNumerical(72, 27);
        auto latitude = UtilityInterface::ConvertComplementCodeToInteger(valueLat, 27);
        wp1.coordinate.SetLatitude(latitude / 600000.0);

        wp1.timeUnit = static_cast<uint8_t>(manager.DecodeToNumerical(99, 1));
        wp1.duration = static_cast<uint16_t>(manager.DecodeToNumerical(100, 12));

        asmInfo.waypoints.push_back(wp1);

        auto index = 112;
        auto waypointNum = (manager.GetBitsNumberToDecode() - index) / 38;

        for (auto i = 0U; i < waypointNum; i++)
        {
            auto &prevWp = asmInfo.waypoints.back();

            // Longitude offset #n: 13 bits (signed 2's complement)
            auto valueOffsetLon = manager.DecodeToNumerical(index, 13);
            int32_t deltaLon = UtilityInterface::ConvertComplementCodeToInteger(valueOffsetLon, 13);
            double lonVal = prevWp.coordinate.GetLongitude() + (deltaLon / 60000.0);

            // Latitude offset #n: 12 bits (signed 2's complement)
            auto valueOffsetLat = manager.DecodeToNumerical(index + 13, 12);
            int32_t deltaLat = UtilityInterface::ConvertComplementCodeToInteger(valueOffsetLat, 12);
            double latVal = prevWp.coordinate.GetLatitude() + (deltaLat / 60000.0);

            ASM_DAC_412_FI_48::Waypoint wp;
            wp.coordinate.SetLongitude(lonVal);
            wp.coordinate.SetLatitude(latVal);
            wp.timeUnit = static_cast<uint8_t>(manager.DecodeToNumerical(index + 25, 1));
            wp.duration = static_cast<uint16_t>(manager.DecodeToNumerical(index + 26, 12));

            asmInfo.waypoints.push_back(wp);

            index += 38;
        }
        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_48>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI50(const AISBitsManager &manager)
    {
        auto totalBits = manager.GetBitsNumberToDecode();
        if (totalBits < 50)
        {
            return;
        }

        ASM_DAC_412_FI_50 asmInfo;
        asmInfo.DAC = 412;
        asmInfo.FI = 50;
        asmInfo.source = m_mmsiSource;
        asmInfo.destination = m_mmsiDestination;

        uint16_t responseInfo = manager.DecodeToNumerical(16, 14);
        asmInfo.hasWindSpeed     = (responseInfo & (1 << 0)) != 0;
        asmInfo.hasWindDirection = (responseInfo & (1 << 1)) != 0;
        asmInfo.hasVisibility    = (responseInfo & (1 << 2)) != 0;
        asmInfo.hasWaveHeight    = (responseInfo & (1 << 3)) != 0;
        asmInfo.hasWaveDirection = (responseInfo & (1 << 4)) != 0;
        asmInfo.hasSwellHeight   = (responseInfo & (1 << 5)) != 0;

        asmInfo.forecastTime = DecodeTime(manager, 30, 20);

        uint32_t L = 0;
        if (asmInfo.hasWindSpeed)     { L += 6; }
        if (asmInfo.hasWindDirection) { L += 9; }
        if (asmInfo.hasVisibility)    { L += 8; }
        if (asmInfo.hasWaveHeight)    { L += 8; }
        if (asmInfo.hasWaveDirection) { L += 9; }
        if (asmInfo.hasSwellHeight)   { L += 8; }

        if (L > 0)
        {
            uint32_t pointsCount = (totalBits - 50) / L;
            uint32_t index = 50;

            for (uint32_t i = 0; i < pointsCount; ++i)
            {
                ASM_DAC_412_FI_50::PointForecast point;

                if (asmInfo.hasWindSpeed)
                {
                    point.windSpeed = manager.DecodeToNumerical(index, 6);
                    index += 6;
                }
                if (asmInfo.hasWindDirection)
                {
                    point.windDirection = manager.DecodeToNumerical(index, 9);
                    index += 9;
                }
                if (asmInfo.hasVisibility)
                {
                    point.visibility = manager.DecodeToNumerical(index, 8);
                    index += 8;
                }
                if (asmInfo.hasWaveHeight)
                {
                    point.waveHeight = manager.DecodeToNumerical(index, 8);
                    index += 8;
                }
                if (asmInfo.hasWaveDirection)
                {
                    point.waveDirection = manager.DecodeToNumerical(index, 9);
                    index += 9;
                }
                if (asmInfo.hasSwellHeight)
                {
                    point.swellHeight = manager.DecodeToNumerical(index, 8);
                    index += 8;
                }

                asmInfo.points.push_back(point);
            }
        }

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_50>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI51(const AISBitsManager &manager)
    {
        if (manager.GetBitsNumberToDecode() < 128)
        {
            return;
        }

        ASM_DAC_412_FI_51 asmInfo;
        asmInfo.DAC = 412;
        asmInfo.FI = 51;
        asmInfo.mmsi = m_mmsiSource;

        asmInfo.extendedVesselType = manager.DecodeToNumerical(17, 8);
        asmInfo.autonomousLevel    = manager.DecodeToNumerical(25, 3);
        asmInfo.naviStatus         = manager.DecodeToNumerical(28, 4);
        
        uint8_t draughtVal = manager.DecodeToNumerical(32, 8);
        asmInfo.draught = draughtVal / 10.0;

        asmInfo.eta = DecodeTime(manager, 40, 20);

        // Decode 6-bit ASCII Port Code (5 characters)
        std::string portCode;
        for (int i = 0; i < 5; ++i)
        {
            uint8_t val = manager.DecodeToNumerical(60 + i * 6, 6);
            if (val < 32) 
            { 
                portCode.push_back(val + 64);
            } 
            else if (val < 64) 
            { 
                portCode.push_back(val);
            }
        }
        // Remove padding '@'
        while (!portCode.empty() && portCode.back() == '@')
        {
            portCode.pop_back();
        }
        asmInfo.portCode = portCode;

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_51>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI52(const AISBitsManager &manager)
    {
        auto totalBits = manager.GetBitsNumberToDecode();
        if (totalBits < 30)
        {
            return;
        }

        ASM_DAC_412_FI_52 asmInfo;
        asmInfo.DAC     = 412;
        asmInfo.FI      = 52;
        asmInfo.mmsi    = m_mmsiSource;
        asmInfo.crewNum = manager.DecodeToNumerical(17, 13);

        uint32_t n = totalBits - 30;
        uint32_t charCount = n / 14;

        std::string gbkStr;
        for (uint32_t i = 0; i < charCount; ++i)
        {
            uint32_t val = manager.DecodeToNumerical(30 + i * 14, 14);
            uint8_t b1 = val / 94 + 0xA1;
            uint8_t b2 = val % 94 + 0xA1;
            
            if (b1 == 0xA1 && b2 == 0xA1)
            {
                gbkStr.push_back(' ');
            }
            else if (b1 == 0xA3 && b2 >= 0xA1 && b2 <= 0xFE)
            {
                gbkStr.push_back(b2 - 0x80);
            }
            else if (b1 >= 0xA1 && b1 <= 0xFE && b2 >= 0xA1 && b2 <= 0xFE)
            {
                gbkStr.push_back(b1);
                gbkStr.push_back(b2);
            }
        }
            asmInfo.chineseName = UtilityInterface::GBKToUTF8(gbkStr);
        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_52>(asmInfo));
    }

    void ASMManager::Impl::ParseASMDAC412FI55(const AISBitsManager &manager)
    {
        auto totalBits = manager.GetBitsNumberToDecode();
        if (totalBits < 104)
        {
            return;
        }

        ASM_DAC_412_FI_55 asmInfo;
        asmInfo.DAC         = 412;
        asmInfo.FI          = 55;
        asmInfo.source      = m_mmsiSource;
        asmInfo.destination = m_mmsiDestination;

        // hourPublish (5 bits): bit 16-20
        asmInfo.hourPublish = static_cast<uint8_t>(manager.DecodeToNumerical(16, 5));
        // minutePublish (6 bits): bit 21-26
        asmInfo.minutePublish = static_cast<uint8_t>(manager.DecodeToNumerical(21, 6));
        // forecastTimeOffset (6 bits): bit 27-32
        asmInfo.forecastTimeOffset = static_cast<uint8_t>(manager.DecodeToNumerical(27, 6));
        // elementFlags (16 bits): bit 33-48
        asmInfo.elementFlags = static_cast<uint16_t>(manager.DecodeToNumerical(33, 16));
        // baseReference (1 bit): bit 49
        asmInfo.baseReference = static_cast<uint8_t>(manager.DecodeToNumerical(49, 1));

        uint32_t currentBit = 50;
        uint32_t D = 0;
        if (asmInfo.baseReference == 1)
        {
            asmInfo.draftRequirement = static_cast<uint8_t>(manager.DecodeToNumerical(50, 8));
            currentBit = 58;
            D = 8;
        }

        // Calculate E (element bit sum)
        uint32_t E = 0;
        if (asmInfo.elementFlags & (1 << 0)) { E += 10; }
        if (asmInfo.elementFlags & (1 << 1)) { E += 9; }
        if (asmInfo.elementFlags & (1 << 2)) { E += 7; }
        if (asmInfo.elementFlags & (1 << 3)) { E += 7; }
        if (asmInfo.elementFlags & (1 << 4)) { E += 9; }
        if (asmInfo.elementFlags & (1 << 5)) { E += 8; }
        if (asmInfo.elementFlags & (1 << 6)) { E += 9; }
        if (asmInfo.elementFlags & (1 << 7)) { E += 8; }
        if (asmInfo.elementFlags & (1 << 8)) { E += 9; }
        if (asmInfo.elementFlags & (1 << 9)) { E += 13; }
        if (asmInfo.elementFlags & (1 << 10)) { E += 8; }

        if (totalBits < 104 + D + E)
        {
            return;
        }

        // Calculate number of groups n
        uint32_t n = 1 + (totalBits - 104 - D - E) / (29 + E);

        // Helper lambda for signed conversions
        auto ConvertToSigned = [](int64_t rawVal, uint32_t bitsNum) -> int64_t {
            if (rawVal & ((int64_t)1 << (bitsNum - 1)))
            {
                rawVal = rawVal - ((int64_t)1 << bitsNum);
            }
            return rawVal;
        };

        // Decode N1 and E1
        int64_t rawLat1 = manager.DecodeToNumerical(currentBit, 25);
        int64_t signedLat1 = ConvertToSigned(rawLat1, 25);
        double lat1 = 0.0;
        if (rawLat1 != 10800000)
        {
            lat1 = signedLat1 / 60000.0;
        }
        currentBit += 25;

        int64_t rawLon1 = manager.DecodeToNumerical(currentBit, 26);
        int64_t signedLon1 = ConvertToSigned(rawLon1, 26);
        double lon1 = 0.0;
        if (rawLon1 != 21600000)
        {
            lon1 = signedLon1 / 60000.0;
        }
        currentBit += 26;

        for (uint32_t i = 0; i < n; ++i)
        {
            ASM_DAC_412_FI_55::ElementGroup group;
            if (i == 0)
            {
                group.coordinate.SetLatitude(lat1);
                group.coordinate.SetLongitude(lon1);
            }
            else
            {
                int64_t rawDeltaLat = manager.DecodeToNumerical(currentBit, 15);
                int64_t signedDeltaLat = ConvertToSigned(rawDeltaLat, 15);
                currentBit += 15;

                int64_t rawDeltaLon = manager.DecodeToNumerical(currentBit, 14);
                int64_t signedDeltaLon = ConvertToSigned(rawDeltaLon, 14);
                currentBit += 14;

                double latVal = 0.0;
                double lonVal = 0.0;
                if (rawLat1 != 10800000 && rawDeltaLat != -16384)
                {
                    latVal = lat1 + signedDeltaLat / 60000.0;
                }
                if (rawLon1 != 21600000 && rawDeltaLon != -8192)
                {
                    lonVal = lon1 + signedDeltaLon / 60000.0;
                }
                group.coordinate.SetLatitude(latVal);
                group.coordinate.SetLongitude(lonVal);
            }

            // Decode elements of this group
            if (asmInfo.elementFlags & (1 << 0))
            {
                group.currentSpeed = static_cast<uint16_t>(manager.DecodeToNumerical(currentBit, 10));
                currentBit += 10;
            }
            if (asmInfo.elementFlags & (1 << 1))
            {
                group.currentDirection = static_cast<uint16_t>(manager.DecodeToNumerical(currentBit, 9));
                currentBit += 9;
            }
            if (asmInfo.elementFlags & (1 << 2))
            {
                group.windSpeed = static_cast<uint8_t>(manager.DecodeToNumerical(currentBit, 7));
                currentBit += 7;
            }
            if (asmInfo.elementFlags & (1 << 3))
            {
                group.gustWindSpeed = static_cast<uint8_t>(manager.DecodeToNumerical(currentBit, 7));
                currentBit += 7;
            }
            if (asmInfo.elementFlags & (1 << 4))
            {
                group.windDirection = static_cast<uint16_t>(manager.DecodeToNumerical(currentBit, 9));
                currentBit += 9;
            }
            if (asmInfo.elementFlags & (1 << 5))
            {
                group.visibility = static_cast<uint8_t>(manager.DecodeToNumerical(currentBit, 8));
                currentBit += 8;
            }
            if (asmInfo.elementFlags & (1 << 6))
            {
                group.waterLevel = static_cast<uint16_t>(manager.DecodeToNumerical(currentBit, 9));
                currentBit += 9;
            }
            if (asmInfo.elementFlags & (1 << 7))
            {
                group.waveHeight = static_cast<uint8_t>(manager.DecodeToNumerical(currentBit, 8));
                currentBit += 8;
            }
            if (asmInfo.elementFlags & (1 << 8))
            {
                group.waveDirection = static_cast<uint16_t>(manager.DecodeToNumerical(currentBit, 9));
                currentBit += 9;
            }
            if (asmInfo.elementFlags & (1 << 9))
            {
                group.waterDepth = static_cast<uint16_t>(manager.DecodeToNumerical(currentBit, 13));
                currentBit += 13;
            }
            if (asmInfo.elementFlags & (1 << 10))
            {
                group.swellHeight = static_cast<uint8_t>(manager.DecodeToNumerical(currentBit, 8));
                currentBit += 8;
            }

            asmInfo.groups.push_back(group);
        }

        asmInfo.infoSource = static_cast<uint8_t>(manager.DecodeToNumerical(currentBit, 3));

        m_parent->asmNotify(std::make_shared<ASM_DAC_412_FI_55>(asmInfo));
    }

    ASMManager::ASMManager()
    {
        m_impl = std::unique_ptr<Impl>(new Impl(this));
    }

    ASMManager::~ASMManager()
    {
    }

    void ASMManager::Parse(const std::string &sentence)
    {
        auto formatter = sentence.substr(3, 3);

        if ("ADM" == formatter)
        {
            m_impl->ParseADM(sentence);
        }
        else if ("ASM" == formatter)
        {
            m_impl->ParseASM(sentence);
        }
    }

    void ASMManager::ParsePayload(const std::string &payload, const uint8_t fillBitNum)
    {
        if (!payload.empty())
        {
            m_impl->HandlePayload(payload, fillBitNum);
        }
    }

    void ASMManager::ParsePayload(const std::string &payload, const uint8_t fillBitNum, uint32_t srcMMSI, uint32_t destMMSI)
    {
        m_impl->m_mmsiSource = srcMMSI;
        m_impl->m_mmsiDestination = destMMSI;
        if (!payload.empty())
        {
            m_impl->HandlePayload(payload, fillBitNum);
        }
    }
}
