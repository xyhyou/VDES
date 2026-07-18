#include "VDESConfigure.h"
#include "VDES.h"
#include "md5.h"
#include "spdlog/spdlog.h"
#include "UtilityInterface.h"
#include "TimerManager.h"
#include "ByteArray.h"

#include <cstdlib>
#include <memory>
#include <fstream>

namespace VDES
{
    class ConfigureManager::Impl
    {
    public:
        Impl(ConfigureManager *parent);

        ~Impl();

        void ParseSentence(const std::string &sentence);

        void ParseEPV(const std::string &sentence);

        void ParseSSD(const std::string &sentence);

        void ParseVSD(const std::string &sentence);

        void ParseETH(const std::string &sentence);

        void ParseNTS(const std::string &sentence);

        void ParseVER(const std::string &sentence);

        void BuildSignatureCmd(std::string &cmd);

        void SendCmd(const std::string &cmd);

        void ResetOwnVesselParamsRcvFlags(void);

        void HandleNotifyEvent(void);

        void HandleFirmwareUpgradeResponse(const std::string &response);

        bool IsDoubleValueEqual(double a, double b, double absError, double relError) const;

        bool IsOwnVesselConfigureEqual(const OwnVessel &ownVessel1,
            const OwnVessel &ownVessel2) const;

    public:
        using RcvFlags = std::map<std::string, bool>;
        using ETHS = std::map<uint8_t, ETHInfo>;
        using PacketDatas = std::unique_ptr<uint8_t>;

        struct FirmwareInfo
        {
            uint16_t     totalNum = 0;
            uint32_t     packetSizeLast = 0;
            std::string  md5;
            PacketDatas  datas;
            uint32_t     fileSize = 0;
            uint32_t     transmitSize = 1400;
        };

        bool                m_enalbeVerifyChecksum;
        std::string         m_storagePath;
        uint32_t            m_mmsiBaseStation;
        OwnVessel           m_ownVessel;
        OwnVessel           m_ownVesselSetted;
        RcvFlags            m_rcvFlags;
        bool                m_queryOwnVessel;
        bool                m_settingOwnVessel;
        ConfigureManager    *m_parent;
        uint32_t            m_timerOwnVessel;
        bool                m_queryETH;
        bool                m_settingETH;
        ETHS                m_ethInfo;
        ETHS                m_ethInfoSetted;
        uint32_t            m_timerETH;
        VersionInfo         m_versionInfo;
        uint32_t            m_timerVersion;
        FirmwareInfo        m_firmwareInfo;
        uint32_t            m_inboxSize;
        uint32_t            m_outboxSize;
        uint32_t            m_timerUpgrade;
    };

    ConfigureManager::Impl::Impl(ConfigureManager *parent)
    {
        m_parent = parent;
        m_enalbeVerifyChecksum = true;
        m_queryOwnVessel = false;
        m_settingOwnVessel = false;
        m_timerOwnVessel = 0;

        m_queryETH = false;
        m_settingETH = false;
        m_timerETH = 0;

        m_timerVersion = 0;
        m_storagePath = ".";

        m_timerUpgrade = 0;
    }

    ConfigureManager::Impl::~Impl()
    {
        auto &timerManager = TimerManager::GetInstance();

        timerManager.RemoveTimer(m_timerOwnVessel);
        timerManager.RemoveTimer(m_timerUpgrade);
        timerManager.RemoveTimer(m_timerETH);
        timerManager.RemoveTimer(m_timerVersion);
    }

    void ConfigureManager::Impl::ParseSentence(const std::string &sentence)
    {
        auto formatter = sentence.substr(3, 3);

        if (formatter == "EPV")
        {
            ParseEPV(sentence);
        }
        else if (formatter == "SSD")
        {
            ParseSSD(sentence);
        }
        else if (formatter == "VSD")
        {
            ParseVSD(sentence);
        }
        else if (formatter == "ETH")
        {
            ParseETH(sentence);
        }
        else if (formatter == "NTS")
        {
            ParseNTS(sentence);
        }
        else if (formatter == "VER")
        {
            ParseVER(sentence);
        }
        else if (formatter == "ST1" || formatter == "ST2" ||
            formatter == "ST3" || formatter == "ST4")
        {
            HandleFirmwareUpgradeResponse(sentence);
        }
        HandleNotifyEvent();
    }

