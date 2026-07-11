#ifndef ASM_DTD_H_
#define ASM_DTD_H_

#include <vector>
#include <string>

#include "Coordinate.h"

namespace VDES
{
    struct ASM_DAC_FI
    {

        virtual ~ASM_DAC_FI() {};

        uint32_t source = 0;

        uint32_t destination = 0;

        uint8_t  channel = 0;

        uint16_t DAC = 0;

        uint8_t  FI = 0;
    };

    /**
     * @brief : Marine Meteorological Forecast (Coordinate)
     */
    struct ASM_DAC412_FI26 : ASM_DAC_FI
    {
        struct LocationInfo
        {
            Coordinate coordinate;

            uint8_t    weatherCode = 0;

            /**
             * valid range : [0 - 120]
             * unit : kn/h
             */
            uint8_t    windSpeed = 0;

            /**
             * valid range : [0 - 360]
             * unit : degree
             */
            uint16_t   windDirection = 360;

            /**
             * temperature: raw 11-bit complement integer.
             * Resolution: 0.1 degree C.
             */
            int16_t   temperature = 602;

            /**
             * airPressure: raw 10-bit integer.
             */
            uint16_t  airPressure = 803;

            /**
             * visibility: raw 8-bit integer.
             */
            uint8_t   visibility = 252;
        };

        uint8_t hourPublish = 24;

        uint8_t hoursOffsetFCST = 0;

        uint8_t infoSource = 0;

        std::vector<LocationInfo> locationInfos;
    };

    /*
     * @brief : Marine Meteorological Forecast (Sea area)
     */

    struct ASM_DAC_412_FI_27 : ASM_DAC_FI
    {
        struct SeaAreaInfo
        {
            uint8_t areaCode;

            uint8_t weatherCode = 0;

            uint16_t   windDirection = 360;

            uint8_t    windScaleLow = 20;

            uint8_t    windScaleHigh = 20;
            
            uint8_t    visibility = 22;
        };

        uint8_t hourPublish = 24;

        uint8_t hoursOffsetFCST = 0;

        uint8_t infoSource = 0;

        std::vector<SeaAreaInfo> seaAreaInfos;
    };

    /**
     * @brief : Marine Environmental Forecast (Coordinate)
     */
    struct ASM_DAC412_FI28 : ASM_DAC_FI
    {
        struct LocationInfo
        {
            Coordinate coordinate;

            uint32_t   flowVelocity = 1023;

            uint16_t   flowDirection = 360;
            
            uint8_t    waveHeight = 252;

            uint16_t   waveDirection = 360;

            uint16_t   temperature = 703;
        };

        uint8_t hourPublish = 24;

        uint8_t hoursOffsetFCST = 0;

        uint8_t infoSource = 0;

        std::vector<LocationInfo> locationInfos;
    };

    /**
     * @brief : Marine Environmental Forecast (Sea area)
     */
    struct ASM_DAC_412_FI_29 : ASM_DAC_FI
    {
        struct SeaAreaInfo
        {
            uint8_t  areaCode;
            uint16_t temperatureLow = 511;
            uint8_t  temperatureHigh = 202;
            uint32_t flowDirectionAvg = 360;
            uint32_t flowDirctionMax = 360;
            uint8_t  flowVelocityAvg = 252;
            uint8_t  flowVelocityMax = 252;
            uint8_t  waveHeight = 252;
            uint8_t  swellHeight = 252;
        };

         uint8_t hourPublish = 24;

        uint8_t hoursOffsetFCST = 0;

        uint8_t infoSource = 0;

        std::vector<SeaAreaInfo> seaAreaInfos;
    };

    /**
     * @brief : Marine Environmental Forecast (Along shore)
     */
    struct ASM_DAC_412_FI_30 : ASM_DAC_FI
    {
        struct WaterAreaInfo
        {
            uint8_t  areaCode;
            uint16_t temperatureLow = 511;
            uint8_t  temperatureHigh = 202;
            uint8_t  waveHeightLow = 252;
            uint8_t  waveHeightHigh = 252;
            uint16_t tideHigh = 1003;
            uint64_t timestampTideHigh;
            uint16_t tideLow = 503;
            uint64_t timestampTideLow;
        };

        uint8_t hourPublish = 24;

        uint8_t hoursOffsetFCST = 0;

        uint8_t infoSource = 0;

