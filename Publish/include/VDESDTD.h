#ifndef VDES_DTD_H_
#define VDES_DTD_H_

#include "Coordinate.h"

#include <string>
#include <vector>

namespace VDES
{
    enum class CommunicationType
    {
        TCP,    // The VDES terminal uses TCP for communication with the MKD.
        UDP,    // The VDES terminal uses UDP for firmware upgrades.
        UART,   // Serial port
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
        uint8_t month  = 0;
        uint8_t day    = 0;
        uint8_t hour   = 24;
        uint8_t minute = 60;
    };

    struct OwnVessel
    {
        uint32_t       mmsi       = 0;
        uint32_t       imo        = 0;
        uint8_t        vesselType = 37;
        uint8_t        naviStatus = 15;
        double         draught    = 0.0F;
        std::string    name;
        std::string    callsign;
        std::string    destination;
        ReferencePoint antennaPosInner;
        ReferencePoint antennaPosOuter;
        ETA            eta;
        uint16_t       crewNum;
    };

    struct ETHInfo
    {
        uint8_t     ID;
        uint32_t    portNumber;
        std::string ip;
        std::string mac;
        std::string mask;
        std::string gateway;
        std::string dns1;
        std::string dns2;
    };

    struct VersionInfo
    {
        std::string deviceType;
        std::string vendorID;
        std::string serialNumber;
        std::string productCode;
        std::string softwareRevision;
        std::string hardwareRevision;
    };

    enum class InformationType
    {
        MESSAGE_RECEIVED = 0,
        ATON,
        MSI_OBSTACLE,
        MSI_MARITINE_OPERATION,
        MSI_MILITARY_ACTIVITY,
        MSI_MARITIME_DISTRESS,
        HYDRO_METEOROLOGY,
        ABNORMAL_SHIP,
        MARINE_METEOROLOGY_FCST,
        MARINE_ENVIRONMENT_FCST,
        MSI_DESIGNATED_AREA,
    };

    struct BusinessAttribute
    {
        uint32_t dataID      = 0;
        bool     isCertified = false;
        bool     read        = false;
        /*
        * If it is self-initiated business data, it represents the sending time.
        * Otherwise, it represents the receiving time.
        */
        uint64_t timestamp = 0;
    };

    enum class SendStatusCode
    {
        NONE = -1,

        /*
        * 0 - Broadcast successful
        */
        SUCCESS = 0,

        /*
        * 1 - The message was successfully sent and a reply was received from
        * the target node(addressing was successful).
        */
        SUCCESS_WITH_REPLY = 1,

        /*
        * 2 - Failed to send.
        */
        FAILURE = 2,

        /*
        * 3 - The sending queue is full.Please try again later.
        */
        QUEUE_FULL = 3,

        /*
        * 4 - Radio link related time - out
        */
        RADIO_ERROR = 4,

        /*
        * 5 - Data length exceeds the limit
        */
        DATA_LENGTH_ERROR = 5,

        /*
        * 6 - Refused service
        */
        REFUSED_SERVICE = 6,
    };

    /**
     * @brief  Sending status of the message
     */
    enum class SendStatus
    {
        NONE,

        /*
        * SendResultCode = NONE
        */
        TIMEOUT,

        /*
        * SendResultCode = SUCCESS, SUCCESS_WITH_REPLY
        */
        SUCCESS,

        /*
        * SendResultCode = FAILURE, QUEUE_FULL, RADIO_ERROR,
        *                  DATA_LENGTH_ERROR, REFUSED_SERVICE
        */
        FAILURE,
    };

    enum class ReplyStatus
    {
        UN_REPLIED,
        VALID,
        INVALID,
    };

    enum class MessageType
    {
        VDES,
        AIS
    };

    struct Message : BusinessAttribute
    {
        uint32_t mmsiSource = 0;
        /*
        * Destination MMSI
        * if mmsiDestination == 0, the message is broadcast,
        * else, the message is sent to the specified MMSI
        */
        uint32_t       mmsiDestination = 0;
        std::string    content;
        bool           needAck     = false;
        ReplyStatus    replyStatus = ReplyStatus::UN_REPLIED;
        uint64_t       deadline    = 0;
        SendStatus     status      = SendStatus::NONE;
        SendStatusCode statusCode  = SendStatusCode::NONE;
        MessageType    messageType = MessageType::VDES;
    };

