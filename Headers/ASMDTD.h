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
             * valid range : [(-60) - 60]℃
             * unit : %
             */
            double    temperature = 100.0F;

            /**
             * valid range : [800 - 1200]hPa
             * unit : %
             */
            uint16_t  airPressure = 0;

            /**
             * valid range : [0.0 - 25.0] nm
             * unit : %
             */
            double    visibility = -1.0F;
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
            /**
             * valid range : [0 - 127]
             */
            uint8_t areaCode;

            /**
             * valid range : [0 - 31]
             */
            uint8_t weatherCode = 0;

            /**
             * valid range : [0 - 511]
             * unit : degree
             */
            uint16_t   windDirection = 360;

            /**
             * valid range : [0 - 19]
             * 20 = default = not available
             * 21 - 31 = reserved for future use
             */
            uint8_t    windScaleLow = 20;

            /**
             * valid range : [0 - 19]
             * 20 = default = not available
             * 21 - 31 = reserved for future use
             */
            uint8_t    windScaleHigh = 20;
            
            /**
             * valid range : [0 - 20] km
             * 21 = greater than 20 km
             * 22 = default = not available
             * 23 - 31 = reserved for future use
             */
            uint8_t    visibility = 22;
        };

        /**
         * valid range : [0 - 23]
         */
        uint8_t hourPublish = 24;

        /**
         * valid range : [0 - 127]
         * Offset from publish time
         */
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

            /**
             * valid range : [0 - 1021] cm/s
             * 1022 = equals or greater than 1022 cm/s
             * 1023 = not available
             */
            uint32_t   flowVelocity = 1023;

            /**
             * valid range : [0° - 359°]
             * 360 = not available
             */
            uint16_t   flowDirection = 360;
            
            /**
             * valid range : [0.0 - 25.0] m
             * -1.0 = not available
            */
            double     waveHeight;

            /**
             * valid range : [0° - 359°]
             * 360 = not available
            */
            uint16_t   waveDirection = 360;

            /**
             * valid range : [(-10.0)℃ - 60.0℃]
             * 100 = not available
            */
            double     temperature = 100.0F;
        };

        /**
         * valid range : [0 - 23]
         */
        uint8_t hourPublish = 24;

        /**
         * valid range : [0 - 127]
         * Offset from publish time
         */
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
            double   temperatureLow;
            double   temperatureHigh;
            uint32_t flowDirectionAvg;
            uint32_t flowDirctionMax;
            double   flowVelocityAvg;
            double   flowVelocityMax;
            double   waveHeight;
            double   swellHeight;
        };

        /**
         * valid range : [0 - 23]
         */
         uint8_t hourPublish = 24;

        /**
         * valid range : [0 - 127]
         * Offset from publish time
         */
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
            double   temperatureLow;
            double   temperatureHigh;
            double   waveHeightLow;
            double   waveHeightHigh;
            double   tideHigh;
            uint64_t timestampTideHigh;
            double   tideLow;
            uint64_t timestampTideLow;
        };
        /**
         * valid range : [0 - 23]
         */
        uint8_t hourPublish = 24;

        /**
         * valid range : [0 - 127]
         * Offset from publish time
         */
        uint8_t hoursOffsetFCST = 0;

        uint8_t infoSource = 0;

        std::vector<WaterAreaInfo> waterAreaInfos;
    };

    /*
     * @brief : Marine Meteorological & Environmental Warning
     */
    struct ASM_DAC_412_FI_31 : ASM_DAC_FI
    {
        // Tropical Cyclone element (warningType = 1)
        struct TropicalCyclone
        {
            double   centerLongitude = 181.0; // 22 bits, 0.01' resolution
            double   centerLatitude = 91.0;   // 21 bits, 0.01' resolution
            uint8_t  cycloneType = 0;         // 3 bits
            uint16_t radiusWindScale7 = 0;    // 10 bits, km
            uint16_t radiusWindScale10 = 0;   // 8 bits, km
            uint16_t radiusWindScale12 = 0;   // 7 bits, km
            uint8_t  moveSpeed = 0;           // 6 bits, km/h
            uint16_t moveDirection = 360;     // 9 bits, degree
            uint8_t  maxWindScale = 20;       // 5 bits, scale
            uint16_t centerPressure = 403;    // 9 bits, base 800hPa, 1hPa step
        };

        // General warning element for wind/wave/sea fog (warningType = 2, 3, 4)
        struct GeneralWarningElement
        {
            uint8_t seaAreaCode = 0;   // 7 bits
            uint8_t warningLevel = 0;  // 2 bits
        };

        // Storm surge element (warningType = 5)
        struct StormSurgeElement
        {
            uint8_t cityCode = 0;      // 6 bits
            double  surgeHeight = 0.0; // 5 bits, 10cm step
            uint8_t warningLevel = 0;  // 2 bits
        };

        // Ice warning element (warningType = 6)
        struct IceWarningElement
        {
            uint8_t regionCode = 0;    // 7 bits
            uint8_t warningLevel = 0;  // 2 bits
        };

        uint32_t MRN = 0;             // 17 bits
        uint8_t  fragment = 0;        // 2 bits
        uint8_t  warningType = 0;     // 4 bits

        TropicalCyclone                    cyclone;
        std::vector<GeneralWarningElement> generalWarnings;
        std::vector<StormSurgeElement>     stormSurges;
        IceWarningElement                  iceWarning;

        uint64_t publishTime = 0;     // 20 bits
        uint64_t startTime = 0;       // 20 bits
        uint64_t endTime = 0;         // 20 bits
        uint8_t  warningDuration = 0; // 6 bits
        uint8_t  infoSource = 0;      // 3 bits
    };
    
    /**
     * @brief : Tide
     */
    struct ASM_DAC_412_FI_32 : ASM_DAC_FI
    {
        struct TideInfo
        {
            Coordinate coordinate;
            double     tideHigh;
            uint64_t   timestampTideHigh;
            double     tideLow;
            uint64_t   timestampTideLow;
            double     tidalDatum;
        };

        uint8_t hourPublish = 24;

        uint8_t infoSource = 0;

        std::vector<TideInfo> tideInfos;
    };

    /**
     * @brief : 航标动态(非AIS航标)
     */
    struct ASM_DAC_412_FI_33 : ASM_DAC_FI
    {
        struct Element
        {
            uint32_t   MRN = 0;
            uint8_t    type = 0;
            Coordinate coordinate;
            
            // Newly established & Light character changed specifics
            uint8_t    rhythmNameCode = 0;
            uint8_t    rhythmParamCode = 0;
            uint8_t    bodyColor = 0;
            uint8_t    lightColor = 0;
            uint8_t    lightPeriod = 0;
            uint8_t    range = 0;
            uint8_t    morseCode = 0; // Morse code changed

            // Relocated specifics
            Coordinate prevCoordinate;
            bool       isRoughPosition = false;

            // Failure specifics
            uint8_t    failureType = 0;

            // Temporary operations specifics
            uint64_t   timestampStart = 0;
            uint64_t   timestampEnd = 0;
        };

        uint32_t             MRN = 0;
        uint8_t              fragment = 0;
        uint8_t              status = 0;
        uint8_t              precaution = 0;
        std::vector<Element> elements;
        std::string          description;
    };

    struct ASM_DAC_412_FI_34 : ASM_DAC_FI
    {
        struct Element
        {
            uint32_t   mmsi = 0;
            uint32_t   MRN = 0;
            uint8_t    type = 0;
            Coordinate coordinate;
            
            // Newly established specifics
            uint8_t    range = 0;

            // Relocated specifics
            Coordinate prevCoordinate;
            bool       isRoughPosition = false;
        };

        uint32_t             MRN = 0;
        uint8_t              fragment = 0;
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
     * @brief : 碍航物
     */
    struct ASM_DAC_412_FI_35 : ASM_DAC_FI
    {
        struct Sector
        {
            uint16_t startAngle = 0;
            uint16_t endAngle = 0;
            double   range = 0.0; // nm
        };

        uint32_t                MRN = 0;
        uint8_t                 fragment = 0;
        uint8_t                 type = 0;
        uint8_t                 geometryType = 0; // 0: 点/圆形, 1: 扇形, 2: 多边形, 3: 折线
        
        Coordinate              coordinate;
        double                  range = 0.0; // meters (点/圆形半径)
        Sector                  sector;
        std::vector<Coordinate> polygon;

        uint64_t                timestampActivate = 0;
        uint8_t                 duration = 0;
        uint8_t                 cautionCode = 0;
    };

    /**
     * @brief : 水上水下活动
     */
    struct ASM_DAC_412_FI_36 : ASM_DAC_FI
    {
        struct Sector
        {
            uint16_t startAngle = 0;
            uint16_t endAngle = 0;
            double   range = 0.0; // nm
        };

        uint32_t                MRN = 0;
        uint8_t                 fragment = 0;
        uint8_t                 type = 0; // 活动类型
        uint32_t                mmsi = 0; // 施工船 MMSI
        uint8_t                 geometryType = 0; // 0: 圆形, 1: 扇形, 2: 多边形, 3: 折线
        
        Coordinate              coordinate;
        double                  range = 0.0; // nm (圆形半径)
        Sector                  sector;
        std::vector<Coordinate> polygon;

        uint8_t                 timeType = 0; // 0: 连续, 1: 间歇
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
        uint32_t   length = 0;
        uint8_t    width = 0;
        double     speed = 0.0;
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
        uint8_t                 geoType = 0; // 0: 圆形, 1: 扇形, 2: 多边形, 3: 折线
        Coordinate              coordinate; // 中心点
        double                  range = 0.0; // 半径
        ASM_DAC_412_FI_35::Sector sector; // 扇形参数
        std::vector<Coordinate> polygon; // 多边形/折线顶点
        uint64_t                timestampStart = 0;
        uint64_t                timestampEnd = 0;
        uint8_t                 timeType = 0;
        uint8_t                 cautionCode = 0;
    };

    /**
     * @brief : Ship distress (船舶遇险)
     */
    struct ASM_DAC_412_FI_39 : ASM_DAC_FI
    {
        uint32_t   MRN = 0;                  // 17 bits
        uint8_t    fragment = 0;             // 2 bits
        uint8_t    distressType = 0;         // 4 bits
        uint16_t   statusDescription = 0;    // 16 bits
        uint8_t    judgment = 0;             // 3 bits
        Coordinate coordinate;               // 经度 22 bits, 纬度 21 bits
        uint8_t    locationInstruction = 0;  // 1 bit
        uint64_t   distressTime = 0;         // 20 bits
        uint8_t    duration = 0;             // 6 bits
        uint8_t    cautionCode = 0;          // 4 bits
    };

    /**
     * @brief : Designated Area (划定区域)
     */
    struct ASM_DAC_412_FI_40 : ASM_DAC_FI
    {
        uint32_t                MRN = 0;                  // 17 bits
        uint8_t                 fragment = 0;             // 2 bits
        uint8_t                 type = 0;                 // 5 bits (areaType)
        uint8_t                 geoType = 0;              // 2 bits
        Coordinate              coordinate;               // 中心点/点1
        double                  range = 0.0;              // 半径 (nm)
        ASM_DAC_412_FI_35::Sector sector;                 // 扇形
        std::vector<Coordinate> polygon;                  // 多边形/矩形顶点
        uint8_t                 timeType = 0;             // 1 bit
        uint64_t                timestampStart = 0;       // 20 bits
        uint64_t                timestampEnd = 0;         // 20 bits
        uint8_t                 cautionCode = 0;          // 4 bits
    };

    /**
     * @brief : Bridge
     */
    struct ASM_DAC_412_FI_41 : ASM_DAC_FI
    {
        struct BridgeSpan 
        {
            Coordinate center;
            double     height           = 0.0;
            double     width            = 0.0;
            uint16_t   directionToPass  = 0;
            uint8_t    passAbility      = 0;
            bool       enableMeeting    = 0;
            bool       enableOvertaking = 0;
        };

        uint32_t   MRN      = 0;
        uint8_t    fragment = 0;
        uint16_t   flowVelocity = 0;
        uint16_t   flowDirection = 0;
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
        uint8_t                 edgeType = 0; // 0: left edge, 1: right edge
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : Net Sounder
     */
    struct ASM_DAC_412_FI_44 : ASM_DAC_FI
    {
        uint32_t                MRN         = 0;
        uint8_t                 fragment    = 0;
        uint8_t                 type        = 0;
        bool                    isContinous = 0;
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : Frontend Prompt Text (前端提示文字)
     */
    struct ASM_DAC_413_FI_5 : ASM_DAC_FI
    {
        uint8_t     promptContent   = 0;
        uint8_t     displayDuration = 0;
        std::string text;
    };

    /**
     * @brief : 位置补充片段 (Supplementary Position Fragment)
     */
    struct ASM_DAC_413_FI_7 : ASM_DAC_FI
    {
        uint32_t                MRN = 0;
        uint16_t                mainDAC = 0;
        uint8_t                 mainFI = 0;
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : 描述性补充片段 (Supplementary Description Fragment)
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
     * @brief : Shore-based Response to Route Recommendation (DAC 412, FI 46)
     */
    struct ASM_DAC_412_FI_46 : ASM_DAC_FI
    {
        double                  effectiveTime = 31.5; // Lifecycle, unit 0.5h
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : Shore-based Response to Hydrometeorology Request (DAC 412, FI 49)
     */
    struct ASM_DAC_412_FI_49 : ASM_DAC_FI
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
    struct ASM_DAC_412_FI_50 : ASM_DAC_FI
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
    struct ASM_DAC_412_FI_51 : ASM_DAC_FI
    {
        uint32_t    mmsi = 0;
        uint16_t    crewNum = 0;
        std::string chineseName;
    };
}

#endif