        std::vector<WaterAreaInfo> waterAreaInfos;
    };

    /*
     * @brief : Marine Meteorological & Environmental Warning
     */
    struct ASM_DAC_412_FI_31 : ASM_DAC_FI
    {
        struct TropicalCyclonePathPoint
        {
            uint64_t timestamp = 0;
            double   centerLongitude = 181.0;
            double   centerLatitude = 91.0;
            uint8_t  cycloneType = 0;
            uint16_t radiusWindScale7 = 0;
            uint16_t radiusWindScale10 = 0;
            uint16_t radiusWindScale12 = 0;
            uint8_t  moveSpeed = 0;
            uint16_t moveDirection = 360;
            uint8_t  maxWindScale = 20;
            uint16_t centerPressure = 403;
        };

        struct TropicalCycloneElement
        {
            uint32_t MRN = 0;
            uint8_t  fragment = 0;
            std::vector<TropicalCyclonePathPoint> pathPoints;
        };

        struct GeneralWarningElement
        {
            uint32_t MRN = 0;
            uint8_t  fragment = 0;
            uint8_t  seaAreaCode = 0;
            uint8_t  warningLevel = 0;
        };

        struct StormSurgeElement
        {
            uint32_t MRN = 0;
            uint8_t  fragment = 0;
            uint8_t  cityCode = 0;
            uint8_t  surgeHeight = 0;
            uint8_t  warningLevel = 0;
        };

        struct IceWarningElement
        {
            uint32_t MRN = 0;
            uint8_t regionCode = 0;
            uint8_t warningLevel = 0;
        };

        uint8_t  warningType = 0;

        TropicalCycloneElement             cyclone;
        std::vector<GeneralWarningElement> generalWarnings;
        std::vector<StormSurgeElement>     stormSurges;
        IceWarningElement                  iceWarning;

        uint64_t publishTime = 0;
        uint64_t startTime = 0;
        uint64_t endTime = 0;
        uint8_t  warningDuration = 0;
        uint8_t  infoSource = 0;
    };
    
    /**
     * @brief : Tide
     */
    struct ASM_DAC_412_FI_32 : ASM_DAC_FI
    {
        struct TideInfo
        {
            Coordinate coordinate;
            uint16_t   tideHigh;
            uint64_t   timestampTideHigh;
            uint16_t   tideLow;
            uint64_t   timestampTideLow;
            uint16_t   tidalDatum;
        };

        uint8_t hourPublish = 24;

        uint8_t infoSource = 0;

        std::vector<TideInfo> tideInfos;
    };

    /**
     * @brief : AtoN dynamics (non-AIS)
     */
    struct ASM_DAC_412_FI_33 : ASM_DAC_FI
    {
        struct Element
        {
            uint32_t   MRN = 0;
            uint8_t    fragmentDesc = 0;
            uint8_t    type = 0;
            Coordinate coordinate;
            
            uint8_t    rhythmNameCode = 0;
            uint8_t    rhythmParamCode = 0;
            uint8_t    bodyColor = 0;
            uint8_t    lightColor = 0;
            uint8_t    lightPeriod = 0;
            uint8_t    range = 0;
            uint8_t    morseCode = 0;

            Coordinate prevCoordinate;
            bool       isRoughPosition = false;

            uint8_t    failureType = 0;

            uint64_t   timestampStart = 0;
            uint64_t   timestampEnd = 0;
        };

        uint8_t              status = 0;
        uint8_t              precaution = 0;
        std::vector<Element> elements;
    };

    struct ASM_DAC_412_FI_34 : ASM_DAC_FI
    {
        struct Element
        {
            uint32_t   mmsi = 0;
            uint32_t   MRN = 0;
            uint8_t    fragmentDescription = 0;
            uint8_t    type = 0;
            Coordinate coordinate;
            
            uint8_t    range = 0;

            Coordinate prevCoordinate;
            bool       isRoughPosition = false;
        };

        uint8_t              atonAttribute = 0;
        uint8_t              status = 0;
        uint8_t              precaution = 0;
        std::vector<Element> elements;
    };

    struct GeoCircle
    {
        Coordinate coordinate;
        double     range = 0.0;
    };

    struct GeoPolygon
    {
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : Obstacle
     */
    struct ASM_DAC_412_FI_35 : ASM_DAC_FI
    {
        struct Sector
        {
            uint16_t startAngle = 0;
            uint16_t endAngle = 0;
            uint8_t  range = 0;
        };