    struct AtoN : BusinessAttribute
    {
        uint32_t       mmsi = 0;
        Coordinate     coordinate;
        uint8_t        type          = 0;
        uint8_t        status        = 0;
        bool           isVirtual     = false;
        bool           isAssigned    = false;
        bool           isOffPosition = false;
        std::string    name;
        ReferencePoint refPoint;
        uint64_t       deadline = 0;
    };

    /**
     * Maritime Safety Information
     */
    struct MSIInfo
    {
        uint32_t dataID = 0;
        uint8_t  type;
        uint8_t  subtype;
        uint64_t timestampStart = 0;
        uint64_t timestampEnd   = 0;
    };

    /**
     * Maritime Safety Information - Obstacle
     */
    struct MSIObstacle : BusinessAttribute
    {
        /*
        * Obstacle type
        * value range [0 - 15]
        * 0 = not available
        * 14 = reserved
        * 15 = reserved
        */
        uint8_t type = 0;

        Coordinate coordinate;

        /*
        * Radius of impact of obstacle.
        * value range: [0 - 12.7]nm
        */
        double range = 0.0F;

        /*
        * value range : [0 - 7]
        * 0 = not available
        * 4 - 7 = reserved
        */
        uint8_t comment = 0;

        uint64_t timestampStart = 0;

        uint64_t timestampEnd = 0;

        // New fields for geometry support
        uint8_t                 geometryType     = 0;   // 0: 点、圆形, 1: 扇形, 2: 多边形, 3: 折线
        uint16_t                sectorStartAngle = 0;
        uint16_t                sectorEndAngle   = 0;
        std::vector<Coordinate> polygon;
    };

    /**
     * Maritime Safety Information - Maritime operation
     */
    struct MSIMaritimeOperation : BusinessAttribute
    {
        /*
        * Operation type
        * value range [0 - 15]
        * 0 = not available
        * 6 - 15 = reserved
        */
        uint8_t type = 0;

        /*
        * Construction vessel status
        * value range: [0 - 7]
        * 0 = not available
        * 5 - 7 = reserved
        */
        uint8_t status = 0;

        /*
        * Construction vessel's MMSI
        * Multiple ships or ship without AIS installation, set to 0
        */
        uint32_t mmsi = 0;

        /*
        * If coordinateSW is invalid, it forms a circular region with the range;
        *
        */
        Coordinate coordinateNE;

        /*
        * If coordinateSW is valid, it forms a rectangular region with coordianteNE.
        */
        Coordinate coordinateSW;

        double range = 0.0F;

        uint64_t timestampStart = 0;

        uint64_t timestampEnd = 0;

        // New fields for geometry and attributes support
        uint8_t                 geometryType     = 0;   // 0: 圆形, 1: 扇形, 2: 多边形, 3: 折线
        uint16_t                sectorStartAngle = 0;
        uint16_t                sectorEndAngle   = 0;
        std::vector<Coordinate> polygon;
        uint8_t                 timeType    = 0;   // 0: 连续, 1: 间歇
        uint8_t                 cautionCode = 0;
    };

    /**
     * Maritime Safety Information - Military activity
     */
    struct MSIMilitaryActivity : BusinessAttribute
    {
        /*
        * The event theme code
        * value range: [0 - 15]
        * 0 = not available
        * 10 - 15 = reserved
        */
        uint8_t theme = 0;

        bool isContinuing = false;

        uint8_t                 geometryType = 0;         // 0: 圆形, 1: 扇形, 2: 多边形, 3: 折线
        Coordinate              coordinateNE;             // 中心点 (圆形/扇形)
        double                  range            = 0.0;   // 半径
        uint16_t                sectorStartAngle = 0;
        uint16_t                sectorEndAngle   = 0;
        std::vector<Coordinate> polygon;   // 多边形/折线顶点

        uint64_t timestampStart = 0;

        uint64_t timestampEnd = 0;

        uint8_t timeType = 0;   // 0: 连续, 1: 间歇

