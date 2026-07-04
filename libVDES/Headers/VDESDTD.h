#ifndef VDES_DTD_H_
#define VDES_DTD_H_

#include <string>

namespace VDES
{
    struct BBox
    {
        double left;
        double right;
        double top;
        double bottom;
    };
    
    struct ReferencePoint
    {
        uint16_t posA = 0;
        uint16_t posB = 0;
        uint8_t  posC = 0;
        uint8_t  posD = 0;
    };

    struct ETA
    {
        uint8_t month = 0;
        uint8_t day = 0;
        uint8_t hour = 24;
        uint8_t minute = 60;
    };

    struct OwnVessel
    {
        uint32_t        mmsi = 0;
        uint32_t        imo = 0;
        uint8_t         vesselType = 37;
        uint8_t         naviStatus = 15;
        double          draught = 0.0F;
        std::string     name;
        std::string     callsign;
        std::string     destination;
        ReferencePoint  antennaPosInner;
        ReferencePoint  antennaPosOuter;
        ETA             eta;
        uint16_t        crewNum;
    };

    struct ETHInfo
    {
        uint8_t     ID;
        std::string ip;
        std::string mac;
        std::string mask;
        std::string gateway;
        std::string dns1;
        std::string dns2;
    };

    struct Message
    {
        uint32_t    mmsiSource = 0;
        uint32_t    mmsiDestination = 0;
        std::string content;
        bool        authenticated = false;
        uint64_t    timestampValid = 0;
    };

    struct AtoN
    {
        uint32_t    mmsi = 0;
        double      longitude = 181.0F;
        double      latitude = 91.0F;
        uint8_t     type = 0;
        uint8_t     status = 0;
        bool        isVirtual = false;
        bool        isCancelled = false;
        std::string name;
        uint64_t    timestampValid = 0;
    };
}

#endif