        uint32_t                MRN = 0;
        uint8_t                 fragment = 0;
        uint8_t                 type = 0;
        uint8_t                 geometryType = 0;
        
        Coordinate              coordinate;
        uint8_t                 range = 0;
        Sector                  sector;
        std::vector<Coordinate> polygon;

        uint64_t                timestampActivate = 0;
        uint8_t                 duration = 0;
        uint8_t                 cautionCode = 0;
    };

    /**
     * @brief : Maritime activity
     */
    struct ASM_DAC_412_FI_36 : ASM_DAC_FI
    {
        struct Sector
        {
            uint16_t startAngle = 0;
            uint16_t endAngle = 0;
            double   range = 0.0;
        };

        uint32_t                MRN = 0;
        uint8_t                 fragment = 0;
        uint8_t                 type = 0;
        uint32_t                mmsi = 0;
        uint8_t                 geometryType = 0;
        
        Coordinate              coordinate;
        double                  range = 0.0;
        Sector                  sector;
        std::vector<Coordinate> polygon;

        uint8_t                 timeType = 0;
        uint64_t                timestampStart = 0;
        uint64_t                timestampEnd = 0;
        uint8_t                 cautionCode = 0;
    };

    /*
     * @brief : Maritime towing
     */
    struct ASM_DAC_412_FI_37 : ASM_DAC_FI
    {
        uint32_t   MRN      = 0;
        uint8_t    fragment = 0;
        uint32_t   mmsi = 0;
        Coordinate node1;
        Coordinate node2;
        uint8_t    towingMethod = 0;
        uint32_t   length = 0;
        uint8_t    width = 0;
        uint8_t    speed = 0;
        uint64_t   timestampStart = 0;
        uint64_t   timestampEnd = 0;
        uint8_t    cautionCode = 0;   
    };

    /**
     * @brief : Military activity
     */
    struct ASM_DAC_412_FI_38 : ASM_DAC_FI
    {
        uint32_t                MRN = 0;
        uint8_t                 fragment = 0;
        uint8_t                 type = 0;
        uint8_t                 geoType = 0;
        Coordinate              coordinate;
        double                  range = 0.0;
        ASM_DAC_412_FI_35::Sector sector;
        std::vector<Coordinate> polygon;
        uint64_t                timestampStart = 0;
        uint64_t                timestampEnd = 0;
        uint8_t                 timeType = 0;
        uint8_t                 cautionCode = 0;
    };

    /**
     * @brief : Vessel distress
     */
    struct ASM_DAC_412_FI_39 : ASM_DAC_FI
    {
        uint32_t   MRN = 0;
        uint8_t    fragment = 0;
        uint8_t    distressType = 0;
        uint16_t   statusDescription = 0;
        uint8_t    judgment = 0;
        Coordinate coordinate;
        uint8_t    locationInstruction = 0;
        uint64_t   distressTime = 0;
        uint8_t    duration = 0;
        uint8_t    cautionCode = 0;
    };

    /**
     * @brief : Designated area
     */
    struct ASM_DAC_412_FI_40 : ASM_DAC_FI
    {
        uint32_t                MRN = 0;
        uint8_t                 fragment = 0;
        uint8_t                 type = 0;
        uint8_t                 geoType = 0;
        Coordinate              coordinate;
        double                  range = 0.0;
        ASM_DAC_412_FI_35::Sector sector;
        std::vector<Coordinate> polygon;
        uint8_t                 timeType = 0;
        uint64_t                timestampStart = 0;
        uint64_t                timestampEnd = 0;
        uint8_t                 cautionCode = 0;
    };

    /**
     * @brief : Bridge
     */
    struct ASM_DAC_412_FI_41 : ASM_DAC_FI
    {
        struct BridgeSpan 
        {
            Coordinate center;
            uint16_t   height           = 0;
            uint16_t   width            = 0;
            uint16_t   directionToPass  = 0;
            uint8_t    passAbility      = 0;
            bool       enableMeeting    = 0;
            bool       enableOvertaking = 0;
        };

        uint32_t   MRN      = 0;
        uint8_t    fragment = 0;
        uint16_t   flowVelocity = 0;
        uint16_t   flowDirection = 0;
        uint64_t   publicationTime = 0;
        std::vector<BridgeSpan> bridgeSpans;
    };