        /*
        * Points to note
        * value range: [0 - 15]
        * 0 = not available
        * 5 - 15 = reserved
        */
        uint8_t cautionCode = 0;
    };

    /**
     * Maritime Safety Information - Maritime Towing
     */
    struct MSIMaritimeTowing : BusinessAttribute
    {
        uint32_t   mmsi = 0;               // 被拖带船舶 MMSI
        Coordinate coordinateStart;        // 起始经纬度
        Coordinate coordinateEnd;          // 终止经纬度
        uint32_t   length         = 0;     // 总长 (m)
        uint8_t    width          = 0;     // 总宽 (m)
        double     speed          = 0.0;   // 航速 (kn)
        uint64_t   timestampStart = 0;     // 起拖时间
        uint64_t   timestampEnd   = 0;     // 抵达时间
        uint8_t    cautionCode    = 0;     // 注意事项
    };

    /**
     * Maritime Safety Information - Maritime distress
     */
    struct MSIMaritimeDistress : BusinessAttribute
    {
        /*
        * Distress type
        * value range: [0 - 15]
        * 0 = not available
        * 12 - 15 = reserved
        */
        uint8_t type = 0;

        /*
        * Situation description
        * value range: [0 - 15]
        * 0 = not available
        * 12 - 15 = reserved
        */
        uint8_t situation = 0;

        /*
        * Vessel status
        * value range: [0 - 7]
        * 0 = not available
        * 5 - 7 = reserved
        */
        uint8_t status;

        /*
        * Additional description
        * value range: [0 - 15]
        * 0 = not available
        * 6 - 15 = reserved
        */
        uint8_t additional;

        Coordinate coordinate;
        uint64_t   timestampValid = 0;

        // New fields for FI=39 support
        uint16_t statusDescription   = 0;   // 状况描述 (16-bit)
        uint8_t  judgment            = 0;   // 判断位 (3-bit)
        uint8_t  locationInstruction = 0;   // 位置说明 (1-bit)
        uint8_t  duration            = 0;   // 发布时效 (6-bit)
        uint8_t  cautionCode         = 0;   // 注意事项 (4-bit)
    };

    /**
     * Maritime Safety Information - Designated Area (划定区域)
     */
    struct MSIDesignatedArea : BusinessAttribute
    {
        uint32_t                MRN          = 0;         // 17 bits
        uint8_t                 fragment     = 0;         // 2 bits
        uint8_t                 areaType     = 0;         // 5 bits
        uint8_t                 geometryType = 0;         // 2 bits
        Coordinate              coordinateNE;             // 圆形/扇形中心点，或矩形东北角
        double                  range            = 0.0;   // 半径 (nm)
        uint16_t                sectorStartAngle = 0;     // 扇形起始角
        uint16_t                sectorEndAngle   = 0;     // 扇形终止角
        std::vector<Coordinate> polygon;                  // 包含多边形/矩形全部顶点

        uint8_t  timeType       = 0;   // 时间类型 (0: 连续, 1: 间歇)
        uint64_t timestampStart = 0;   // 起始时间
        uint64_t timestampEnd   = 0;   // 结束时间
        uint8_t  cautionCode    = 0;   // 注意事项
    };

    /**
     * Hydrological and meteorological Information
     */
    struct Hydrometeorology : BusinessAttribute
    {
        uint64_t timestampPublished = 0;

        /*
        * The forecast time is on the hour.
        */
        uint64_t timestampForecast = 0;

        /*
        * Forecast information for meteorological point
        */
        Coordinate coordinate;
        uint64_t   timestampHT = 0;
        uint64_t   timestampLT = 0;

        /*
        * valid range : [-25.0 ~ 25.0]m
        *
        */
        double waterLevel = 0;

        /*
        * Information source
        * value range : [0 - 7]
        * 0 = not available
        * 4 - 7 = reserved
        */
        uint8_t source;
    };

    struct AbnormalShip : BusinessAttribute
    {
        struct CoordinateInfo
        {
            Coordinate coordinate;
            uint64_t   timestampAlert = 0;
        };
        using CoordinateInfos = std::vector<CoordinateInfo>;

        uint32_t mmsi = 0;