    void ConfigureManager::Impl::ParseEPV(const std::string &sentence)
    {
        auto strList = UtilityInterface::SplitString(sentence, ",");
        if (6 == strList.size())
        {
            if (!strList.at(4).empty())
            {
                auto identifier = atoi(strList.at(4).c_str());
                switch (identifier)
                {
                case 106: // MMSI
                    if (!strList.at(5).empty())
                    {
                        m_ownVessel.mmsi = atoi(strList.at(5).c_str());
                        m_rcvFlags["MMSI"] = true;
                    }
                    break;
                case 107: // IMO
                    if (!strList.at(5).empty())
                    {
                        m_ownVessel.imo = atoi(strList.at(5).c_str());
                        m_rcvFlags["IMO"] = true;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    void ConfigureManager::Impl::ParseSSD(const std::string &sentence)
    {
        auto strList = UtilityInterface::SplitString(sentence, ",");
        if (9 == strList.size())
        {
            m_ownVessel.callsign = strList.at(1);
            if (m_ownVessel.callsign == "@@@@@@@")
            {
                m_ownVessel.callsign.clear();
            }
            m_ownVessel.name = strList.at(2);
            auto strListTemp = UtilityInterface::SplitString(strList.at(8), "*");
            if (2 == strListTemp.size())
            {
                ReferencePoint *point = nullptr;

                if ("AI" == strListTemp.at(0))
                {
                    point = &m_ownVessel.antennaPosInner;
                    m_rcvFlags["SSD_INNER"] = true;
                }
                else
                {
                    point = &m_ownVessel.antennaPosOuter;
                    m_rcvFlags["SSD_OUTER"] = true;
                }
                if (point)
                {
                    if (!strList.at(3).empty())
                    {
                        point->posA = atoi(strList.at(3).c_str());
                    }
                    if (!strList.at(4).empty())
                    {
                        point->posB = atoi(strList.at(4).c_str());
                    }
                    if (!strList.at(5).empty())
                    {
                        point->posC = atoi(strList.at(5).c_str());
                    }
                    if (!strList.at(6).empty())
                    {
                        point->posD = atoi(strList.at(6).c_str());
                    }
                }
            }
        }
    }

    void ConfigureManager::Impl::ParseVSD(const std::string &sentence)
    {
        auto strList = UtilityInterface::SplitString(sentence, ",");
        if (10 == strList.size())
        {
            if (!strList.at(1).empty())
            {
                m_ownVessel.vesselType = atoi(strList.at(1).c_str());
            }
            if (!strList.at(2).empty())
            {
                m_ownVessel.draught = atof(strList.at(2).c_str());
            }
            if (!strList.at(3).empty())
            {
                m_ownVessel.crewNum = atoi(strList.at(3).c_str());
            }
            m_ownVessel.destination = strList.at(4);
            if (m_ownVessel.destination == "@@@@@@@@@@@@@@@@@@@@")
            {
                m_ownVessel.destination.clear();
            }
            
            if (!strList.at(5).empty())
            {
                int hour, minute;

                auto count = sscanf(strList.at(5).c_str(), "%02d%02d", &hour, &minute);
                if (2 == count)
                {
                    m_ownVessel.eta.hour = static_cast<uint8_t>(hour);
                    m_ownVessel.eta.minute = static_cast<uint8_t>(minute);
                }
            }

            if (!strList.at(6).empty())
            {
                m_ownVessel.eta.day = atoi(strList.at(6).c_str());
            }
            if (!strList.at(7).empty())
            {
                m_ownVessel.eta.month = atoi(strList.at(7).c_str());
            }
            if (!strList.at(8).empty())
            {
                m_ownVessel.naviStatus = atoi(strList.at(8).c_str());
            }
            m_rcvFlags["VSD"] = true;
        }
    }

    void ConfigureManager::Impl::ParseETH(const std::string &sentence)
    {
        auto strList = UtilityInterface::SplitString(sentence, ",");
        if (8 == strList.size())
        {
            if (!strList.at(1).empty())
            {
                auto ethID = atoi(strList.at(1).c_str());
                if (ethID <= 1)
                {
                    auto iter = m_ethInfo.find(ethID);
                    if (iter == m_ethInfo.end())
                    {
                        m_ethInfo[ethID] = ETHInfo();
                    }
                    auto &ethConfigure = m_ethInfo[ethID];

                    ethConfigure.ID = ethID;
                    ethConfigure.mac = strList.at(2);
                    ethConfigure.ip = strList.at(3);
                    ethConfigure.mask = strList.at(4);
                    ethConfigure.gateway = strList.at(5);
                    ethConfigure.dns1 = strList.at(6);
                    auto strListTemp = UtilityInterface::SplitString(strList.at(7), "*");
                    if (2 == strListTemp.size())
                    {
                        ethConfigure.dns2 = strListTemp.at(0);
                    }
                }
                if (ethID == 1)
                {
                    TimerManager::GetInstance().RemoveTimer(m_timerETH);

                    if (m_queryETH)
                    {
                        m_queryETH = false;
                        m_parent->notifyEvent(EventType::ETH_CONFIGURE, 0);
                    }

                    if (!m_ethInfoSetted.empty())
                    {
                        for (auto &kv : m_ethInfoSetted)
                        {
                            auto &ethSet = kv.second;
                            auto &ethCurrent = m_ethInfo[kv.first];
                            if (ethSet.ip != ethCurrent.ip ||
                                ethSet.mask != ethCurrent.mask ||
                                ethSet.gateway != ethCurrent.gateway ||
                                ethSet.portNumber != ethCurrent.portNumber)
                            {
                                m_parent->notifyEvent(EventType::ETH_CONFIGURE, 3);
                                m_ethInfoSetted.clear();
                                return;
                            }
                        }
                        m_ethInfoSetted.clear();
                        m_parent->notifyEvent(EventType::ETH_CONFIGURE, 2);
                    }
                }
            }
        }
    }

    void ConfigureManager::Impl::ParseNTS(const std::string &sentence)
    {
        auto strList = UtilityInterface::SplitString(sentence, ",");
        if (5 == strList.size())
        {
            if (!strList.at(1).empty())
            {
                auto ethID = atoi(strList.at(1).c_str());
                if (ethID <= 1)
                {
                    auto strListTemp = UtilityInterface::SplitString(strList.at(4), "*");
                    if (2 == strListTemp.size())
                    {
                        auto iter = m_ethInfo.find(ethID);
                        if (iter == m_ethInfo.end())
                        {
                            m_ethInfo[ethID] = ETHInfo();
                        }
                        auto &ethConfigure = m_ethInfo[ethID];
                        ethConfigure.portNumber = atoi(strListTemp.at(0).c_str());
                        if (m_ethInfoSetted.find(ethID) != m_ethInfoSetted.end())
                        {
                            m_ethInfoSetted[ethID].portNumber = ethConfigure.portNumber;
                        }
                    }
                }
            }
        }
    }

    void ConfigureManager::Impl::ParseVER(const std::string &sentence)
    {
        auto strList = UtilityInterface::SplitString(sentence, ",");
        if (11 == strList.size())
        {
            auto totalNumber = atoi(strList.at(1).c_str());
            auto sentenceNumber = atoi(strList.at(2).c_str());

            TimerManager::GetInstance().RemoveTimer(m_timerVersion);

            if (sentenceNumber == 1)
            {
                m_versionInfo.deviceType.clear();
                m_versionInfo.hardwareRevision.clear();
                m_versionInfo.productCode.clear();
                m_versionInfo.serialNumber.clear();
                m_versionInfo.softwareRevision.clear();
                m_versionInfo.vendorID.clear();
            }

            if (!strList.at(3).empty())
            {
                m_versionInfo.deviceType = strList.at(3);
            }

            if (!strList.at(4).empty())
            {
                m_versionInfo.vendorID = strList.at(4);
            }

            if (!strList.at(6).empty())
            {
                m_versionInfo.serialNumber = strList.at(6);
            }

            if (!strList.at(7).empty())
            {
                m_versionInfo.productCode = strList.at(7);
            }

            if (!strList.at(8).empty())
            {
                m_versionInfo.softwareRevision = strList.at(8);
            }

            if (!strList.at(9).empty())
            {
                m_versionInfo.hardwareRevision = strList.at(9);
            }

            if (totalNumber == sentenceNumber)
            {
                m_parent->notifyEvent(EventType::VERSION_INFO, 0);
            }
        }
    }

    void ConfigureManager::Impl::BuildSignatureCmd(std::string &cmd)
    {
        auto formatter = cmd.substr(3, 3);

        auto strTemp = "AOHAI" + cmd;
        auto digest = UtilityInterface::CalculateMD5Digest(strTemp);
        auto strSSA = fmt::format("$XNSSA,{0},1,{1}", formatter, digest);

        SendCmd(strSSA);
        SendCmd(cmd);
    }

    void ConfigureManager::Impl::SendCmd(const std::string &cmd)
    {
        auto strCmdTemp = cmd;
        UtilityInterface::AddChecksum(strCmdTemp);
        SPDLOG_DEBUG("SendCmd: {}", strCmdTemp);
        VDESManager::GetInstance().sendEvent(CommunicationType::TCP, strCmdTemp.c_str(), strCmdTemp.size());
    }

    void ConfigureManager::Impl::ResetOwnVesselParamsRcvFlags(void)
    {
        if (m_rcvFlags.empty())
        {
            m_rcvFlags.insert(std::make_pair("MMSI", false));
            m_rcvFlags.insert(std::make_pair("IMO", false));
            m_rcvFlags.insert(std::make_pair("SSD_INNER", false));
            m_rcvFlags.insert(std::make_pair("SSD_OUTER", false));
            m_rcvFlags.insert(std::make_pair("VSD", false));
        }
        else
        {
            m_rcvFlags["MMSI"] = false;
            m_rcvFlags["IMO"] = false;
            m_rcvFlags["SSD_INNER"] = false;
            m_rcvFlags["SSD_OUTER"] = false;
            m_rcvFlags["VSD"] = false;
        }
    }

    void ConfigureManager::Impl::HandleNotifyEvent(void)
    {
        if (m_queryOwnVessel || m_settingOwnVessel)
        {
            if (!m_rcvFlags["MMSI"] || !m_rcvFlags["IMO"] ||
                !m_rcvFlags["SSD_INNER"] || !m_rcvFlags["SSD_OUTER"] ||
                !m_rcvFlags["VSD"])
            {
                return;
            }

            TimerManager::GetInstance().RemoveTimer(m_timerOwnVessel);

            if (m_queryOwnVessel)
            {
                m_parent->notifyEvent(EventType::OWN_VESSEL_CONFIGURE, 0);
                m_queryOwnVessel = false;
            }
            if (m_settingOwnVessel)
            {
                if (IsOwnVesselConfigureEqual(m_ownVessel, m_ownVesselSetted))
                {
                    m_parent->notifyEvent(EventType::OWN_VESSEL_CONFIGURE, 2);
                }
                else
                {
                    m_parent->notifyEvent(EventType::OWN_VESSEL_CONFIGURE, 3);
                }
                m_settingOwnVessel = false;
            }
            m_ownVesselSetted = m_ownVessel;
            ResetOwnVesselParamsRcvFlags();
        }
    }

    void ConfigureManager::Impl::HandleFirmwareUpgradeResponse(const std::string &response)
    {
        static int retCode = -100;
        auto       &VDESManager = VDESManager::GetInstance();

        auto SendPacketData = [&](const uint32_t packetNo) {
            ByteArray cmdDatas;

            cmdDatas.Append("$AHST3");
            cmdDatas.Append('\0', 1);

            auto strTemp = fmt::format("{}", packetNo);
            cmdDatas.Append(strTemp);
            cmdDatas.Append('\0', 11 - strTemp.length());

            auto index = (packetNo - 1) * m_firmwareInfo.transmitSize;
            auto packetSize = (packetNo == m_firmwareInfo.totalNum) ? 
                m_firmwareInfo.packetSizeLast : m_firmwareInfo.transmitSize;
            auto packetData = m_firmwareInfo.datas.get() + index;
            auto md5 = UtilityInterface::CalculateMD5Digest(packetData, packetSize);
            cmdDatas.Append(md5);
            cmdDatas.Append('\0', 1);
            cmdDatas.Append((const char *)packetData, packetSize);
            VDESManager.sendEvent(CommunicationType::UDP, (char *)cmdDatas.Data(), cmdDatas.Size());
        };

        if (TimerManager::IsTimerValid(m_timerUpgrade))
        {
            TimerManager::GetInstance().ResetTimer(m_timerUpgrade);
        }

        if (response.find("$AHST1,ACK,O") != std::string::npos)
        {
            ByteArray cmdDatas;

            cmdDatas.Append("$AHST2");
            cmdDatas.Append('\0', 1);

            auto strTemp = fmt::format("{}", m_firmwareInfo.totalNum);
            cmdDatas.Append(strTemp);
            cmdDatas.Append('\0', 11 - strTemp.length());

            cmdDatas.Append("1");
            cmdDatas.Append('\0', 10);

            cmdDatas.Append("1400");
            cmdDatas.Append('\0', 7);

            strTemp = fmt::format("{}", m_firmwareInfo.packetSizeLast);
            cmdDatas.Append(strTemp);
            cmdDatas.Append('\0', 11 - strTemp.length());

            VDESManager.sendEvent(CommunicationType::UDP, (char *)cmdDatas.Data(), cmdDatas.Size());
        }
        else if (response.find("$AHST2,ACK,O") != std::string::npos)
        {
            SendPacketData(1);
        }
        else if (response.find("$AHST3,ACK,") != std::string::npos)
        {
            auto strList = UtilityInterface::SplitString(response, ",");
            if (3 == strList.size())
            {
                auto packetNo = atoi(strList.at(2).c_str());
                if (packetNo > 0)
                {
                    if (packetNo < m_firmwareInfo.totalNum)
                    {
                        SendPacketData(packetNo + 1);
                        auto progress = (packetNo + 1) * 100 / m_firmwareInfo.totalNum;
                        if (retCode != progress)
                        {
                            retCode = progress;
                            m_parent->notifyEvent(EventType::UPGRADE_FIRMWARE, progress);
                        }
                    }
                    else
                    {
                        ByteArray cmdDatas;

                        cmdDatas.Append("$AHST4");
                        cmdDatas.Append('\0', 1);
                        cmdDatas.Append(m_firmwareInfo.md5);
                        cmdDatas.Append('\0', 100);
                        VDESManager.sendEvent(CommunicationType::UDP, (char *)cmdDatas.Data(),
                                              cmdDatas.Size());
                        if (TimerManager::IsTimerValid(m_timerUpgrade))
                        {
                            TimerManager::GetInstance().ResetTimer(m_timerUpgrade, 180 * 1000);
                        }
                        if (retCode != 101)
                        {
                            retCode = 101;
                            m_parent->notifyEvent(EventType::UPGRADE_FIRMWARE, 101);
                        }
                    }
                }
                else
                {
                    if (retCode != -1)
                    {
                        retCode = -1;
                        TimerManager::GetInstance().RemoveTimer(m_timerUpgrade);
                        m_parent->notifyEvent(EventType::UPGRADE_FIRMWARE, -1);
                    }
                } 
            }
        }
        else if (response.find("$AHST4,ACK,O") != std::string::npos)
        {
            if (retCode != 102)
            {
                retCode = 102;
                m_parent->notifyEvent(EventType::UPGRADE_FIRMWARE, 102);
                // Reset the VDES terminal
                auto strCmd = fmt::format("$XNRST,{:09d},1,1,C", m_ownVessel.mmsi);
                VDESManager.sendEvent(CommunicationType::UDP, strCmd.c_str(), strCmd.length());  
            } 
            TimerManager::GetInstance().RemoveTimer(m_timerUpgrade);
        }
    }

    bool ConfigureManager::Impl::IsDoubleValueEqual(double a, double b, double absError, 
        double relError) const
    {
        if (a == b)
        {
            return true;
        }
        if (fabs(a - b) < absError)
        {
            return true;
        }
        if (fabs(a) < fabs(b))
        {
            return (fabs((a - b) / b) < relError) ? true : false;
        }
        else
        {
            return (fabs((a - b) / a) < relError) ? true : false;
        }

        return false;
    }

    bool ConfigureManager::Impl::IsOwnVesselConfigureEqual(const OwnVessel &ownVessel1, 
        const OwnVessel &ownVessel2) const
    {
        if (ownVessel1.antennaPosInner.posA == ownVessel2.antennaPosInner.posA &&
            ownVessel1.antennaPosInner.posB == ownVessel2.antennaPosInner.posB &&
            ownVessel1.antennaPosInner.posC == ownVessel2.antennaPosInner.posC &&
            ownVessel1.antennaPosOuter.posA == ownVessel2.antennaPosOuter.posA &&
            ownVessel1.antennaPosOuter.posB == ownVessel2.antennaPosOuter.posB &&
            ownVessel1.antennaPosOuter.posC == ownVessel2.antennaPosOuter.posC &&
            ownVessel1.antennaPosOuter.posD == ownVessel2.antennaPosOuter.posD &&
            ownVessel1.callsign == ownVessel2.callsign &&
            ownVessel1.crewNum == ownVessel2.crewNum &&
            ownVessel1.destination == ownVessel2.destination &&
            IsDoubleValueEqual(ownVessel1.draught, ownVessel2.draught, 0.00001, 0.0001) &&
            ownVessel1.eta.day == ownVessel2.eta.day &&
            ownVessel1.eta.month == ownVessel2.eta.month &&
            ownVessel1.eta.hour == ownVessel2.eta.hour &&
            ownVessel1.eta.minute == ownVessel2.eta.minute &&
            ownVessel1.imo == ownVessel2.imo &&
            ownVessel1.mmsi == ownVessel2.mmsi &&
            ownVessel1.name == ownVessel2.name &&
            ownVessel1.naviStatus == ownVessel2.naviStatus &&
            ownVessel1.vesselType == ownVessel2.vesselType)
        {
            return true;
        }
        return false;
    }

    ConfigureManager &ConfigureManager::GetInstance(void)
    {
        static ConfigureManager manager;
        return manager;
    }
    void ConfigureManager::SetStoragePath(const std::string &path)
    {
        m_impl->m_storagePath = path;
    }

    std::string ConfigureManager::GetStoragePath(void) const
    {
        return m_impl->m_storagePath;
    }

    void ConfigureManager::SetBaseStationMMSI(uint32_t mmsi)
    {
        m_impl->m_mmsiBaseStation = mmsi;
    }

    uint32_t ConfigureManager::GetBaseStationMMSI(void) const
    {
        return m_impl->m_mmsiBaseStation;
    }

    void ConfigureManager::SetMailBoxCapacity(const uint32_t inboxSize, const uint32_t outboxSize)
    {
        if (inboxSize > 0)
        {
            m_impl->m_inboxSize = inboxSize;
        }
        if (outboxSize > 0)
        {
            m_impl->m_outboxSize = outboxSize;
        }
    }

    std::tuple<uint32_t, uint32_t> ConfigureManager::GetMailBoxCapacity(void) const
    {
        return std::make_tuple(m_impl->m_inboxSize, m_impl->m_outboxSize);
    }

    void ConfigureManager::EnableVerifyChecksum(const bool enable)
    {
        m_impl->m_enalbeVerifyChecksum = enable;
    }

    bool ConfigureManager::IsEnableVerifyChecksum(void) const
    {
        return m_impl->m_enalbeVerifyChecksum;
    }

    bool ConfigureManager::SetOwnVesselMMSI(const uint32_t mmsi)
    {
        if ((0 == mmsi) ||
            (mmsi >= 200000000 && mmsi <= 799999999) ||
            (mmsi >= 982000000 && mmsi <= 987999999))
        {
            m_impl->m_ownVesselSetted.mmsi = mmsi;
            return true;
        }
        return false;
    }

    bool ConfigureManager::SetOwnVesselIMO(const uint32_t imo)
    {
        if (imo <= 9999999)
        {
            m_impl->m_ownVesselSetted.imo = imo;
            return true;
        }
        return false;
    }

    bool ConfigureManager::SetOwnVesselType(const uint8_t type)
    {
        m_impl->m_ownVesselSetted.vesselType = type;
        return true;
    }

    bool ConfigureManager::SetOwnVesselNaviStaus(const uint8_t status)
    {
        if (status <= 15)
        {
            m_impl->m_ownVesselSetted.naviStatus = status;
            return true;
        }
        return false;
    }

    bool ConfigureManager::SetOwnVesselDraught(const double draught)
    {
        if (draught <= 25.5)
        {
            m_impl->m_ownVesselSetted.draught = draught;
            return true;
        }
        return false;
    }

    bool ConfigureManager::SetOwnVesselCallsign(const std::string &callsign)
    {
        if (callsign.length() <= 7)
        {
            m_impl->m_ownVesselSetted.callsign = callsign;
            return true;
        }
        return false;
    }

    bool ConfigureManager::SetOwnVesselName(const std::string &name)
    {
        if (name.length() <= 20)
        {
            m_impl->m_ownVesselSetted.name = name;
            return true;
        }
        return false;
    }

    bool ConfigureManager::SetOwnVesselDestination(const std::string &destination)
    {
        if (destination.length() <= 20)
        {
            SPDLOG_DEBUG("SetOwnVesselDestination: {}", destination);
            m_impl->m_ownVesselSetted.destination = destination;
            return true;
        }
        return false;
    }

    bool ConfigureManager::SetOwnVesselETA(const uint8_t month, const uint8_t day, 
        const uint8_t hour, const uint8_t minute)
    {
        if (month <= 12 && day <= 31 && hour <= 24 && minute <= 60)
        {
            m_impl->m_ownVesselSetted.eta.month = month;
            m_impl->m_ownVesselSetted.eta.day = day;
            m_impl->m_ownVesselSetted.eta.hour = hour;
            m_impl->m_ownVesselSetted.eta.minute = minute;
            return true;
        }
        return false;
    }

    bool ConfigureManager::SetOwnVesselPositionReportRefPoint(const RefPointType type, 
        const uint16_t posA, const uint16_t posB, const uint8_t posC, const uint8_t posD)
    {
        if (posA <= 511 && posB <= 511 && posC <= 63 && posD <= 63)
        {
            if (RefPointType::INNER == type)
            {
                m_impl->m_ownVesselSetted.antennaPosInner.posA = posA;
                m_impl->m_ownVesselSetted.antennaPosInner.posB = posB;
                m_impl->m_ownVesselSetted.antennaPosInner.posC = posC;
                m_impl->m_ownVesselSetted.antennaPosInner.posD = posD;
            }
            else
            {
                m_impl->m_ownVesselSetted.antennaPosOuter.posA = posA;
                m_impl->m_ownVesselSetted.antennaPosOuter.posB = posB;
                m_impl->m_ownVesselSetted.antennaPosOuter.posC = posC;
                m_impl->m_ownVesselSetted.antennaPosOuter.posD = posD;
            }
            return true;
        }
        return false;
    }

    bool ConfigureManager::SetCrewNum(const uint16_t crewNum)
    {
        if (crewNum <= 8191)
        {
            SPDLOG_DEBUG("SetCrewNum: {}", crewNum);
            m_impl->m_ownVesselSetted.crewNum = crewNum;
            return true;
        }
        return false;
    }

    void ConfigureManager::SaveOwnVesselConfigure(void)
    {
        if (!m_impl->m_queryOwnVessel && !m_impl->m_settingOwnVessel)
        {
            m_impl->m_settingOwnVessel = true;
            m_impl->ResetOwnVesselParamsRcvFlags();

            // Configure MMSI
            auto strEPV = fmt::format("$XNEPV,C,AI,{0:09d},106,{1:09d}",
                m_impl->m_ownVessel.mmsi, m_impl->m_ownVesselSetted.mmsi);
            m_impl->BuildSignatureCmd(strEPV);

            // Configure IMO
            strEPV = fmt::format("$XNEPV,C,AI,{0:09d},107,{1:07d}",
                m_impl->m_ownVessel.mmsi, m_impl->m_ownVesselSetted.imo);
            m_impl->BuildSignatureCmd(strEPV);

            // Configure ship static data
            auto strSSD = fmt::format("$XNSSD,{0},{1},{2},{3},{4},{5},0,AI",
                m_impl->m_ownVesselSetted.callsign,
                m_impl->m_ownVesselSetted.name,
                m_impl->m_ownVesselSetted.antennaPosInner.posA,
                m_impl->m_ownVesselSetted.antennaPosInner.posB,
                m_impl->m_ownVesselSetted.antennaPosInner.posC,
                m_impl->m_ownVesselSetted.antennaPosInner.posD);
            m_impl->BuildSignatureCmd(strSSD);

            strSSD = fmt::format("$XNSSD,{0},{1},{2},{3},{4},{5},0,HE",
                m_impl->m_ownVesselSetted.callsign,
                m_impl->m_ownVesselSetted.name,
                m_impl->m_ownVesselSetted.antennaPosOuter.posA,
                m_impl->m_ownVesselSetted.antennaPosOuter.posB,
                m_impl->m_ownVesselSetted.antennaPosOuter.posC,
                m_impl->m_ownVesselSetted.antennaPosOuter.posD);
            m_impl->BuildSignatureCmd(strSSD);

            // Configure AIS voyage static data
            auto strVSD = fmt::format("$XNVSD,{0},{1:.1f},{2},{3},{4:02d}{5:02d}00.00,{6:02d},{7:02d},{8},0",
                m_impl->m_ownVesselSetted.vesselType,
                m_impl->m_ownVesselSetted.draught,
                m_impl->m_ownVesselSetted.crewNum,
                m_impl->m_ownVesselSetted.destination,
                m_impl->m_ownVesselSetted.eta.hour,
                m_impl->m_ownVesselSetted.eta.minute,
                m_impl->m_ownVesselSetted.eta.day,
                m_impl->m_ownVesselSetted.eta.month,
                m_impl->m_ownVesselSetted.naviStatus);
            m_impl->BuildSignatureCmd(strVSD);

            m_impl->SendCmd("$XNAIQ,EPV");
            m_impl->SendCmd("$XNAIQ,SSD");
            m_impl->SendCmd("$XNAIQ,VSD");

            if (!TimerManager::IsTimerValid(m_impl->m_timerOwnVessel))
            {
                m_impl->m_timerOwnVessel = TimerManager::GetInstance().AddTimer(3000, false, [&] {
                    m_impl->m_settingOwnVessel = false;
                    m_impl->m_timerOwnVessel = 0;
                    notifyEvent(EventType::OWN_VESSEL_CONFIGURE, 3);
                });
            }
        } 
    }

    OwnVessel ConfigureManager::GetOwnVesselInfo(void) const
    {
        return m_impl->m_ownVessel;
    }

    void ConfigureManager::QueryOwnVesselConfigure(void)
    {
        if (!m_impl->m_queryOwnVessel && !m_impl->m_settingOwnVessel)
        {
            m_impl->SendCmd("$XNAIQ,EPV");
            m_impl->SendCmd("$XNAIQ,SSD");
            m_impl->SendCmd("$XNAIQ,VSD");

            m_impl->m_queryOwnVessel = true;
            m_impl->ResetOwnVesselParamsRcvFlags();

            if (!TimerManager::IsTimerValid(m_impl->m_timerOwnVessel))
            {
                m_impl->m_timerOwnVessel = TimerManager::GetInstance().AddTimer(3000, false, [&] {
                    m_impl->m_queryOwnVessel = false;
                    m_impl->m_timerOwnVessel = 0;
                    notifyEvent(EventType::OWN_VESSEL_CONFIGURE, 1);
                });
            }
        }
    }

    bool ConfigureManager::SetEthernet(const uint8_t ethID, const std::string &ip, 
        const std::string &mask, const std::string &gateway, const uint32_t portNumber)
    {
        if (ethID <= 1)
        {
            ETHInfo ethConfigure;

            ethConfigure.ID = ethID;
            ethConfigure.ip = ip;
            ethConfigure.mask = mask;
            ethConfigure.gateway = gateway;
            ethConfigure.portNumber = portNumber;
           
            m_impl->m_ethInfoSetted[ethID] = ethConfigure;

            auto strETH = fmt::format("$XNETH,{0},{1},{2},{3},{4},{5},{6}",
                ethID, ethConfigure.mac, ethConfigure.ip, ethConfigure.mask, ethConfigure.gateway, 
                ethConfigure.dns1, ethConfigure.dns2);
            m_impl->SendCmd(strETH);

            auto strNTS = fmt::format("$XNNTS,{0},0,1,{1}", ethID, portNumber);
            m_impl->SendCmd(strNTS);

            m_impl->SendCmd("$XNAIQ,NTS");
            m_impl->SendCmd("$XNAIQ,ETH");
            m_impl->m_settingETH = true;
            return true;
        }
        return false;
    }

    std::vector<ETHInfo> ConfigureManager::GetEthernetInfos(void) const
    {
        std::vector<ETHInfo> ethInfos;

        for (auto &kv : m_impl->m_ethInfo)
        {
            ethInfos.push_back(kv.second);
        }
        return ethInfos;
    }

    void ConfigureManager::QueryEthernetConfigure(void)
    {
        if (!m_impl->m_queryETH)
        {
            m_impl->SendCmd("$XNAIQ,NTS");
            m_impl->SendCmd("$XNAIQ,ETH");
            m_impl->m_queryETH = true;

            if (!TimerManager::IsTimerValid(m_impl->m_timerETH))
            {
                m_impl->m_timerETH = TimerManager::GetInstance().AddTimer(3000, false, [&] {
                    m_impl->m_queryETH = false;
                    m_impl->m_timerETH = 0;
                    notifyEvent(EventType::ETH_CONFIGURE, 1);
                });
            }
        }
    }

    VersionInfo ConfigureManager::GetVersionInfo(void) const
    {
        return m_impl->m_versionInfo;
    }

    void ConfigureManager::QueryVersionInfo(void)
    {
        m_impl->SendCmd("$XNAIQ,VER");
        if (!TimerManager::IsTimerValid(m_impl->m_timerVersion))
        {
            m_impl->m_timerVersion = TimerManager::GetInstance().AddTimer(3000, false, [&] {
                notifyEvent(EventType::VERSION_INFO, 1);
            });
        }
    }

    void ConfigureManager::UpgradeFirmware(const std::string &filePath)
    {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);

        if (file.is_open())
        {
            file.seekg(0, std::ios::end);
            auto size = static_cast<int>(file.tellg());
            file.clear();
            file.seekg(0, std::ios::beg);

            if (size > 0)
            {
                m_impl->m_firmwareInfo.transmitSize = 1400;
                m_impl->m_firmwareInfo.datas.reset();
                m_impl->m_firmwareInfo.datas = std::unique_ptr<uint8_t>(new uint8_t[size]);
                file.read((char *)(m_impl->m_firmwareInfo.datas.get()), size);
                auto &blockSize = m_impl->m_firmwareInfo.transmitSize;
                auto blockSizeLast = size % blockSize;
                m_impl->m_firmwareInfo.totalNum = (size / blockSize) + ((blockSizeLast == 0) ? 0 : 1);
                m_impl->m_firmwareInfo.packetSizeLast = blockSizeLast;
                m_impl->m_firmwareInfo.md5 = UtilityInterface::CalculateMD5Digest(m_impl->m_firmwareInfo.datas.get(), size);
            }
            file.close();
        }

        // Build the upgrade command
        ByteArray cmdDatas;

        cmdDatas.Append("$AHST1");
        cmdDatas.Append('\0', 1);
        cmdDatas.Append("AhTec");
        cmdDatas.Append('\0', 6);
        cmdDatas.Append("666");
        cmdDatas.Append('\0', 8);

        VDES::VDESManager::GetInstance().sendEvent(CommunicationType::UDP, 
            (const char *)cmdDatas.Data(), cmdDatas.Size());

        if (!TimerManager::IsTimerValid(m_impl->m_timerUpgrade))
        {
            auto &timerManager = TimerManager::GetInstance();
            m_impl->m_timerUpgrade = timerManager.AddTimer(10000, false, [&] {
                m_impl->m_timerUpgrade = 0;
                notifyEvent(EventType::UPGRADE_FIRMWARE, -2);
            });
        }
    }

    ConfigureManager::ConfigureManager()
    {
        m_impl = std::unique_ptr<Impl>(new Impl(this));
        m_impl->ResetOwnVesselParamsRcvFlags();
        
        auto &manager = VDESManager::GetInstance();

        auto func = std::bind(&ConfigureManager::Impl::ParseSentence, m_impl.get(), std::placeholders::_1);
        manager.m_receiveEvent.append(func);
    }

    ConfigureManager::~ConfigureManager()
    {

    }
}
