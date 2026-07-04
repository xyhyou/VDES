#ifndef VDES_CONFIGURE_H_
#define VDES_CONFIGURE_H_

#include "VDESDTD.h"
#include "eventpp/callbacklist.h"

#include <string>
#include <memory>
#include <vector>

namespace VDES
{
    class ConfigureManager
    {
    public:
        static ConfigureManager &GetInstance(void);

        void SetStoragePath(const std::string &path);

        std::string GetStoragePath(void) const;

        void EnableVerifyChecksum(const bool enable);

        bool IsEnableVerifyChecksum(void) const;

        /**
        * @brief      Set the station's user id
        * @param[in]  mmsi - station's user id
        *             valid value range:
        *             000000000
        *             [200000000 - 799999999]
        *             [982000000 - 987999999]
        * @param[out] none
        * @return	  return true if success
        */
        bool SetOwnVesselMMSI(const uint32_t mmsi);
    
        /**
        * @brief      Set the station's IMO
        * @param[in]  IMO - [0000000 - 9999999]
        * @param[out] none
        * @return	  return true if success
        */
        bool SetOwnVesselIMO(const uint32_t imo);

        /**
        * @brief      Set the vessel type
        * @param[in]  type - [0 - 255] 
        *             0 = not available or no ship = default
        * @param[out] none
        * @return	  return true if success
        */
        bool SetOwnVesselType(const uint8_t type);

        /*
        * @brief      Set the vessel's navigation status
        * @param[in]  status - [0 - 15]
        *             15 = undefined = default
        * @param[out] none
        * @return	  return true if success
        */
        bool SetOwnVesselNaviStaus(const uint8_t status);

        /**
        * @brief      Set the vessel's draught
        * @param[in]  draught - [0 - 25.5]
        *             0 = not available = default;
        * @param[out] none
        * @return	  return true if success
        */
        bool SetOwnVesselDraught(const double draught);

        /**
        * @brief      Set the call sign
        * @param[in]  callSign  -  maximum 7 characters 6 bit ASCII
        *             "@@@@@@@" = not available = default.
        * @param[out] none
        * @return	  return true if success
        */
        bool SetOwnVesselCallsign(const std::string &callsign);

        /**
        * @brief      Set the staion's name
        * @param[in]  name  -  maximum 20 characters 6 bit ASCII 
        *             "@@@@@@@@@@@@@@@@@@@@" = not available = default.
        * @param[out] none
        * @return	  return true if success
        */
        bool SetOwnVesselName(const std::string &name);

        /**
        * @brief      Set the destination
        * @param[in]  destination  -  maximum 20 characters 6 bit ASCII
        *             "@@@@@@@@@@@@@@@@@@@@" = not available = default.
        * @param[out] none
        * @return	  return true if success
        */
        bool SetOwnVesselDestination(const std::string &destination);

        /**
        * @brief      Set the estimated time of arrival
        * @param[in]  month	 -  1-12, 0 = not available = default
        * @param[in]  day	 -  1-31, 0 = not available = default
        * @param[in]  hour   -  0-23, 24 = not available = default
        * @param[in]  minute -  0-59, 60 = not available = default
        * @param[out] none
        * @return	  return true if success
        */
        bool SetOwnVesselETA(const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t minute);

        enum class RefPointType
        {
            INNER, // Internal antenna location
            OUTER, // External antenna location
        };

        /*
        * @brief	  Set the reference point for reported position
        * @param[in]  posA - range:0 - 511 m
        * @param[in]  posB - range:0 - 511 m
        * @param[in]  posC - range:0 - 63  m
        * @param[in]  posD - range:0 - 63  m
        * @param[out] none
        * @return     return true if success
        * @note       A = B = C = D = 0 , means invalid.
        */
        bool SetOwnVesselPositionReportRefPoint(const RefPointType type, const uint16_t posA,
                                                const uint16_t posB, const uint8_t posC, 
                                                const uint8_t posD);

        /**
        * @brief      Set the crew number
        * @param[in]  crewNum - [0-8191]
        * @param[out] none
        * @return	  return true if success
        */
        bool SetCrewNum(const uint16_t crewNum);

        /*
        * @brief	  Save the own vessel configure
        * @param[in]  none
        * @param[out] none
        * @return     none
        * @note       After invoking this interface, the ship information parameters 
        *             will be set in the VDES terminal
        */
        void SaveOwnVesselConfigure(void);

        /*
        * @brief	  Get the own vessel configure
        * @param[in]  none
        * @param[out] none
        * @return     own vessel configure information
        */
        OwnVessel GetOwnVesselInfo(void) const;

        /*
        * @brief	  Query the own vessel configure
        * @param[in]  none
        * @param[out] none
        * @return     none
        * @note       This interface will send query commands to the VDES terminal.
        */
        void QueryOwnVesselConfigure(void);

        /*
        * @brief	  Set the VDES terminal's ethernet
        * @param[in]  ethID    -   [0 - 1]
        * @param[in]  ip       -   IP address
        * @param[in]  mask     -   IP mask
        * @param[in]  gateway  -   IP gateway
        * @param[in]  dns1     -   DNS1
        * @param[in]  dns2     -   DNS2
        * @param[out] none
        * @return     return true if success
        * @note       The VDES termianl has two ethernet interfaces, eth0 and eth1.
        *             MKD and VDES terminals establish an Ethernet connection for communication.
        */
        bool SetEthernet(const uint8_t ethID, const std::string &ip, const std::string &mask, 
                         const std::string &gateway, const std::string &dns1, 
                         const std::string &dns2);

        /*
        * @brief	  Get the VDES terminal's ethernet configuration
        * @param[in]  none
        * @param[out] none
        * @return     VDES terminal's ethernet information
        */
        std::vector<ETHInfo> GetEthernetInfos(void) const;

        /*
        * @brief	  Query the VDES terminal's ethernet configuration
        * @param[in]  none
        * @param[out] none
        * @return     none
        * @note       This interface will send query commands to the VDES terminal.
        */
        void QueryEthernetConfigure(void);

        /*
        * @brief	  Upgrade the VDES terminal's firmware
        * @param[in]  filePath - firmware file path
        * @param[out] none
        * @return     none
        * @note       MKD and VDES terminals use UDP for upgrades.The port is 999.
        */
        void UpgradeFirmware(const std::string &filePath);

        enum class EventType
        {
            /*
            * Event ret code value:
            * 0 - Query own vessel configure success
            * 1 - Query own vessel configure failed
            * 2 - Save own vessel configure success
            * 3 - Save own vessel configure failed
            */
            OWN_VESSEL_CONFIGURE,

            /*
            * Event ret code value:
            * 0 - Query ethernet configure success
            * 1 - Query ethernet configure failed
            * 2 - Save ethernet configure success
            * 3 - Save ethernet configure failed
            */
            ETH_CONFIGURE,

            /*
            * Event ret code value:
            * -1        - Upgrade failed
            * [0 - 100] - Upgrade Progress
            * 101       - Upgrade success
            */
            UPGRADE_EVENT,
        };

        using NotifyEvent = eventpp::CallbackList<void(const EventType, const int retCode)>;

        NotifyEvent notifyEvent;
    private:
        ConfigureManager();

        ~ConfigureManager();

        ConfigureManager(const ConfigureManager &) = delete;

        ConfigureManager &operator=(const ConfigureManager &) = delete;

    private:
        class Impl;
        using InstancePtr = std::unique_ptr<ConfigureManager>;
        using ImplPtr = std::unique_ptr<Impl>;

        static InstancePtr instance;
        ImplPtr            m_impl;

    };
}

#endif