        /*
        * Alert type
        * valid range : [0 - 15]
        * 9 - 15 = reserved
        */
        uint8_t alertType = 0;

        /*
        * Alert content
        * 0 - 100 UTF8 characters
        */
        std::string contentAlert;

        /*
        * For the alert type = 4 (duplicate identity), where the same MMSI code appears
        * in different geographic locations, this situation results in the existence of
        * multiple location information for the variable.
        */
        CoordinateInfos coordinateInfos;
    };

    struct Waypoint
    {
        Coordinate coordinate;
        uint64_t   timestamp = 0;
        /*
        * 360 = not available
        */
        double course = 360.0F;
    };

    using Waypoints = std::vector<Waypoint>;

    struct VTSReply : BusinessAttribute
    {
        bool        isAgreed      = false;
        uint32_t    requestDataID = 0;
        uint32_t    mmsiRequestor = 0;
        std::string shipNameChnRequestor;
        std::string shipNameEngRequestor;
        std::string remarkChn;
        std::string remarkEng;
        uint8_t     queueNo = 0;
        Waypoints   waypoints;
    };

    struct VTSRequest : BusinessAttribute
    {
        /*
        * 0 = not available
        * 1 = on line
        * 2 = off line
        * 3 = moor
        * 4 = unberth
        * 5 = Entering lock
        * 6 = Leaving lock
        * 7 = Weighing anchor
        * 8 = Dropping anchor
        * 9 - 15 = reserved
        */
        uint8_t requestType = 0;
        /*
        * -1 = not available
        * 0  = VTS agree
        * 1  = VTS disagree
        */
        int8_t   replyResult    = -1;
        uint64_t timestampReply = 0;
        /*
        * -1 = not available
        * 0  = can be executed
        * 1  = cannot be executed
        */
        int8_t executingStatus = -1;
    };

    /**************************** ASM Information ***********************************/

    struct ASMAttribute
    {
        uint32_t dataID    = 0;
        uint64_t timestamp = 0;
        uint16_t DAC       = 0;
        uint8_t  FI        = 0;
        bool     read      = false;
    };

    /**
     * @brief : Marine Meteorology Forecast(海洋气象预报坐标)
     */
    struct MarineMeteorologyFCST : ASMAttribute
    {
        Coordinate coordinate;
        uint8_t    weatherCode = 0;
        /*
        * valid range : [0 - 120]
        * unit : kn/h
        */
        uint8_t windSpeed = 0;

        /*
        * valid range : [0 - 360]
        * unit : degree
        */
        uint16_t windDirection = 360;

        /*
        * temperature: raw 11-bit complement integer.
        * Resolution: 0.1 °C.
        * Special values: 602 (default / not available)
        */
        int16_t temperature = 602;

        /*
        * airPressure: raw 10-bit integer.
        * Resolution: 0.5 hPa, offset: 800 hPa.
        * Special values: 803 (default / not available)
        */
        uint16_t airPressure = 803;

        /*
        * visibility: raw 8-bit integer.
        * For location #1: resolution 0.1 km. Special values: 252 (default / not available)
        * For incremental records: resolution 0.1 nmile. Special values: 252 (default / not available)
        */
        uint8_t visibility = 252;

        /*
        * valid range : [1 - 3]
        * 0 = not available
        * 4 - 8 = reserved
        */
        uint8_t infoSource = 0;

        uint64_t timestampFCST = 0;
    };

    /**
     * @brief : 海洋气象预报（海区）(新增)
     */
    struct MarineMeteorologyFCSTArea : ASMAttribute
    {
        /*
        * valid range : [0 - 127]
        */
        uint8_t areaCode;

        /*
        * valid range : [0 - 31]
        */
        uint8_t weatherCode = 0;

        /*
        * valid range : [0 - 511]
        * unit : degree
        */
        uint16_t windDirection = 360;

        /*
        * valid range : [0 - 19]
        * 20 = default = not available
        * 21 - 31 = reserved for future use
        */
        uint8_t windScaleLow = 20;

        /*
        * valid range : [0 - 19]
        * 20 = default = not available
        * 21 - 31 = reserved for future use
        */
        uint8_t windScaleHigh = 20;