    /**
     * @brief : Navigational Channel
     */
    struct ASM_DAC_412_FI_42 : ASM_DAC_FI
    {
        uint32_t                MRN      = 0;
        uint8_t                 fragment = 0;
        uint16_t                width    = 0;
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : Navigational Channel Boundary
     */
    struct ASM_DAC_412_FI_43 : ASM_DAC_FI
    {
        uint32_t                MRN      = 0;
        uint8_t                 fragment = 0;
        uint8_t                 edgeType = 0;
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : Net Sounder
     */
    struct ASM_DAC_412_FI_45 : ASM_DAC_FI
    {
        struct NetInfo
        {
            uint32_t   MRN = 0;
            uint8_t    fragmentDesc = 0;
            Coordinate coordinate;
        };

        uint8_t              fragment    = 0;
        uint8_t              type        = 0;
        bool                 isContinous = 0;
        std::vector<NetInfo> nets;
        std::string          description;
    };

    /**
     * @brief : Frontend prompt text
     */
    struct ASM_DAC_413_FI_5 : ASM_DAC_FI
    {
        uint8_t     promptContent   = 0;
        uint8_t     displayDuration = 0;
        std::string text;
    };

    /**
     * @brief : Supplementary position fragment
     */
    struct ASM_DAC_413_FI_7 : ASM_DAC_FI
    {
        uint32_t                MRN = 0;
        uint16_t                mainDAC = 0;
        uint8_t                 mainFI = 0;
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : Supplementary description fragment
     */
    struct ASM_DAC_413_FI_8 : ASM_DAC_FI
    {
        uint32_t    MRN = 0;
        uint16_t    mainDAC = 0;
        uint8_t     mainFI = 0;
        uint8_t     encodingType = 0;
        uint16_t    textLength = 0;
        std::string description;
    };

    /**
     * @brief : Shore-based Response to Route Recommendation (DAC 412, FI 47)
     */
    struct ASM_DAC_412_FI_47 : ASM_DAC_FI
    {
        double                  effectiveTime = 31.5;
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : Shore-based Response to Hydrometeorology Request (DAC 412, FI 49)
     */
    struct ASM_DAC_412_FI_50 : ASM_DAC_FI
    {
        struct PointForecast
        {
            uint8_t  windSpeed     = 63;
            uint16_t windDirection = 360;
            uint8_t  visibility    = 255;
            uint8_t  waveHeight    = 255;
            uint16_t waveDirection = 360;
            uint8_t  swellHeight   = 255;
        };

        uint32_t                    MRN = 0;
        uint64_t                    forecastTime = 0;
        
        bool                        hasWindSpeed = false;
        bool                        hasWindDirection = false;
        bool                        hasVisibility = false;
        bool                        hasWaveHeight = false;
        bool                        hasWaveDirection = false;
        bool                        hasSwellHeight = false;

        std::vector<PointForecast>  points;
    };

    /**
     * @brief : Extended Static and Voyage Information - Part A (DAC 412, FI 50)
     */
    struct ASM_DAC_412_FI_51 : ASM_DAC_FI
    {
        uint32_t    mmsi = 0;
        uint8_t     extendedVesselType = 0;
        uint8_t     autonomousLevel = 0;
        uint8_t     naviStatus = 15;
        double      draught = 0.0;
        uint64_t    eta = 0;
        std::string portCode;
    };

    /**
     * @brief : Extended Static and Voyage Information - Part B (DAC 412, FI 51)
     */
    struct ASM_DAC_412_FI_52 : ASM_DAC_FI
    {
        uint32_t    mmsi = 0;
        uint16_t    crewNum = 0;
        std::string chineseName;
    };

    /**
     * @brief : Cancellation of specific information by MRN
     */
    struct ASM_DAC_413_FI_9 : ASM_DAC_FI
    {
        struct Element
        {
            uint16_t dac = 0;
            uint8_t  fi  = 0;
            uint32_t mrn = 0;
        };
        std::vector<Element> elements;
    };

    /**
     * @brief : Cancellation of continuous MRN range information
     */
    struct ASM_DAC_413_FI_10 : ASM_DAC_FI
    {
        uint16_t targetDAC = 0;
        uint8_t  targetFI  = 0;
        uint32_t startMRN  = 0;
        uint32_t endMRN    = 0;
    };
}

#endif