        /*
        * valid range : [0 - 20] km
        * 21 = greater than 20 km
        * 22 = default = not available
        * 23 - 31 = reserved for future use
        */
        uint8_t visibility = 22;

        uint8_t infoSource = 0;

        uint64_t timestampFCST = 0;
    };

    /**
     * @brief : Marine Environmental Forecast
     */
    struct MarineEnvironmentFCST : ASMAttribute
    {
        Coordinate coordinate;

        /*
        * valid range : [0.0 - 25.0] kn
        * -1.0 = not available
        */
        double flowVelocity = -1.0F;

        /*
        * valid range : [0° - 359°]
        * 360 = not available
        */
        uint16_t flowDirection = 360;

        /*
        * waveHeight: raw 8-bit integer.
        * Resolution: 0.1 m.
        * Special values: 252 (default / not available)
        */
        uint8_t waveHeight = 252;

        /*
        * valid range : [0° - 359°]
        * 360 = not available
        */
        uint16_t waveDirection = 360;

        /*
        * temperature: raw 10-bit integer.
        * Resolution: 0.1 °C, offset: -10.0 °C.
        * Special values: 703 (default / not available)
        */
        uint16_t temperature = 703;

        /*
        * valid range : [1 - 3]
        * 0 = not available
        * 4 - 8 = reserved
        */
        uint8_t infoSource = 0;

        uint64_t timestampFCST = 0;
    };

    /**
     * @brief : 海洋环境预报海区(新增)
     */
    struct MarineEnvironmentFCSTArea : ASMAttribute
    {
        uint8_t areaCode;

        uint16_t temperatureLow = 511;

        uint8_t temperatureHigh = 202;

        uint32_t flowDirectionAvg = 360;

        uint32_t flowDirctionMax = 360;

        uint8_t flowVelocityAvg = 252;

        uint8_t flowVelocityMax = 252;

        uint8_t waveHeight = 252;

        uint8_t swellHeight = 252;

        uint8_t infoSource = 0;

        uint64_t timestampFCST = 0;
    };

    /**
     * @brief : 海洋环境预报近岸 (Along shore)
     */
    struct MarineEnvironmentFCSTAlongshore : ASMAttribute
    {
        uint8_t areaCode;

        uint16_t temperatureLow = 511;

        uint8_t temperatureHigh = 202;

        uint8_t waveHeightLow = 252;

        uint8_t waveHeightHigh = 252;

        uint16_t tideHigh = 1003;

        uint64_t timestampTideHigh;

        uint16_t tideLow = 503;

        uint64_t timestampTideLow;

        uint8_t infoSource = 0;

        uint64_t timestampFCST = 0;
    };

    /**
     * @brief : 桥梁（新增）
     */
    struct Bridge : ASMAttribute
    {
        uint32_t   MRN;
        uint8_t    fragment;
        uint8_t    status;
        Coordinate center;
        double     height           = 0.0;
        double     width            = 0.0;
        uint16_t   directionToPass  = 0;
        uint8_t    passAbility      = 0;
        bool       enableMeeting    = 0;
        bool       enableOvertaking = 0;
        uint16_t   flowVelocity     = 0;
        uint16_t   flowDirection    = 0;
    };

    struct NetSounder : ASMAttribute
    {
        uint32_t MRN;
        uint8_t  fragment;
        uint8_t  type;
        bool     isContinous;

        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : 航道中线 (Navigational Channel Centerline)
     */
    struct ChannelCenterline : ASMAttribute
    {
        uint32_t                MRN      = 0;
        uint8_t                 fragment = 0;
        uint16_t                width    = 0;
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : 航道（航路）边线 (Navigational Channel Boundary)
     */
    struct ChannelBoundary : ASMAttribute
    {
        uint32_t                MRN      = 0;
        uint8_t                 fragment = 0;
        uint8_t                 edgeType = 0;   // 0: Left boundary, 1: Right boundary
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : Frontend Prompt Text (前端提示文字)
     */
    struct FrontendPrompt : ASMAttribute
    {
        uint8_t     promptContent;
        uint8_t     displayDuration;
        std::string text;
    };

    /**
     * @brief : Marine Meteorology early warning
     */
    struct MarineMeteorologyEW : ASMAttribute
    {
        struct WarningElement
        {
            uint32_t MRN         = 0;
            uint8_t areaCode     = 0;     // seaAreaCode, cityCode, or regionCode
            uint8_t warningLevel = 0;     // warningLevel
            double  surgeHeight  = 0.0;   // surge height in meters (only for Storm Surge)
        };

        struct TropicalCyclonePathPoint
        {
            uint64_t timestamp         = 0;
            double   centerLongitude   = 181.0;
            double   centerLatitude    = 91.0;
            uint8_t  cycloneType       = 0;
            uint16_t radiusWindScale7  = 0;
            uint16_t radiusWindScale10 = 0;
            uint16_t radiusWindScale12 = 0;
            uint8_t  moveSpeed         = 0;
            uint16_t moveDirection     = 360;
            uint8_t  maxWindScale      = 20;
            uint16_t centerPressure    = 403;
        };

        uint32_t MRN         = 0;
        uint8_t  fragment    = 0;
        uint8_t  warningType = 0;   // 1: Cyclone, 2: Wind, 3: Wave, 4: Sea Fog, 5: Storm Surge, 6: Ice

        std::vector<TropicalCyclonePathPoint> cyclonePathPoints;
        std::vector<WarningElement> elements;   // For types 2, 3, 4, 5, 6

        uint64_t timestampPublished = 0;
        uint64_t timestampStart     = 0;
        uint64_t timestampEnd       = 0;
        uint8_t  warningDuration    = 0;
        uint8_t  infoSource         = 0;
    };

    /**
     * @brief : Tide forecast DTO
     */
    struct TideForecast : ASMAttribute
    {
        struct TideStation
        {
            Coordinate coordinate;
            double     tidalDatum        = 0.0;
            double     tideHigh          = 0.0;
            uint64_t   timestampTideHigh = 0;
            double     tideLow           = 0.0;
            uint64_t   timestampTideLow  = 0;
        };

        uint8_t                  hourPublish = 24;
        uint8_t                  infoSource  = 0;
        std::vector<TideStation> stations;
    };

    /**
     * @brief : Aids to Navigation (AtoN) dynamics (non-AIS AtoN) DTO
     */
    struct AtoNDynamics : ASMAttribute
    {
        struct Element
        {
            uint32_t   MRN  = 0;
            uint8_t    type = 0;
            Coordinate coordinate;

            // Newly established & light character changed specifics
            uint8_t rhythmNameCode  = 0;
            uint8_t rhythmParamCode = 0;
            uint8_t bodyColor       = 0;
            uint8_t lightColor      = 0;
            uint8_t lightPeriod     = 0;
            uint8_t range           = 0;
            uint8_t morseCode       = 0;

            // Relocated specifics
            Coordinate prevCoordinate;
            bool       isRoughPosition = false;

            // Failure specifics
            uint8_t failureType = 0;

            // Temporary operations specifics
            uint64_t timestampStart = 0;
            uint64_t timestampEnd   = 0;
        };

        uint32_t             MRN        = 0;
        uint8_t              fragment   = 0;
        uint8_t              status     = 0;
        uint8_t              precaution = 0;
        std::vector<Element> elements;
        std::string          description;
    };

    /**
     * @brief : AIS Aids to Navigation (AIS AtoN) dynamics DTO
     */
    struct AISAtoNDynamics : ASMAttribute
    {
        struct Element
        {
            uint32_t   mmsi = 0;
            uint32_t   MRN  = 0;
            uint8_t    type = 0;
            Coordinate coordinate;

            // Newly established specifics
            uint8_t range = 0;

            // Relocated specifics
            Coordinate prevCoordinate;
            bool       isRoughPosition = false;
        };

        uint32_t             MRN           = 0;
        uint8_t              fragment      = 0;
        uint8_t              atonAttribute = 0;
        uint8_t              status        = 0;
        uint8_t              precaution    = 0;
        std::vector<Element> elements;
    };

    /**
     * @brief : Route Recommendation Request DTO (DAC 412, FI 46)
     */
    struct RouteRecommendationRequest
    {
        uint32_t   dataID         = 0;     // Database record ID
        uint64_t   timestampSent  = 0;     // Sent timestamp
        uint32_t   sequenceNum    = 0;     // Rolling sequence number
        int32_t    sendStatus     = -1;    // Send status (-1: pending; 0-8: AMK ack)
        uint32_t   grossTonnage   = 0;     // Total Tonnage
        double     maxStaticDraft = 0.0;   // Maximum Static Draft (in meters)
        uint8_t    cargoType      = 0;     // Cargo Type
        uint8_t    month          = 0;     // Departure Month
        uint8_t    day            = 0;     // Departure Day
        uint8_t    hour           = 0;     // Departure Hour
        uint8_t    minute         = 0;     // Departure Minute
        Coordinate startCoordinate;        // Start Coordinate
        Coordinate destCoordinate;         // Destination Coordinate

        bool Validate(std::string &errorMsg) const
        {
            if (month > 12)
            {
                return false;
            }
            if (day > 31)
            {
                return false;
            }
            if (hour > 24)
            {
                return false;
            }
            if (minute > 60)
            {
                return false;
            }
            if (!startCoordinate.IsValid())
            {
                return false;
            }
            if (!destCoordinate.IsValid())
            {
                return false;
            }
            return true;
        }
    };

    /**
     * @brief : Route Recommendation Response DTO (DAC 412, FI 46)
     */
    struct RouteRecommendationResponse : ASMAttribute
    {
        double                  effectiveTime = 31.5; // Lifecycle, unit 0.5h
        std::vector<Coordinate> coordinates;
    };

    /**
     * @brief : Hydrometeorological Request DTO (DAC 412, FI 48)
     */
    struct HydrometeorologyRequest
    {
        uint32_t                dataID = 0;           // Database record ID
        uint64_t                timestampSent = 0;    // Sent timestamp
        uint32_t                sequenceNum = 0;      // Rolling sequence number
        int32_t                 sendStatus = -1;      // Send status (-1: pending; 0-8: AMK ack)
        uint32_t                MRN = 0;              // Range: 1~131071, 0 is default
        std::vector<Coordinate> coordinates;          // Limit: max 10 points
        
        bool                    windSpeed = false;
        bool                    windDirection = false;
        bool                    visibility = false;
        bool                    waveHeight = false;
        bool                    waveDirection = false;
        bool                    swellHeight = false;
        
        // Requested time (UTC epoch seconds, 0 represents default/unavailable)
        uint64_t                requestTime = 0;

        bool Validate(std::string &errorMsg) const
        {
            if (MRN > 131071)
            {
                errorMsg = "MRN exceeds maximum value 131071.";
                return false;
            }
            if (coordinates.empty())
            {
                errorMsg = "Coordinates list cannot be empty.";
                return false;
            }
            if (coordinates.size() > 10)
            {
                errorMsg = "Coordinates list exceeds maximum size of 10 points.";
                return false;
            }
            for (size_t i = 0; i < coordinates.size(); ++i)
            {
                if (!coordinates[i].IsValid())
                {
                    errorMsg = "Coordinates contain invalid points.";
                    return false;
                }
            }
            return true;
        }
    };
    /**
     * @brief : Hydrometeorological Response DTO (DAC 412, FI 49)
     */
    struct HydrometeorologyResponse : ASMAttribute
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
     * @brief : Extended own vessel static and voyage info
     */
    struct ExtendedVesselInfo
    {
        uint8_t     extendedVesselType = 0;       // 8 bits (Table 147)
        uint8_t     autonomousLevel    = 0;       // 3 bits (Table 147)
        std::string portCode;                     // 30 bits, 5 characters of 6-bit ASCII
        std::string chineseName;                  // n bits, 14-bit Chinese encoding
    };

    /**
     * @brief : Extended static and voyage info received from other vessels
     */
    struct OtherVesselExtendedInfo : ASMAttribute
    {
        uint32_t    mmsi = 0;
        uint8_t     extendedVesselType = 0;
        uint8_t     autonomousLevel = 0;
        uint8_t     naviStatus = 15;
        double      draught = 0.0;
        uint64_t    eta = 0;
        std::string portCode;
        uint16_t    crewNum = 0;
        std::string chineseName;
    };

}   // namespace VDES

#endif