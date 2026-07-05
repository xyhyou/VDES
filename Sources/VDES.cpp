#include "VDES.h"

#include <map>
#include <array>
#include <mutex>
#include <string>
#include <vector>
#include <cstdio>
#include <functional>
#include <algorithm>

#include "VDESConfigure.h"
#include "AISBitsManager.h"
#include "UtilityInterface.h"
#include "ASMManager.h"
#include "VDESDTD.h"
#include "spdlog/fmt/fmt.h"
#include "GeoCalculator.h"
#include "spdlog/spdlog.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include "TimerManager.h"

namespace VDES
{
    class VDESManager::Impl
    {
    public:
        Impl(VDESManager *parent);

        ~Impl();

        std::string BuildSQLCmd(const std::string &dataTableName, const std::string &bboxTableName, 
                                const BoundingBox &bbox);

        void DatabaseErrorProcess(const SQLite::Exception &exception, const std::string &invoker);

        void InitializeMessageTable(void);

        void InitializeAtoNTable(void);

        void InitializeMSIObstacleTable(void);

        void InitializeMSIMaritineOperationTable(void);

        void InitializeMSIMilitaryActivityTable(void);

        void InitializeMSIMaritimeDistressTable(void);

        void InitializeMSIMaritimeTowingTable(void);

        void InitializeMSIDesignatedAreaTable(void);

        void InitializeBridgeTable(void);

        void InitializeChannelCenterlineTable(void);

        void InitializeChannelBoundaryTable(void);

        void InitializeNetSounderTable(void);

        void InitializeFrontendPromptTable(void);

        void InitializeHydrometeorologyTable(void);

        void InitializeAbnormalShipTable(void);

        void InitializeMarineMeteorologyFCSTTable(void);

        void InitializeMarineEnvironmentFCSTTable(void);

        void InitializeVTSRequestTable(void);

        void InitializeVTSReplyTable(void);

        void InitializeDatabase(void);

        double DecodeCoordinate(const AISBitsManager &manager, const uint32_t startBitPos,
            const uint32_t bitsNum);

        uint64_t DecodeTime(const AISBitsManager &manager, const uint32_t startBitPos,
            const uint32_t bitsNum);

        enum class MessageBusinessType
        {
            NONE,
            SIX_BIT_ASCII,
            UTF8,
            FOURTEEN_BIT,
        };

        void BuildMessage(const uint32_t mmsiSource, const uint32_t mmsiDest,
                          const MessageBusinessType &messageType, const bool needReply,
                          const uint16_t sequenceNumber,
                          const uint64_t deadline = 0);

        void UpdateSentBusinessStatus(SendStatus status, 
            const SendStatusCode statusCode = SendStatusCode::NONE);
        
        uint32_t UpdateSentVTSRequest(const uint16_t sequenceNo, const uint8_t replyResult);

        void LoadInboxFromQueryResult(Message &message, const SQLite::Statement &query);

        void LoadOutboxFromQueryResult(Message &message, const SQLite::Statement &query);

        void LoadAtoNFromQueryResult(AtoN &aton, const SQLite::Statement &query);

        void LoadMSIObstacleFromQueryResult(MSIObstacle &obstacle, const SQLite::Statement &query);

        void LoadMSIMaritineOperationFromQueryResult(MSIMaritimeOperation &operation,
            const SQLite::Statement &query);

        void LoadMSIMilitaryActivityFromQueryResult(MSIMilitaryActivity &activity,
            const SQLite::Statement &query);

        void LoadMSIMaritimeDistressFromQueryResult(MSIMaritimeDistress &distress,
            const SQLite::Statement &query);

        void LoadMSIMaritimeTowingFromQueryResult(MSIMaritimeTowing &towing,
            const SQLite::Statement &query);

        void LoadMSIDesignatedAreaFromQueryResult(MSIDesignatedArea &area,
            const SQLite::Statement &query);

        void LoadBridgeFromQueryResult(Bridge &bridge,
            const SQLite::Statement &query);

        void LoadChannelCenterlineFromQueryResult(ChannelCenterline &centerline,
            const SQLite::Statement &query);

        void LoadChannelBoundaryFromQueryResult(ChannelBoundary &boundary,
            const SQLite::Statement &query);

        void LoadNetSounderFromQueryResult(NetSounder &netSounder,
            const SQLite::Statement &query);

        void LoadFrontendPromptFromQueryResult(FrontendPrompt &prompt,
            const SQLite::Statement &query);

        void LoadHydrometeorologyFromQueryResult(Hydrometeorology &hydro,
            const SQLite::Statement &query);

        void LoadAbnormalShipFromQueryResult(AbnormalShip &ship, 
            const SQLite::Statement &query);

        void LoadMarineMeteorologyFCSTFromQueryResult(MarineMeteorologyFCST &fcst,
            const SQLite::Statement &query);

        void LoadMarineEnvironmentFCSTFromQueryResult(MarineEnvironmentFCST &fcst,
            const SQLite::Statement &query);

        void LoadVTSRequestFromQueryResult(VTSRequest &request, const SQLite::Statement &query);

        void LoadVTSReplyFromQueryResult(VTSReply &reply, const SQLite::Statement &query);

        uint32_t SaveMessage(const MailBox mailbox, const Message &message,
                         const MessageBusinessType &messageType, const uint32_t sequenceNumber);

        void SaveAtoN(const AtoN &aton, const bool isVDM = false);

        void DeleteAtoN(const uint32_t mmsi);

        void SaveMSIObstacle(const MSIObstacle &obstacle);

        void SaveMSIMaritineOperation(const MSIMaritimeOperation &operation);

        void SaveMSIMilitaryActivity(const MSIMilitaryActivity &activity);

        void SaveMSIMaritimeDistress(const MSIMaritimeDistress &distress);

        void SaveMSIMaritimeTowing(const MSIMaritimeTowing &towing);

        void SaveMSIDesignatedArea(const MSIDesignatedArea &area);

        void SaveBridge(const Bridge &bridge);

        void SaveChannelCenterline(const ChannelCenterline &centerline);

        void SaveChannelBoundary(const ChannelBoundary &boundary);

        void SaveNetSounder(const NetSounder &netSounder);

        void SaveFrontendPrompt(const FrontendPrompt &prompt);

        void SaveHydrometeoroloies(const Hydrometeorologies &container);

        void SaveAbnormalShip(const AbnormalShip &ship);

        void SaveMarineMeteorologyFCST(const MarineMeteorologyFCSTs &container);

        void SaveMarineEnvironmentFCST(const MarineEnvironmentFCSTs &container);

        void SaveMarineMeteorologyEW(const MarineMeteorologyEW &ew);

        void InitializeMarineMeteorologyEWTable(void);

        void LoadMarineMeteorologyEWFromQueryResult(MarineMeteorologyEW &ew, const SQLite::Statement &query);

        void SaveTideForecast(const TideForecast &forecast);

        void InitializeTideForecastTable(void);

        void LoadTideForecastFromQueryResult(TideForecast &forecast, const SQLite::Statement &query);

        void SaveAtoNDynamics(const AtoNDynamics &dynamics);

        void InitializeAtoNDynamicsTable(void);

        void LoadAtoNDynamicsFromQueryResult(AtoNDynamics &dynamics, const SQLite::Statement &query);

        void SaveAISAtoNDynamics(const AISAtoNDynamics &dynamics);

        void InitializeAISAtoNDynamicsTable(void);

        void LoadAISAtoNDynamicsFromQueryResult(AISAtoNDynamics &dynamics, const SQLite::Statement &query);

        void SaveVTSRequest(const VTSRequest &request);

        void SaveVTSReply(const VTSReply &reply, const uint16_t sequenceNo);

        void SaveRouteRecommendationResponse(const RouteRecommendationResponse &response);
        
        void InitializeRouteRecommendationResponseTable(void);
        
        void LoadRouteRecommendationResponseFromQueryResult(RouteRecommendationResponse &response, const SQLite::Statement &query);

        void SaveHydrometeorologyResponse(const HydrometeorologyResponse &response);
        
        void InitializeHydrometeorologyResponseTable(void);
        
        void LoadHydrometeorologyResponseFromQueryResult(HydrometeorologyResponse &response, const SQLite::Statement &query);

        void SaveHydrometeorologyRequest(const HydrometeorologyRequest &request, uint32_t seqNo);
        void InitializeHydrometeorologyRequestTable(void);

        void InitializeRouteRecommendationRequestTable(void);
        
        void SaveRouteRecommendationRequest(const RouteRecommendationRequest &request, uint32_t seqNo);

        void ParseAMK(const std::string &sentence);
        
        void UpdateAABRequestStatus(uint32_t seqNo, uint8_t ackType);

        void InitializeOwnExtendedVesselInfoTable(void);
        
        void InitializeOtherVesselExtendedInfoTable(void);
        
        void SaveOwnExtendedVesselInfo(const ExtendedVesselInfo &info);
        
        void SaveOtherVesselExtendedInfoPartA(const ASM_DAC_412_FI_50 &partA);
        
        void SaveOtherVesselExtendedInfoPartB(const ASM_DAC_412_FI_51 &partB);
        
        void LoadOtherVesselExtendedInfoFromQueryResult(OtherVesselExtendedInfo &info, const SQLite::Statement &query);
        
        void SendExtendedVesselInfoPartA(void);
        
        void SendExtendedVesselInfoPartB(void);
        
        void BroadcastExtendedVesselInfo(void);

        void SaveMarineMeteorologyFCSTArea(const MarineMeteorologyFCSTAreas &container);
        void InitializeMarineMeteorologyFCSTAreaTable(void);
        void LoadMarineMeteorologyFCSTAreaFromQueryResult(MarineMeteorologyFCSTArea &area, const SQLite::Statement &query);

        void SaveMarineEnvironmentFCSTArea(const MarineEnvironmentFCSTAreas &container);
        void InitializeMarineEnvironmentFCSTAreaTable(void);
        void LoadMarineEnvironmentFCSTAreaFromQueryResult(MarineEnvironmentFCSTArea &area, const SQLite::Statement &query);

        void SaveMarineEnvironmentFCSTAlongshore(const MarineEnvironmentFCSTAlongshores &container);
        void InitializeMarineEnvironmentFCSTAlongshoreTable(void);
        void LoadMarineEnvironmentFCSTAlongshoreFromQueryResult(MarineEnvironmentFCSTAlongshore &area, const SQLite::Statement &query);

        void ParseOneLineData(const std::string &sentence);

        void ParseVDOVDM(const std::string &sentence);

        void ParseVEM(const std::string &sentence);

        void ParseTMK(const std::string &sentence);

        void ParseTSK(const std::string &sentence);

        void ParseABK(const std::string &sentence);

        void ParseTDB(const std::string &sentence);

        void ParseTSB(const std::string &sentence);

        void ParseAISMessage6(const AISBitsManager &manager);

        void ParseAISMessage8(AISBitsManager &manager);

        void ParseAISMessage12(const AISBitsManager &manager);

        void ParseAISMessage14(const AISBitsManager &manager);

        void ParseAISMessage21(const AISBitsManager &manager);

        void ParseBusinessSixBitASCIIMessage(const uint32_t mmsiSource,
            const uint32_t mmsiDest, const AISBitsManager &manager);

        void ParseBusinessUTF8Message(const uint32_t mmsiSource,
            const uint32_t mmsiDest, const AISBitsManager &manager);

        void ParseBusiness14BitMessage(const uint32_t mmsiSource,
            const uint32_t mmsiDest, const AISBitsManager &manager);

        void ParseBusinessMessageConfirmation(const uint32_t mmsiSource,
            const uint32_t mmsiDest, const AISBitsManager &manager);

        void ParseBusinessHydrometeorology(const uint32_t mmsiSource,
            const uint32_t mmsiDest, const AISBitsManager &manager);

        void ParseBusinessAtoN(const uint32_t mmsiSource, const uint32_t mmsiDest, 
            const AISBitsManager &manager);

        void ParseBusinessMSI(const uint32_t mmsiSource, const uint32_t mmsiDest,
            const AISBitsManager &manager);

        void ParseMSIObstacle(const AISBitsManager &manager);

        void ParseMSIMaritimeOperations(const AISBitsManager &manager);

        void ParseMSIMilitaryActivity(const AISBitsManager &manager);

        void ParseMSIMaritimeDistress(const AISBitsManager &manager);

        void ParseBusinessAbnormalShip(const uint32_t mmsiSource, const uint32_t mmsiDest,
            const AISBitsManager &manager);

        void ParseBusinessVTSReply(const uint32_t mmsiSource, const uint32_t mmsiDest,
            const AISBitsManager &manager);

        void HandleASMMessage(ASMManager::ASMPtr asmData);

        void SendAISMessage6(const uint32_t mmsiDestination, const std::string &content);

        void SendAISMessage8(const std::string &content);

        void SendAISMessage12(const uint32_t mmsiDestination, const std::string &content);

        void SendAISMessage14(const std::string &content);

        void HandleConfigureEvent(ConfigureManager::EventType type, const int retCode);

    public:
        static const uint32_t BUFFER_SIZE = 4096;

        struct CoordinateStruct
        {
            double latitude;
            double longitude;
        };

        struct WaypointStruct
        {
            CoordinateStruct coordinate;
            uint64_t         timestamp; 
            double           course;
        };

        enum class TransmissionBusiness
        {
            NONE = 0,
            SIX_BIT_ASCII,
            UTF8,
            FOURTEEN_BIT,
            ADDRESS_MESSAGE_CONFIRM,
            VTS_REQUEST,
            VTS_REPLY,
            VTS_CONFIRM,
        };

        enum class ExecutingStatus
        {
            NONE = -1,  
            CAN_EXECUTE = 0,
            CANNOT_EXECUTE = 1
        };

        using Array = std::array<char, BUFFER_SIZE>;
        using ParsersMap = std::map<std::string, std::function<void(const std::string &)>>;
        using BusinessMap = std::map<uint16_t, std::function<void(const uint32_t &mmsiSource, 
                                const uint32_t &mmsiDest, const AISBitsManager &)>>;
        using AISParserMap = std::map<uint8_t, std::function<void(AISBitsManager &)>>;
        using MessageMap = std::map<std::string, std::map<uint16_t, std::string>>;
        using AtoNMap = std::map<uint32_t, AtoN>;
        using DatabasePtr = std::unique_ptr<SQLite::Database>;

        VDESManager             *m_parent;
        Array                   m_buffer;
        uint32_t                m_pos;
        ParsersMap              m_parsersMap;
        BusinessMap             m_businessMap;
        MessageMap              m_messageMap;
        AISParserMap            m_aisParserMap;
        ASMManager              m_asmManager;

        AtoNMap                 m_atonMap;
        std::mutex              m_mutexAtoN;
        DatabasePtr             m_database;
        uint32_t                m_version;

        mutable std::mutex      m_mutexMessageInbox;
        mutable std::mutex      m_mutexMessageOutbox;
        std::mutex              m_mutexMSIObstacle;
        std::mutex              m_mutexMSIMaritimeOperation;
        std::mutex              m_mutexMSIMilitaryActivity;
        std::mutex              m_mutexMSIMaritimeDistress;
        std::mutex              m_mutexMSIMaritimeTowing;
        std::mutex              m_mutexMSIDesignatedArea;
        std::mutex              m_mutexBridge;
        std::mutex              m_mutexChannelCenterline;
        std::mutex              m_mutexChannelBoundary;
        std::mutex              m_mutexNetSounder;
        std::mutex              m_mutexFrontendPrompt;
        std::mutex              m_mutexHydrometeorology;
        std::mutex              m_mutexAbnormalShip;
        std::mutex              m_mutexMarineMeteorologyFCST;
        std::mutex              m_mutexMarineEnvironmentFCST;
        std::mutex              m_mutexMarineMeteorologyEW;
        std::mutex              m_mutexTideForecast;
        std::mutex              m_mutexAtoNDynamics;
        std::mutex              m_mutexAISAtoNDynamics;
        std::mutex              m_mutexVTSRequest;
        std::mutex              m_mutexVTSReply;
        std::mutex              m_mutexRouteRecommendationResponse;
        std::mutex              m_mutexHydrometeorologyResponse;
        std::mutex              m_mutexOwnExtendedVesselInfo;
        std::mutex              m_mutexOtherVesselExtendedInfo;
        std::mutex              m_mutexMarineMeteorologyFCSTArea;
        std::mutex              m_mutexMarineEnvironmentFCSTArea;
        std::mutex              m_mutexMarineEnvironmentFCSTAlongshore;

        uint32_t                m_timerSendMessage;
        uint32_t                m_dataIDLastSentMessage;
        uint32_t                m_timerSendAISMessage;
        uint32_t                m_dataIDLastSentAISMessage;
        uint32_t                m_dataIDLastSentVTSRequest;
        uint32_t                m_textSequenceNoVTS;
        uint32_t                m_textSequenceNoSixBitASCII;
        uint32_t                m_textSequenceNoUTF8;
        uint32_t                m_textSequenceNoFourteenBit;
        uint32_t                m_sequenceNoAAB;
        uint32_t                m_timerExtendedVesselBroadcast;
        uint32_t                m_businessIDToBeConfirmed;
        ReplyStatus             m_replyStatus;
        ExecutingStatus         m_executingStatus;
        TransmissionBusiness    m_transmissionBusiness;

        MarineMeteorologyFCSTAreas m_meteorologyAreas;
		MarineEnvironmentFCSTAreas m_environmentAreas;
        MarineEnvironmentFCSTAlongshores m_environmentAlongshores;
        Bridges m_bridges;
    };

    VDESManager::Impl::Impl(VDESManager *parent) : m_pos(0), m_buffer(), 
        m_parent(parent)
    {
        m_timerSendMessage = 0;
        m_timerSendAISMessage = 0;
        m_dataIDLastSentMessage = 0;
        m_dataIDLastSentVTSRequest = 0;
        m_dataIDLastSentAISMessage = 0;
        m_textSequenceNoVTS = 0;
        m_textSequenceNoSixBitASCII = 0;
        m_textSequenceNoUTF8 = 0;
        m_textSequenceNoFourteenBit = 0;
        m_sequenceNoAAB = 0;
        m_timerExtendedVesselBroadcast = 0;

        m_businessIDToBeConfirmed = 0;

        m_transmissionBusiness = TransmissionBusiness::NONE;
        m_executingStatus = ExecutingStatus::NONE;

        m_parsersMap.insert(std::make_pair("ASM", std::bind(&ASMManager::Parse, &this->m_asmManager, std::placeholders::_1)));

        m_parsersMap.insert(std::make_pair("ADM", std::bind(&ASMManager::Parse, &this->m_asmManager, std::placeholders::_1)));

        m_parsersMap.insert(std::make_pair("AMK", std::bind(&VDESManager::Impl::ParseAMK, this, std::placeholders::_1)));

        m_parsersMap.insert(std::make_pair("VDM", std::bind(&Impl::ParseVDOVDM, this, std::placeholders::_1)));

        m_parsersMap.insert(std::make_pair("VDO", std::bind(&Impl::ParseVDOVDM, this, std::placeholders::_1)));

        m_parsersMap.insert(std::make_pair("ABK", std::bind(&Impl::ParseABK, this, std::placeholders::_1)));

        m_parsersMap.insert(std::make_pair("VEM", std::bind(&Impl::ParseVEM, this, std::placeholders::_1)));
        
        m_parsersMap.insert(std::make_pair("TMK", std::bind(&Impl::ParseTMK, this, std::placeholders::_1)));
        
        m_parsersMap.insert(std::make_pair("TSK", std::bind(&Impl::ParseTSK, this, std::placeholders::_1)));
#ifdef ENABLE_PARSE_TDB
        m_parsersMap.insert(std::make_pair("TDB", std::bind(&Impl::ParseTDB, this, std::placeholders::_1)));

        m_parsersMap.insert(std::make_pair("TSB", std::bind(&Impl::ParseTSB, this, std::placeholders::_1)));
#endif
        /*********************** AIS message parsers ***********************/
        m_aisParserMap.insert(std::make_pair(6, std::bind(&Impl::ParseAISMessage6, this, std::placeholders::_1)));

        m_aisParserMap.insert(std::make_pair(8, std::bind(&Impl::ParseAISMessage8, this, std::placeholders::_1)));

        m_aisParserMap.insert(std::make_pair(12, std::bind(&Impl::ParseAISMessage12, this, std::placeholders::_1)));

        m_aisParserMap.insert(std::make_pair(14, std::bind(&Impl::ParseAISMessage14, this, std::placeholders::_1)));

        m_aisParserMap.insert(std::make_pair(21, std::bind(&Impl::ParseAISMessage21, this, std::placeholders::_1)));

        /*********************** Business message parsers ***********************/
        m_businessMap.insert(std::make_pair(0, std::bind(&Impl::ParseBusinessSixBitASCIIMessage, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)));

        m_businessMap.insert(std::make_pair(1, std::bind(&Impl::ParseBusinessUTF8Message, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)));

        m_businessMap.insert(std::make_pair(2, std::bind(&Impl::ParseBusinessMessageConfirmation, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)));

        m_businessMap.insert(std::make_pair(3, std::bind(&Impl::ParseBusinessAtoN, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)));

        m_businessMap.insert(std::make_pair(7, std::bind(&Impl::ParseBusinessVTSReply, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)));

        m_businessMap.insert(std::make_pair(8, std::bind(&Impl::ParseBusinessMSI, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)));
        
        m_businessMap.insert(std::make_pair(9, std::bind(&Impl::ParseBusinessHydrometeorology, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)));

        m_businessMap.insert(std::make_pair(10, std::bind(&Impl::ParseBusinessAbnormalShip, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)));

        m_businessMap.insert(std::make_pair(11, std::bind(&Impl::ParseBusiness14BitMessage, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)));

        auto func = std::bind(&VDESManager::Impl::HandleASMMessage, this, std::placeholders::_1);
        m_asmManager.asmNotify.append(func);
    }

    VDESManager::Impl::~Impl()
    {
    }

    std::string VDESManager::Impl::BuildSQLCmd(const std::string &dataTableName, 
        const std::string &bboxTableName, const BoundingBox &bbox)
    {
        std::string sqlCmd;
        auto        rects = bbox.GetRects();
        auto        rectNum = rects.size();

        if (rectNum > 0)
        {
            for (auto i = 0U; i < rectNum; i++)
            {
                auto &rect = rects[i];
                auto positive = (rect.left == -180 && rectNum == 2) ? false : true;
                auto strTemp = fmt::format("SELECT {DataTable}.* FROM {DataTable}, {BBoxTable} "
                    "WHERE {DataTable}.ID = {Flag}{BBoxTable}.ID AND {BBoxTable}.Left <= {Right} "
                    "AND {BBoxTable}.Right >= {Left} AND {BBoxTable}.Bottom <= {Top} "
                    "AND {BBoxTable}.Top >= {Bottom}",
                    fmt::arg("DataTable", dataTableName),
                    fmt::arg("Flag", positive ? "" : "-"),
                    fmt::arg("BBoxTable", bboxTableName),
                    fmt::arg("Right", rect.right),
                    fmt::arg("Left", rect.left),
                    fmt::arg("Bottom", rect.bottom),
                    fmt::arg("Top", rect.top));
                
                sqlCmd.append(strTemp);
                if (i < rectNum - 1 && rectNum > 1)
                {
                    sqlCmd.append(" UNION ");
                }
            }
        }
        return sqlCmd;
    }

    void VDESManager::Impl::DatabaseErrorProcess(const SQLite::Exception &exception, 
        const std::string &invoker)
    {
        SPDLOG_DEBUG("Invoker = {0}, error code = {1}, extended error code = {2} : {3}",
            invoker, exception.getErrorCode(), exception.getExtendedErrorCode(),
            exception.what());

    }

    void VDESManager::Impl::InitializeMessageTable(void)
    {
        try
        {
            if (!m_database->tableExists("MessageInbox"))
            {
                auto sql = fmt::format("CREATE TABLE MessageInbox ("
                    "ID                      INTEGER   PRIMARY KEY,"
                    "Source                  INT       NOT NULL DEFAULT 0,"
                    "Destination             INT       NOT NULL DEFAULT 0,"
                    "Type                    INT       NOT NULL DEFAULT 0,"
                    "[Text Sequence Number]  INT       NOT NULL DEFAULT 0,"
                    "Content                 TEXT,"
                    "[Need Ack]              BOOLEAN   NOT NULL DEFAULT 0,"
                    "Valid                   BOOLEAN   NOT NULL DEFAULT 0,"
                    "Certified               BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp Deadline]    INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");
                m_database->exec(sql);

                if (!m_database->indexExists("InboxTimeRcvIndex", "MessageInbox"))
                {
                    auto sql = fmt::format("CREATE INDEX InboxTimeRcvIndex ON MessageInbox ([Timestamp Receive])");
                    m_database->exec(sql);
                }
            }

            if (!m_database->fieldExists("[Timestamp Deadline]", "MessageInbox"))
            {
                auto sql = fmt::format("ALTER TABLE MessageInbox ADD [Timestamp Deadline] INTEGER NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }

            if (!m_database->fieldExists("Read", "MessageInbox"))
            {
                auto sql = fmt::format("ALTER TABLE MessageInbox ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }

            if (!m_database->tableExists("MessageOutbox"))
            {
                auto sql = fmt::format("CREATE TABLE MessageOutbox ("
                    "ID                      INTEGER   PRIMARY KEY,"
                    "Source                  INT       NOT NULL DEFAULT 0,"
                    "Destination             INT       NOT NULL DEFAULT 0,"
                    "Type                    INT       NOT NULL DEFAULT 0,"
                    "[Text Sequence Number]  INT       NOT NULL DEFAULT 0,"
                    "Content                 TEXT,"
                    "[Send Status]           INT       NOT NULL DEFAULT 0,"
                    "[Status Code]           INT       NOT NULL DEFAULT -1,"
                    "[Need Ack]              BOOLEAN   NOT NULL DEFAULT 0,"
                    "Valid                   BOOLEAN   NOT NULL DEFAULT 0,"
                    "Certified               BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Timestamp Send]        INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp Deadline]    INTEGER   NOT NULL DEFAULT 0)");
                m_database->exec(sql);

                if (!m_database->indexExists("OutboxTimeSendIndex", "MessageOutbox"))
                {
                    auto sql = fmt::format("CREATE INDEX OutboxTimeSendIndex ON MessageOutbox ([Timestamp Send])");
                    m_database->exec(sql);
                }
            }

            if (!m_database->fieldExists("[Timestamp Deadline]", "MessageOutbox"))
            {
                auto sql = fmt::format("ALTER TABLE MessageOutbox ADD [Timestamp Deadline] INTEGER NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMessageTable");
        }
    }

    void VDESManager::Impl::InitializeAtoNTable(void)
    {
        try
        {
            if (!m_database->tableExists("AtoNs"))
            {
                auto sql = fmt::format("CREATE TABLE AtoNs ("
                    "ID                      INTEGER   PRIMARY KEY,"
                    "Latitude                INT       NOT NULL DEFAULT 91,"
                    "Longitude               INT       NOT NULL DEFAULT 181,"
                    "Type                    INT       NOT NULL DEFAULT 0,"
                    "Status                  INT       NOT NULL DEFAULT 0,"
                    "Virtual                 BOOLEAN   NOT NULL DEFAULT 0,"
                    "OffPosition             BOOLEAN   NOT NULL DEFAULT 0,"
                    "Assigned                BOOLEAN   NOT NULL DEFAULT 0,"
                    "Certified               BOOLEAN   NOT NULL DEFAULT 0,"
                    "Name                    TEXT,"
                    "Timestamp               INTEGER   NOT NULL DEFAULT 0,"
                    "Deadline                INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");

                m_database->exec(sql);

                if (!m_database->tableExists("AtoNBBox"))
                {
                    m_database->exec("CREATE VIRTUAL TABLE AtoNBBox USING rtree(ID, Left, Right, Bottom, Top)");
                }
                if (!m_database->indexExists("AtoNTimeIndex", "AtoNs"))
                {
                    m_database->exec("CREATE INDEX AtoNTimeIndex ON AtoNs (Timestamp)");
                }
            }

            if (!m_database->fieldExists("Deadline", "AtoNs"))
            {
                auto sql = fmt::format("ALTER TABLE AtoNs ADD Deadline INTEGER NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }

            if (!m_database->fieldExists("Read", "AtoNs"))
            {
                auto sql = fmt::format("ALTER TABLE AtoNs ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeAtoNTable");
        }
    }

    void VDESManager::Impl::InitializeMSIObstacleTable(void)
    {
        try
        {
            if (!m_database->tableExists("MSIObstacle"))
            {
                auto sql = fmt::format("CREATE TABLE MSIObstacle ("
                    "ID                      INTEGER   PRIMARY KEY,"
                    "Type                    INT       NOT NULL DEFAULT 0,"
                    "Latitude                INT       NOT NULL DEFAULT 91,"
                    "Longitude               INT       NOT NULL DEFAULT 181,"
                    "Range                   DOUBLE    NOT NULL DEFAULT 0.0,"
                    "[Timestamp Start]       INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp End]         INTEGER   NOT NULL DEFAULT 0,"
                    "Comment                 BOOLEAN   NOT NULL DEFAULT 0,"
                    "Certified               BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");
                    
                m_database->exec(sql);

                if (!m_database->indexExists("ObstacleTimeIndex", "MSIObstacle"))
                {
                    m_database->exec("CREATE INDEX ObstacleTimeIndex ON MSIObstacle ([Timestamp Receive])");
                }

                if (!m_database->tableExists("MSIObstacleBBox"))
                {
                    m_database->exec("CREATE VIRTUAL TABLE MSIObstacleBBox USING rtree(ID, Left, Right, Bottom, Top)");
                }
            }

            if (!m_database->fieldExists("Read", "MSIObstacle"))
            {
                auto sql = fmt::format("ALTER TABLE MSIObstacle ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }

            if (!m_database->fieldExists("GeometryType", "MSIObstacle"))
            {
                auto sql = fmt::format("ALTER TABLE MSIObstacle ADD GeometryType INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("SectorStartAngle", "MSIObstacle"))
            {
                auto sql = fmt::format("ALTER TABLE MSIObstacle ADD SectorStartAngle INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("SectorEndAngle", "MSIObstacle"))
            {
                auto sql = fmt::format("ALTER TABLE MSIObstacle ADD SectorEndAngle INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("Polygon", "MSIObstacle"))
            {
                auto sql = fmt::format("ALTER TABLE MSIObstacle ADD Polygon BLOB DEFAULT NULL");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMSIObstacleTable");
        }
    }

    void VDESManager::Impl::InitializeMSIMaritineOperationTable(void)
    {
        try
        {
            if (!m_database->tableExists("MSIMaritimeOperation"))
            {
                auto sql = fmt::format("CREATE TABLE MSIMaritimeOperation ("
                    "ID                      INTEGER   PRIMARY KEY,"
                    "Type                    INT       NOT NULL DEFAULT 0,"
                    "Status                  INT       NOT NULL DEFAULT 0,"
                    "MMSI                    INT       NOT NULL DEFAULT 0,"
                    "LatitudeNE              DOUBLE    NOT NULL DEFAULT 91.0,"
                    "LongitudeNE             DOUBLE    NOT NULL DEFAULT 181.0,"
                    "LatitudeSW              DOUBLE    NOT NULL DEFAULT 91.0,"
                    "LongitudeSW             DOUBLE    NOT NULL DEFAULT 181.0,"
                    "Range                   DOUBLE    NOT NULL DEFAULT 0.0,"
                    "[Timestamp Start]       INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp End]         INTEGER   NOT NULL DEFAULT 0,"
                    "Certified               BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");

                m_database->exec(sql);

                if (!m_database->indexExists("OperationTimeIndex", "MSIMaritimeOperation"))
                {
                    m_database->exec("CREATE INDEX OperationTimeIndex ON MSIMaritimeOperation ([Timestamp Receive])");
                }

                if (!m_database->tableExists("MSIMaritimeOperationBBox"))
                {
                    m_database->exec("CREATE VIRTUAL TABLE MSIMaritimeOperationBBox USING rtree(ID, Left, Right, Bottom, Top)");
                }
            }

            if (!m_database->fieldExists("Read", "MSIMaritimeOperation"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeOperation ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }

            if (!m_database->fieldExists("GeometryType", "MSIMaritimeOperation"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeOperation ADD GeometryType INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("SectorStartAngle", "MSIMaritimeOperation"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeOperation ADD SectorStartAngle INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("SectorEndAngle", "MSIMaritimeOperation"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeOperation ADD SectorEndAngle INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("Polygon", "MSIMaritimeOperation"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeOperation ADD Polygon BLOB DEFAULT NULL");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("TimeType", "MSIMaritimeOperation"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeOperation ADD TimeType INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("Caution", "MSIMaritimeOperation"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeOperation ADD Caution INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMSIMaritineOperationTable");
        }
    }

    void VDESManager::Impl::InitializeMSIMilitaryActivityTable(void)
    {
        try
        {
            if (!m_database->tableExists("MSIMilitaryActivity"))
            {
                auto sql = fmt::format("CREATE TABLE MSIMilitaryActivity ("
                    "ID                      INTEGER   PRIMARY KEY,"
                    "Theme                   INT       NOT NULL DEFAULT 0,"
                    "Duration                BOOLEAN   NOT NULL DEFAULT 0,"
                    "Coordinates             BLOB,"
                    "[Timestamp Start]       INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp End]         INTEGER   NOT NULL DEFAULT 0,"
                    "Caution                 INT       NOT NULL DEFAULT 0,"
                    "Certified               BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");

                m_database->exec(sql);

                if (!m_database->indexExists("ActivityTimeIndex", "MSIMilitaryActivity"))
                {
                    m_database->exec("CREATE INDEX ActivityTimeIndex ON MSIMilitaryActivity ([Timestamp Receive])");
                }

                if (!m_database->tableExists("MSIMilitaryActivityBBox"))
                {
                    m_database->exec("CREATE VIRTUAL TABLE MSIMilitaryActivityBBox USING rtree(ID, Left, Right, Bottom, Top)");
                }
            }

            if (!m_database->fieldExists("Read", "MSIMilitaryActivity"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMilitaryActivity ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }

            if (!m_database->fieldExists("GeometryType", "MSIMilitaryActivity"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMilitaryActivity ADD GeometryType INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("SectorStartAngle", "MSIMilitaryActivity"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMilitaryActivity ADD SectorStartAngle INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("SectorEndAngle", "MSIMilitaryActivity"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMilitaryActivity ADD SectorEndAngle INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("Range", "MSIMilitaryActivity"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMilitaryActivity ADD Range DOUBLE NOT NULL DEFAULT 0.0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("TimeType", "MSIMilitaryActivity"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMilitaryActivity ADD TimeType INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMSIMilitaryActivityTable");
        }
    }

    void VDESManager::Impl::InitializeMSIMaritimeDistressTable(void)
    {
        try
        {
            if (!m_database->tableExists("MSIMaritimeDistress"))
            {
                auto sql = fmt::format("CREATE TABLE MSIMaritimeDistress ("
                    "ID                      INTEGER   PRIMARY KEY,"
                    "Type                    INT       NOT NULL DEFAULT 0,"
                    "Situation               INT       NOT NULL DEFAULT 0,"
                    "Status                  INT       NOT NULL DEFAULT 0,"
                    "Additional              INT       NOT NULL DEFAULT 0,"
                    "Latitude                DOUBLE    NOT NULL DEFAULT 91.0,"
                    "Longitude               DOUBLE    NOT NULL DEFAULT 181.0,"
                    "Timestamp               INTEGER   NOT NULL DEFAULT 0,"
                    "Certified               BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");

                m_database->exec(sql);

                if (!m_database->indexExists("DistressTimeIndex", "MSIMaritimeDistress"))
                {
                    m_database->exec("CREATE INDEX DistressTimeIndex ON MSIMaritimeDistress ([Timestamp Receive])");
                }

                if (!m_database->tableExists("MSIMaritimeDistressBBox"))
                {
                    m_database->exec("CREATE VIRTUAL TABLE MSIMaritimeDistressBBox USING rtree(ID, Left, Right, Bottom, Top)");
                }
            }

            if (!m_database->fieldExists("Read", "MSIMaritimeDistress"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeDistress ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }

            if (!m_database->fieldExists("StatusDescription", "MSIMaritimeDistress"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeDistress ADD StatusDescription INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("Judgment", "MSIMaritimeDistress"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeDistress ADD Judgment INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("LocationInstruction", "MSIMaritimeDistress"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeDistress ADD LocationInstruction INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("Duration", "MSIMaritimeDistress"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeDistress ADD Duration INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
            if (!m_database->fieldExists("Caution", "MSIMaritimeDistress"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeDistress ADD Caution INT NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMSIMaritimeDistressTable");
        }
    }

    void VDESManager::Impl::InitializeMSIMaritimeTowingTable(void)
    {
        try
        {
            if (!m_database->tableExists("MSIMaritimeTowing"))
            {
                auto sql = fmt::format("CREATE TABLE MSIMaritimeTowing ("
                    "ID                      INTEGER   PRIMARY KEY,"
                    "MMSI                    INT       NOT NULL DEFAULT 0,"
                    "LatitudeStart           DOUBLE    NOT NULL DEFAULT 91.0,"
                    "LongitudeStart          DOUBLE    NOT NULL DEFAULT 181.0,"
                    "LatitudeEnd             DOUBLE    NOT NULL DEFAULT 91.0,"
                    "LongitudeEnd            DOUBLE    NOT NULL DEFAULT 181.0,"
                    "Length                  INT       NOT NULL DEFAULT 0,"
                    "Width                   INT       NOT NULL DEFAULT 0,"
                    "Speed                   DOUBLE    NOT NULL DEFAULT 0.0,"
                    "[Timestamp Start]       INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp End]         INTEGER   NOT NULL DEFAULT 0,"
                    "Caution                 INT       NOT NULL DEFAULT 0,"
                    "Certified               BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");

                m_database->exec(sql);

                if (!m_database->indexExists("TowingTimeIndex", "MSIMaritimeTowing"))
                {
                    m_database->exec("CREATE INDEX TowingTimeIndex ON MSIMaritimeTowing ([Timestamp Receive])");
                }

                if (!m_database->tableExists("MSIMaritimeTowingBBox"))
                {
                    m_database->exec("CREATE VIRTUAL TABLE MSIMaritimeTowingBBox USING rtree(ID, Left, Right, Bottom, Top)");
                }
            }

            if (!m_database->fieldExists("Read", "MSIMaritimeTowing"))
            {
                auto sql = fmt::format("ALTER TABLE MSIMaritimeTowing ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMSIMaritimeTowingTable");
        }
    }

    void VDESManager::Impl::InitializeMSIDesignatedAreaTable(void)
    {
        try
        {
            m_database->exec("CREATE TABLE IF NOT EXISTS MSIDesignatedArea ("
                "ID INTEGER PRIMARY KEY, "
                "MRN INT, "
                "Fragment INT, "
                "AreaType INT, "
                "GeometryType INT, "
                "Latitude DOUBLE, "
                "Longitude DOUBLE, "
                "Range DOUBLE, "
                "SectorStartAngle INT, "
                "SectorEndAngle INT, "
                "Polygon BLOB, "
                "TimeType INT, "
                "TimestampStart INTEGER, "
                "TimestampEnd INTEGER, "
                "Caution INT, "
                "Certified BOOLEAN, "
                "[Timestamp Receive] INTEGER, "
                "Read BOOLEAN"
                ")");

            if (m_database->tableExists("MSIDesignatedAreaBBox") && !m_database->fieldExists("Left", "MSIDesignatedAreaBBox"))
            {
                m_database->exec("DROP TABLE MSIDesignatedAreaBBox");
            }

            m_database->exec("CREATE VIRTUAL TABLE IF NOT EXISTS MSIDesignatedAreaBBox USING rtree("
                "ID, "
                "Left, Right, "
                "Bottom, Top"
                ")");
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMSIDesignatedAreaTable");
        }
    }

    void VDESManager::Impl::InitializeBridgeTable(void)
    {
        try
        {
            m_database->exec("CREATE TABLE IF NOT EXISTS Bridge ("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                "MRN INT, "
                "Fragment INT, "
                "Latitude DOUBLE, "
                "Longitude DOUBLE, "
                "Height DOUBLE, "
                "Width DOUBLE, "
                "DirectionToPass INT, "
                "PassAbility INT, "
                "EnableMeeting BOOLEAN, "
                "EnableOvertaking BOOLEAN, "
                "FlowVelocity INT, "
                "FlowDirection INT, "
                "[Timestamp Receive] INTEGER"
                ")");
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeBridgeTable");
        }
    }

    void VDESManager::Impl::InitializeChannelCenterlineTable(void)
    {
        try
        {
            m_database->exec("CREATE TABLE IF NOT EXISTS ChannelCenterline ("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                "MRN INT, "
                "Fragment INT, "
                "Width INT, "
                "Coordinates BLOB, "
                "[Timestamp Receive] INTEGER"
                ")");
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeChannelCenterlineTable");
        }
    }

    void VDESManager::Impl::InitializeChannelBoundaryTable(void)
    {
        try
        {
            m_database->exec("CREATE TABLE IF NOT EXISTS ChannelBoundary ("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                "MRN INT, "
                "Fragment INT, "
                "EdgeType INT, "
                "Coordinates BLOB, "
                "[Timestamp Receive] INTEGER"
                ")");
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeChannelBoundaryTable");
        }
    }

    void VDESManager::Impl::InitializeNetSounderTable(void)
    {
        try
        {
            m_database->exec("CREATE TABLE IF NOT EXISTS NetSounder ("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                "MRN INT, "
                "Fragment INT, "
                "Type INT, "
                "IsContinous INT, "
                "Coordinates BLOB, "
                "[Timestamp Receive] INTEGER"
                ")");
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeNetSounderTable");
        }
    }

    void VDESManager::Impl::InitializeFrontendPromptTable(void)
    {
        try
        {
            m_database->exec("CREATE TABLE IF NOT EXISTS FrontendPrompt ("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                "PromptContent INT, "
                "DisplayDuration INT, "
                "Text TEXT, "
                "[Timestamp Receive] INTEGER, "
                "[Timestamp Expiry] INTEGER"
                ")");
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeFrontendPromptTable");
        }
    }

    void VDESManager::Impl::InitializeHydrometeorologyTable(void)
    {
        try
        {
             if (!m_database->tableExists("Hydrometeorology"))
            {
                auto sql = fmt::format("CREATE TABLE Hydrometeorology ("
                    "ID                      INTEGER   PRIMARY KEY,"
                    "Latitude                DOUBLE    NOT NULL DEFAULT 91.0,"
                    "Longitude               DOUBLE    NOT NULL DEFAULT 181.0,"
                    "[Timestamp HT]          INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp LT]          INTEGER   NOT NULL DEFAULT 0,"
                    "[Water Level]           DOUBLE    NOT NULL DEFAULT 0,"
                    "Source                  INT       NOT NULL DEFAULT 0,"
                    "[Timestamp Forecast]    INTEGER   NOT NULL DEFAULT 0,"
                    "Certified               BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Timestamp Published]   INTEGER   NOT NULL DEFAULT 0,"
                    "UNIQUE(Latitude, Longitude, [Timestamp Forecast]))");

                m_database->exec(sql);

                if (!m_database->indexExists("HydroTimeRcvIndex", "Hydrometeorology"))
                {
                    m_database->exec("CREATE INDEX HydroTimeRcvIndex ON Hydrometeorology ([Timestamp Receive])");
                }

                if (!m_database->indexExists("HydroCoordinateIndex", "Hydrometeorology"))
                {
                    m_database->exec("CREATE INDEX HydroCoordinateIndex ON Hydrometeorology (Latitude, Longitude)");
                }

                if (!m_database->indexExists("HydroTimeForeIndex", "Hydrometeorology"))
                {
                    m_database->exec("CREATE INDEX HydroTimeForeIndex ON Hydrometeorology ([Timestamp Forecast])");
                }

                if (!m_database->tableExists("HydrometeorologyBBox"))
                {
                    m_database->exec("CREATE VIRTUAL TABLE HydrometeorologyBBox USING rtree(ID, Left, Right, Bottom, Top)");
                }
            }

            if (!m_database->fieldExists("Read", "Hydrometeorology"))
            {
                auto sql = fmt::format("ALTER TABLE Hydrometeorology ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }

            if (!m_database->fieldExists("[Timestamp Published]", "Hydrometeorology"))
            {
                auto sql = fmt::format("ALTER TABLE Hydrometeorology ADD [Timestamp Published] INTEGER NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMSIMaritimeDistressTable");
        }
    }

    void VDESManager::Impl::InitializeAbnormalShipTable(void)
    {
        try
        {
            auto tableExist = m_database->tableExists("AbnormalShip");

            if (tableExist && m_version < 1)
            {
                // Need to clean the datas
                auto sql = fmt::format("DROP TABLE AbnormalShip");
                m_database->exec(sql);
                tableExist = false;
                sql = fmt::format("DROP TABLE CoordinateInfo");
                m_database->exec(sql);

                m_version = 1;
                m_database->exec("PRAGMA user_version = 1");
            }
           
            if (!tableExist)
            {
                auto sql = fmt::format("CREATE TABLE AbnormalShip ("
                    "ID                      INTEGER   PRIMARY KEY,"
                    "MMSI                    INT       NOT NULL DEFAULT 0,"
                    "Type                    INT       NOT NULL DEFAULT 0,"
                    "Content                 TEXT,"
                    "Certified               BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");

                m_database->exec(sql);

                if (!m_database->indexExists("ShipTimeRcvIndex", "AbnormalShip"))
                {
                    m_database->exec("CREATE INDEX ShipTimeRcvIndex ON AbnormalShip ([Timestamp Receive])");
                }
            }

            if (!m_database->fieldExists("Read", "AbnormalShip"))
            {
                auto sql = fmt::format("ALTER TABLE AbnormalShip ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }

            if (!m_database->tableExists("CoordinateInfo"))
            {
                auto sql = fmt::format("CREATE TABLE CoordinateInfo ("
                    "ID                      INTEGER   NOT NULL,"
                    "Latitude                DOUBLE    NOT NULL DEFAULT 91.0,"
                    "Longitude               DOUBLE    NOT NULL DEFAULT 181.0,"
                    "[Timestamp Alert]       INTEGER   NOT NULL DEFAULT 0)");
                m_database->exec(sql);

                if (!m_database->indexExists("IDIndex", "CoordinateInfo"))
                {
                    m_database->exec("CREATE INDEX IDIndex ON CoordinateInfo (ID)");
                }
            }   
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeAbnormalShipTable");
        }
    }

    void VDESManager::Impl::InitializeMarineMeteorologyFCSTTable(void)
    {
        try
        {
            if (!m_database->tableExists("MarineMeteorologyFCST"))
            {
                auto sql = fmt::format("CREATE TABLE MarineMeteorologyFCST("
                    "ID				        INTEGER   PRIMARY KEY,"
                    "[Weather Code]         INT		  NOT NULL  DEFAULT  0,"
                    "Latitude			    DOUBLE	  NOT NULL  DEFAULT  91,"
                    "Longitude			    DOUBLE    NOT NULL  DEFAULT  181,"
                    "[Wind Speed]			DOUBLE    NOT NULL  DEFAULT  -1,"
                    "[Wind Direction]		DOUBLE    NOT NULL  DEFAULT  360,"
                    "Temperature		    DOUBLE    NOT NULL  DEFAULT  -100,"
                    "[Air Pressure]		    INT       NOT NULL  DEFAULT  0,"
                    "Visibility			    DOUBLE    NOT NULL  DEFAULT  -1,"
                    "[Info Source]			INT       NOT NULL  DEFAULT  -1,"
                    "[Timestamp Forecast]   INTEGER   NOT NULL  DEFAULT  0,"
                    "[Timestamp Receive]    INTEGER   NOT NULL  DEFAULT  0,"
                    "Read                   BOOLEAN   NOT NULL  DEFAULT  0,"
                    "UNIQUE (Latitude, Longitude, [Timestamp Forecast]))");
                m_database->exec(sql);

                if (!m_database->indexExists("MeteorologyFCSTTimeIndex", "MarineMeteorologyFCST"))
                {
                    m_database->exec("CREATE INDEX MeteorologyFCSTTimeIndex ON MarineMeteorologyFCST ([Timestamp Forecast])");
                }

                if (!m_database->tableExists("MeteorologyFCSTBBox"))
                {
                    m_database->exec("CREATE VIRTUAL TABLE MeteorologyFCSTBBox USING rtree(ID, Left, Right, Bottom, Top)");
                }
            }

            if (!m_database->fieldExists("Read", "MarineMeteorologyFCST"))
            {
                auto sql = fmt::format("ALTER TABLE MarineMeteorologyFCST ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMarineMeteorologyFCSTTable");
        }
    }

    void VDESManager::Impl::InitializeMarineEnvironmentFCSTTable(void)
    {
        try
        {
            if (!m_database->tableExists("MarineEnvironmentFCST"))
            {
                auto sql = fmt::format("CREATE TABLE MarineEnvironmentFCST("
                    "ID				        INTEGER   PRIMARY KEY,"
                    "Latitude			    DOUBLE	  NOT NULL  DEFAULT  91,"
                    "Longitude			    DOUBLE    NOT NULL  DEFAULT  181,"
                    "[Flow Velocity]	    DOUBLE    NOT NULL  DEFAULT  -1,"
                    "[Flow Direction]		INT       NOT NULL  DEFAULT  360,"
                    "[Wave Height] 		    DOUBLE    NOT NULL  DEFAULT  -1.0,"
                    "[Wave Direction]		INT       NOT NULL  DEFAULT  360,"
                    "Temperature		    DOUBLE    NOT NULL  DEFAULT  -100,"
                    "[Info Source]			INT       NOT NULL  DEFAULT  -1,"
                    "[Timestamp Forecast]   INTEGER   NOT NULL  DEFAULT  0,"
                    "[Timestamp Receive]    INTEGER   NOT NULL  DEFAULT  0,"
                    "Read                   BOOLEAN   NOT NULL  DEFAULT  0,"
                    "UNIQUE (Latitude, Longitude, [Timestamp Forecast]))");
                m_database->exec(sql);

                if (!m_database->indexExists("EnvironmentFCSTTimeIndex", "MarineEnvironmentFCST"))
                {
                    m_database->exec("CREATE INDEX EnvironmentFCSTTimeIndex ON MarineEnvironmentFCST ([Timestamp Forecast])");
                }

                if (!m_database->tableExists("EnvironmentFCSTBBox"))
                {
                    m_database->exec("CREATE VIRTUAL TABLE EnvironmentFCSTBBox USING rtree(ID, Left, Right, Bottom, Top)");
                }
            }

            if (!m_database->fieldExists("Read", "MarineEnvironmentFCST"))
            {
                auto sql = fmt::format("ALTER TABLE MarineEnvironmentFCST ADD Read BOOLEAN NOT NULL DEFAULT 0");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMarineEnvironmentFCSTTable");
        }
    }

    void VDESManager::Impl::InitializeMarineMeteorologyFCSTAreaTable(void)
    {
        try
        {
            if (!m_database->tableExists("MarineMeteorologyFCSTArea"))
            {
                auto sql = fmt::format("CREATE TABLE MarineMeteorologyFCSTArea("
                    "[Area Code]          INT       PRIMARY KEY,"
                    "[Weather Code]       INT       NOT NULL DEFAULT 0,"
                    "[Wind Direction]     INT       NOT NULL DEFAULT 360,"
                    "[Wind Scale Low]     INT       NOT NULL DEFAULT 20,"
                    "[Wind Scale High]    INT       NOT NULL DEFAULT 20,"
                    "Visibility           INT       NOT NULL DEFAULT 22,"
                    "[Info Source]        INT       NOT NULL DEFAULT 0,"
                    "[Timestamp Forecast] INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]  INTEGER   NOT NULL DEFAULT 0)");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMarineMeteorologyFCSTAreaTable");
        }
    }

    void VDESManager::Impl::InitializeMarineEnvironmentFCSTAreaTable(void)
    {
        try
        {
            if (!m_database->tableExists("MarineEnvironmentFCSTArea"))
            {
                auto sql = fmt::format("CREATE TABLE MarineEnvironmentFCSTArea("
                    "[Area Code]          INT       PRIMARY KEY,"
                    "[Temperature Low]    INT       NOT NULL DEFAULT 511,"
                    "[Temperature High]   INT       NOT NULL DEFAULT 202,"
                    "[Flow Direction Avg] INT       NOT NULL DEFAULT 360,"
                    "[Flow Direction Max] INT       NOT NULL DEFAULT 360,"
                    "[Flow Velocity Avg]  INT       NOT NULL DEFAULT 252,"
                    "[Flow Velocity Max]  INT       NOT NULL DEFAULT 252,"
                    "[Wave Height]        INT       NOT NULL DEFAULT 252,"
                    "[Swell Height]       INT       NOT NULL DEFAULT 252,"
                    "[Info Source]        INT       NOT NULL DEFAULT 0,"
                    "[Timestamp Forecast] INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]  INTEGER   NOT NULL DEFAULT 0)");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMarineEnvironmentFCSTAreaTable");
        }
    }

    void VDESManager::Impl::InitializeMarineEnvironmentFCSTAlongshoreTable(void)
    {
        try
        {
            if (!m_database->tableExists("MarineEnvironmentFCSTAlongshore"))
            {
                auto sql = fmt::format("CREATE TABLE MarineEnvironmentFCSTAlongshore("
                    "[Area Code]          INT       PRIMARY KEY,"
                    "[Temperature Low]    INT       NOT NULL DEFAULT 511,"
                    "[Temperature High]   INT       NOT NULL DEFAULT 202,"
                    "[Wave Height Low]    INT       NOT NULL DEFAULT 252,"
                    "[Wave Height High]   INT       NOT NULL DEFAULT 252,"
                    "[Tide High]          INT       NOT NULL DEFAULT 1003,"
                    "[Timestamp Tide High] INTEGER  NOT NULL DEFAULT 0,"
                    "[Tide Low]           INT       NOT NULL DEFAULT 503,"
                    "[Timestamp Tide Low]  INTEGER  NOT NULL DEFAULT 0,"
                    "[Info Source]        INT       NOT NULL DEFAULT 0,"
                    "[Timestamp Forecast] INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]  INTEGER   NOT NULL DEFAULT 0)");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMarineEnvironmentFCSTAlongshoreTable");
        }
    }

    void VDESManager::Impl::InitializeMarineMeteorologyEWTable(void)
    {
        try
        {
            if (!m_database->tableExists("MarineMeteorologyEW"))
            {
                auto sql = fmt::format("CREATE TABLE MarineMeteorologyEW ("
                    "ID                      INTEGER   PRIMARY KEY AUTOINCREMENT,"
                    "MRN                     INT       NOT NULL DEFAULT 0,"
                    "Fragment                INT       NOT NULL DEFAULT 0,"
                    "[Warning Type]          INT       NOT NULL DEFAULT 0,"
                    "[Timestamp Published]   INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp Start]       INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp End]         INTEGER   NOT NULL DEFAULT 0,"
                    "[Warning Duration]      INT       NOT NULL DEFAULT 0,"
                    "[Info Source]           INT       NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");
                m_database->exec(sql);

                if (!m_database->indexExists("EWTimeRcvIndex", "MarineMeteorologyEW"))
                {
                    m_database->exec("CREATE INDEX EWTimeRcvIndex ON MarineMeteorologyEW ([Timestamp Receive])");
                }
            }

            if (!m_database->tableExists("EW_TropicalCyclone"))
            {
                auto sql = fmt::format("CREATE TABLE EW_TropicalCyclone ("
                    "ID                      INTEGER   PRIMARY KEY AUTOINCREMENT,"
                    "Warning_ID              INTEGER   NOT NULL,"
                    "Timestamp               INTEGER   NOT NULL DEFAULT 0,"
                    "[Center Lon]            DOUBLE    NOT NULL DEFAULT 181.0,"
                    "[Center Lat]            DOUBLE    NOT NULL DEFAULT 91.0,"
                    "[Cyclone Type]          INT       NOT NULL DEFAULT 0,"
                    "[Wind 7 Radius]         INT       NOT NULL DEFAULT 0,"
                    "[Wind 10 Radius]        INT       NOT NULL DEFAULT 0,"
                    "[Wind 12 Radius]        INT       NOT NULL DEFAULT 0,"
                    "[Move Speed]            INT       NOT NULL DEFAULT 0,"
                    "[Move Direction]        INT       NOT NULL DEFAULT 360,"
                    "[Max Wind Scale]        INT       NOT NULL DEFAULT 20,"
                    "[Center Pressure]       INT       NOT NULL DEFAULT 403,"
                    "FOREIGN KEY(Warning_ID) REFERENCES MarineMeteorologyEW(ID) ON DELETE CASCADE)");
                m_database->exec(sql);

                if (!m_database->indexExists("EWCycloneWarningIDIndex", "EW_TropicalCyclone"))
                {
                    m_database->exec("CREATE INDEX EWCycloneWarningIDIndex ON EW_TropicalCyclone (Warning_ID)");
                }
            }

            if (!m_database->tableExists("EW_WarningElement"))
            {
                auto sql = fmt::format("CREATE TABLE EW_WarningElement ("
                    "Warning_ID              INTEGER   NOT NULL,"
                    "MRN                     INT       NOT NULL DEFAULT 0,"
                    "[Area Code]             INT       NOT NULL DEFAULT 0,"
                    "[Warning Level]         INT       NOT NULL DEFAULT 0,"
                    "[Surge Height]          DOUBLE    NOT NULL DEFAULT 0.0,"
                    "FOREIGN KEY(Warning_ID) REFERENCES MarineMeteorologyEW(ID) ON DELETE CASCADE)");
                m_database->exec(sql);

                if (!m_database->indexExists("EWElementWarningIDIndex", "EW_WarningElement"))
                {
                    m_database->exec("CREATE INDEX EWElementWarningIDIndex ON EW_WarningElement (Warning_ID)");
                }
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeMarineMeteorologyEWTable");
        }
    }

    void VDESManager::Impl::InitializeTideForecastTable(void)
    {
        try
        {
            if (!m_database->tableExists("TideForecast"))
            {
                auto sql = fmt::format("CREATE TABLE TideForecast ("
                    "ID                      INTEGER   PRIMARY KEY AUTOINCREMENT,"
                    "[Hour Publish]          INT       NOT NULL DEFAULT 24,"
                    "[Info Source]           INT       NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");
                m_database->exec(sql);

                if (!m_database->indexExists("TideTimeRcvIndex", "TideForecast"))
                {
                    m_database->exec("CREATE INDEX TideTimeRcvIndex ON TideForecast ([Timestamp Receive])");
                }
            }

            if (!m_database->tableExists("TideStation"))
            {
                auto sql = fmt::format("CREATE TABLE TideStation ("
                    "Forecast_ID             INTEGER   NOT NULL,"
                    "Longitude               DOUBLE    NOT NULL DEFAULT 181.0,"
                    "Latitude                DOUBLE    NOT NULL DEFAULT 91.0,"
                    "[Tidal Datum]           DOUBLE    NOT NULL DEFAULT 0.0,"
                    "[Tide High]             DOUBLE    NOT NULL DEFAULT 0.0,"
                    "[Timestamp Tide High]   INTEGER   NOT NULL DEFAULT 0,"
                    "[Tide Low]              DOUBLE    NOT NULL DEFAULT 0.0,"
                    "[Timestamp Tide Low]    INTEGER   NOT NULL DEFAULT 0,"
                    "FOREIGN KEY(Forecast_ID) REFERENCES TideForecast(ID) ON DELETE CASCADE)");
                m_database->exec(sql);

                if (!m_database->indexExists("TideStationForecastIDIndex", "TideStation"))
                {
                    m_database->exec("CREATE INDEX TideStationForecastIDIndex ON TideStation (Forecast_ID)");
                }
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeTideForecastTable");
        }
    }

    void VDESManager::Impl::InitializeAtoNDynamicsTable(void)
    {
        try
        {
            if (!m_database->tableExists("AtoNDynamics"))
            {
                auto sql = fmt::format("CREATE TABLE AtoNDynamics ("
                    "ID                      INTEGER   PRIMARY KEY AUTOINCREMENT,"
                    "MRN                     INT       NOT NULL DEFAULT 0,"
                    "Fragment                INT       NOT NULL DEFAULT 0,"
                    "Status                  INT       NOT NULL DEFAULT 0,"
                    "Precaution              INT       NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");
                m_database->exec(sql);

                if (!m_database->indexExists("AtoNTimeRcvIndex", "AtoNDynamics"))
                {
                    m_database->exec("CREATE INDEX AtoNTimeRcvIndex ON AtoNDynamics ([Timestamp Receive])");
                }
            }

            if (!m_database->tableExists("AtoNDynamicElement"))
            {
                auto sql = fmt::format("CREATE TABLE AtoNDynamicElement ("
                    "Dynamics_ID             INTEGER   NOT NULL,"
                    "MRN                     INT       NOT NULL DEFAULT 0,"
                    "Type                    INT       NOT NULL DEFAULT 0,"
                    "Longitude               DOUBLE    NOT NULL DEFAULT 181.0,"
                    "Latitude                DOUBLE    NOT NULL DEFAULT 91.0,"
                    "[Rhythm Name Code]      INT       NOT NULL DEFAULT 0,"
                    "[Rhythm Param Code]     INT       NOT NULL DEFAULT 0,"
                    "[Body Color]            INT       NOT NULL DEFAULT 0,"
                    "[Light Color]           INT       NOT NULL DEFAULT 0,"
                    "[Light Period]          INT       NOT NULL DEFAULT 0,"
                    "Range                   INT       NOT NULL DEFAULT 0,"
                    "[Morse Code]            INT       NOT NULL DEFAULT 0,"
                    "[Prev Longitude]        DOUBLE    NOT NULL DEFAULT 181.0,"
                    "[Prev Latitude]         DOUBLE    NOT NULL DEFAULT 91.0,"
                    "[Is Rough Position]     BOOLEAN   NOT NULL DEFAULT 0,"
                    "[Failure Type]          INT       NOT NULL DEFAULT 0,"
                    "[Timestamp Start]       INTEGER   NOT NULL DEFAULT 0,"
                    "[Timestamp End]         INTEGER   NOT NULL DEFAULT 0,"
                    "FOREIGN KEY(Dynamics_ID) REFERENCES AtoNDynamics(ID) ON DELETE CASCADE)");
                m_database->exec(sql);

                if (!m_database->indexExists("AtoNDynamicElementIDIndex", "AtoNDynamicElement"))
                {
                    m_database->exec("CREATE INDEX AtoNDynamicElementIDIndex ON AtoNDynamicElement (Dynamics_ID)");
                }
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeAtoNDynamicsTable");
        }
    }

    void VDESManager::Impl::InitializeAISAtoNDynamicsTable(void)
    {
        try
        {
            if (!m_database->tableExists("AISAtoNDynamics"))
            {
                auto sql = fmt::format("CREATE TABLE AISAtoNDynamics ("
                    "ID                      INTEGER   PRIMARY KEY AUTOINCREMENT,"
                    "MRN                     INT       NOT NULL DEFAULT 0,"
                    "Fragment                INT       NOT NULL DEFAULT 0,"
                    "AtoNAttribute           INT       NOT NULL DEFAULT 0,"
                    "Status                  INT       NOT NULL DEFAULT 0,"
                    "Precaution              INT       NOT NULL DEFAULT 0,"
                    "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                    "Read                    BOOLEAN   NOT NULL DEFAULT 0)");
                m_database->exec(sql);

                if (!m_database->indexExists("AISAtoNTimeRcvIndex", "AISAtoNDynamics"))
                {
                    m_database->exec("CREATE INDEX AISAtoNTimeRcvIndex ON AISAtoNDynamics ([Timestamp Receive])");
                }
            }

            if (!m_database->tableExists("AISAtoNDynamicElement"))
            {
                auto sql = fmt::format("CREATE TABLE AISAtoNDynamicElement ("
                    "Dynamics_ID             INTEGER   NOT NULL,"
                    "MMSI                    INT       NOT NULL DEFAULT 0,"
                    "MRN                     INT       NOT NULL DEFAULT 0,"
                    "Type                    INT       NOT NULL DEFAULT 0,"
                    "Longitude               DOUBLE    NOT NULL DEFAULT 181.0,"
                    "Latitude                DOUBLE    NOT NULL DEFAULT 91.0,"
                    "Range                   INT       NOT NULL DEFAULT 0,"
                    "[Prev Longitude]        DOUBLE    NOT NULL DEFAULT 181.0,"
                    "[Prev Latitude]         DOUBLE    NOT NULL DEFAULT 91.0,"
                    "[Is Rough Position]     BOOLEAN   NOT NULL DEFAULT 0,"
                    "FOREIGN KEY(Dynamics_ID) REFERENCES AISAtoNDynamics(ID) ON DELETE CASCADE)");
                m_database->exec(sql);

                if (!m_database->indexExists("AISAtoNDynamicElementIDIndex", "AISAtoNDynamicElement"))
                {
                    m_database->exec("CREATE INDEX AISAtoNDynamicElementIDIndex ON AISAtoNDynamicElement (Dynamics_ID)");
                }
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeAISAtoNDynamicsTable");
        }
    }

    void VDESManager::Impl::InitializeVTSRequestTable(void)
    {
        try
        {
            if (!m_database->tableExists("VTSRequest"))
            {
                auto sql = fmt::format("CREATE TABLE VTSRequest("
                    "ID				        INTEGER   PRIMARY KEY,"
                    "[Text Sequence Number] INT       NOT NULL DEFAULT 0,"
                    "[Request Type]			INT 	  NOT NULL  DEFAULT  0,"
                    "[Timestamp Request]    INTEGER   NOT NULL  DEFAULT  0,"
                    "[VTS Reply]            INTEGER   NOT NULL  DEFAULT  -1,"
                    "[Timestamp Reply]      INTEGER   NOT NULL  DEFAULT  0,"
                    "[Executing Status]     INTEGER   NOT NULL  DEFAULT  -1)");
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeVTSRequestTable");
        }
    }

    void VDESManager::Impl::InitializeVTSReplyTable(void)
    {
        try
        {
            if (!m_database->tableExists("VTSReply"))
            {
                auto sql = fmt::format("CREATE TABLE VTSReply("
                    "ID				        INTEGER   PRIMARY KEY,"
                    "[VTS Request ID]		INTEGER   NOT NULL DEFAULT -1,"
                    "[Text Sequence Number] INTEGER   NOT NULL DEFAULT 0,"
                    "[Reply Result]			INTEGER   NOT NULL  DEFAULT  -1,"
                    "[Requestor MMSI]       INTEGER   NOT NULL  DEFAULT  0,"
                    "[Chinese Remark]       TEXT,"
                    "[English Remark]       TEXT,"
                    "[Queue Number]         INTEGER   NOT NULL  DEFAULT  -1,"
                    "Waypoints              BLOB,"
                    "Timestamp              INTEGER   NOT NULL  DEFAULT  0)");
                m_database->exec(sql);

                // Create indexes
                m_database->exec("CREATE INDEX VTSReplyTimeIndex ON VTSReply (Timestamp)");
                m_database->exec("CREATE INDEX VTSReplyRequestIDIndex ON VTSReply ([VTS Request ID])");
                m_database->exec("CREATE INDEX VTSReplyTxtSeqIndex ON VTSReply ([Text Sequence Number])");
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeVTSReplyTable");
        }
    }

    void VDESManager::Impl::InitializeDatabase(void)
    {
        if (!m_database)
        {
            auto storagePath = VDES::ConfigureManager::GetInstance().GetStoragePath();
            auto filePath = storagePath + "/VDES.db";

            m_database = std::unique_ptr<SQLite::Database>(new SQLite::Database(filePath,
                SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE));
        }
        
        if (m_database)
        {
            if (!m_database->isOpened())
            {
                m_database->open();
            }
            
            m_database->setBusyTimeout(2000);
            m_database->exec("PRAGMA auto_vacuum = 1");
            m_version = m_database->execAndGet("PRAGMA user_version").getUInt();
          
            InitializeMessageTable();
            InitializeAtoNTable();
            InitializeMSIObstacleTable();
            InitializeMSIMaritineOperationTable();
            InitializeMSIMilitaryActivityTable();
            InitializeMSIMaritimeDistressTable();
            InitializeMSIMaritimeTowingTable();
            InitializeMSIDesignatedAreaTable();
            InitializeBridgeTable();
            InitializeChannelCenterlineTable();
            InitializeChannelBoundaryTable();
            InitializeNetSounderTable();
            InitializeFrontendPromptTable();
            InitializeHydrometeorologyTable();
            InitializeAbnormalShipTable();
            InitializeVTSRequestTable();
            InitializeVTSReplyTable();

            InitializeMarineMeteorologyFCSTTable();
            InitializeMarineEnvironmentFCSTTable();
            InitializeMarineMeteorologyEWTable();
            InitializeTideForecastTable();
            InitializeAtoNDynamicsTable();
            InitializeAISAtoNDynamicsTable();

            InitializeRouteRecommendationResponseTable();
            InitializeHydrometeorologyResponseTable();
            InitializeHydrometeorologyRequestTable();
            InitializeRouteRecommendationRequestTable();
            InitializeOwnExtendedVesselInfoTable();
            InitializeOtherVesselExtendedInfoTable();
            InitializeMarineMeteorologyFCSTAreaTable();
            InitializeMarineEnvironmentFCSTAreaTable();
            InitializeMarineEnvironmentFCSTAlongshoreTable();
        }
    }

    double VDESManager::Impl::DecodeCoordinate(const AISBitsManager &manager, const uint32_t startBitPos,
        const uint32_t bitsNum)
    {
        auto temp = manager.DecodeToNumerical(startBitPos, bitsNum);
        if ((temp & ((int64_t)1 << (bitsNum - 1))))
        {
            temp = ((int64_t)1 << bitsNum) - temp;
            temp *= -1;
        }
        auto value = temp / 10000.0 / 60.0;

        return value; 
    }

    uint64_t VDESManager::Impl::DecodeTime(const AISBitsManager &manager, const uint32_t startBitPos, 
        const uint32_t bitsNum)
    {
        if (bitsNum == 20 || bitsNum == 16)
        {
            uint8_t  month = 0, day = 0, hour = 0, minute = 0;
            uint32_t pos = startBitPos;

            if (bitsNum == 20)
            {
                month = static_cast<uint8_t>(manager.DecodeToNumerical(startBitPos, 4));
                pos += 4;
            }
            day = static_cast<uint8_t>(manager.DecodeToNumerical(pos, 5));
            hour = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 5, 5));
            minute = static_cast<uint8_t>(manager.DecodeToNumerical(pos + 10, 6));
            
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
                timeUTC.tm_hour = hour;
                timeUTC.tm_min = minute;
                timeUTC.tm_sec = 0;
                auto timestamp = UtilityInterface::MakeGmtime(&timeUTC);
                return timestamp;
            }
        }
        return 0;
    }

    void VDESManager::Impl::ParseOneLineData(const std::string &sentence)
    {
        auto enableParse = true;
        
        m_parent->m_receiveEvent(sentence);

        // Verify the checksum
        if (ConfigureManager::GetInstance().IsEnableVerifyChecksum())
        {
            if (!UtilityInterface::IsChecksumValid(sentence))
            {
                enableParse = false;
            }
        }
       
        if (enableParse)
        {
            SPDLOG_DEBUG("Impl::ParseOneLineData: {}", sentence);
            auto formatter = sentence.substr(3, 3);
            if (m_parsersMap.find(formatter) != m_parsersMap.end())
            {
                m_parsersMap[formatter](sentence);
            }
        }
    }

    /*
    *         1 2 3 4  5  6 
    *         | | | |  |  | 
    *  $--VDM,x,x,x,a,s—s,x*hh<CR><LF>
    *  Field Number:
    *  1)  Total number of sentences needed, 1 to 9
    *  2)  Sentence number, 1 to 9
    *  3)  Sequential message identifier, 0 to 9
    *  4)  AIS channel
    *  5)  Encapsulated ITU-R M.1371 radio message
    *  6)  Number of fill-bits
    */
    void VDESManager::Impl::ParseVDOVDM(const std::string &sentence)
    {
        auto strList = UtilityInterface::SplitString(sentence, ",");
        if (7 == strList.size())
        {
            static std::string payload;
            static uint8_t sequentialID = 0;

            if (!strList.at(1).empty() && !strList.at(2).empty())
            {
                auto totalNum = atoi(strList.at(1).c_str());
                auto sentenceNum = atoi(strList.at(2).c_str());
                auto sequentialIDTemp = atoi(strList.at(3).c_str());

                if (sentenceNum == 1)
                {
                    payload.clear();
                    sequentialID = sequentialIDTemp;
                }
                if (sequentialID == sequentialIDTemp)
                {
                    payload += strList.at(5);
                }
                if (totalNum == sentenceNum)
                {
                    auto strListTemp = UtilityInterface::SplitString(strList.at(6), "*");
                    if (2 == strListTemp.size())
                    {
                        auto fillBitNum = atoi(strListTemp.at(0).c_str());
                        AISBitsManager aisBitsManager;
                        aisBitsManager.SetVDMPayloadToDecode(payload, fillBitNum);
                        
                        auto messageID = static_cast<uint8_t>(aisBitsManager.DecodeToNumerical(0, 6));
                        if (m_aisParserMap.find(messageID) != m_aisParserMap.end())
                        {
                            m_aisParserMap[messageID](aisBitsManager);
                        }
                    }
                }
            }
        }
    }

    /*
    *         1  2   3     4         5	    6  7  
    *         |  |   |     |         |      |  |  
    *  $--VEM,xx,xx,xx,xxxxxxxxx,xxxxxxxxx,s—s,x*hh<CR><LF>
    *  Field Number:
    *  1)  Total number of sentences needed, 1 to 99
    *  2)  Sentence number, 1 to 99
    *  3)  Sequential message identifier, 0 to 99
    *  4)  Source MMSI
    *  5)  Destination MMSI
    *  6)  Encapsulated ITU-R M.1371 radio message, longest 1400 6-bit ASCII codes
    *  7)  Number of fill-bits
    */
    void VDESManager::Impl::ParseVEM(const std::string &sentence)
    {
        auto strList = UtilityInterface::SplitString(sentence, ",");
        if (8 == strList.size())
        {
            if (!strList.at(1).empty() && !strList.at(2).empty())
            {
                static std::string encapsulatedDatas;

                auto totalNum = atoi(strList.at(1).c_str());
                auto sentenceNum = atoi(strList.at(2).c_str());
                auto mmsiSource = atoi(strList.at(4).c_str());
                auto mmsiDest = atoi(strList.at(5).c_str());

                if (1 == sentenceNum)
                {
                    encapsulatedDatas.clear();
                }
                encapsulatedDatas += strList.at(6);
                if (totalNum == sentenceNum)
                {
                    auto strListTemp = UtilityInterface::SplitString(strList.at(7), "*");
                    if (2 == strListTemp.size())
                    {
                        auto fillBits = atoi(strListTemp.at(0).c_str());
                        AISBitsManager manager;
                        manager.SetVDMPayloadToDecode(encapsulatedDatas, fillBits);

                        // VPFI
                        auto VPFI = manager.DecodeToNumerical(0, 16);
                        // Message no
                        auto messageNo = static_cast<uint16_t>(manager.DecodeToNumerical(16, 16));
                        if (m_businessMap.find(messageNo) != m_businessMap.end())
                        {
                            m_businessMap[messageNo](mmsiSource, mmsiDest, manager);
                        }
                    }
                }
            }
        }
    }

    /*
    *         1   2        3           
    *         |   |        |              
    *  $--TMK,xx,xxxxxxxxx,x*hh<CR><LF>
    *  Field Number:
    *  1)  Total number of sentences needed, 1 to 99
    *  2)  Destination MMSI
    *  3)  Status code
    *      0 - Broadcast successful
    *      1 - The message was successfully sent and a reply was received from 
    *          the target node (addressing was successful).
    *      2 - Failed to send
    *      3 - The sending queue is full. Please try again later.
    *      4 - Radio link related time-out
    *      5 - Data length exceeds the limit
    *      6 - Refused service
    */
    void VDESManager::Impl::ParseTMK(const std::string &sentence)
    {
        if (TimerManager::IsTimerValid(m_timerSendMessage))
        {
            auto strList = UtilityInterface::SplitString(sentence, ",");
            if (4 == strList.size())
            {
                auto strListTemp = UtilityInterface::SplitString(strList.at(3), "*");
                if (2 == strListTemp.size())
                {
                    if (!strListTemp.at(0).empty())
                    {
                        auto value = atoi(strListTemp.at(0).c_str());
                        auto statusCode = static_cast<SendStatusCode>(value);
                        UpdateSentBusinessStatus(SendStatus::NONE, statusCode);
                        m_parent->notifyEvent(EventType::MESSAGE_SEND, value);
                    }
                }
            }
            TimerManager::GetInstance().RemoveTimer(m_timerSendMessage);
            m_transmissionBusiness = TransmissionBusiness::NONE;
        }
    }

    /*
    *         1   2        3
    *         |   |        |
    *  $--TSK,xx,xxxxxxxxx,x*hh<CR><LF>
    *  Field Number:
    *  1)  Total number of sentences needed, 1 to 99
    *  2)  Destination MMSI
    *  3)  Status code
    *      0 - Broadcast successful
    *      1 - The message was successfully sent and a reply was received from
    *          the target node (addressing was successful).
    *      2 - Failed to send
    *      3 - The sending queue is full. Please try again later.
    *      4 - Radio link related time-out
    *      5 - Data length exceeds the limit
    *      6 - Refused service
    */
    void VDESManager::Impl::ParseTSK(const std::string &sentence)
    {
        if (TimerManager::IsTimerValid(m_timerSendMessage))
        {
            auto strList = UtilityInterface::SplitString(sentence, ",");
            if (4 == strList.size())
            {
                auto strListTemp = UtilityInterface::SplitString(strList.at(3), "*");
                if (2 == strListTemp.size())
                {
                    if (!strListTemp.at(0).empty())
                    {
                        auto value = atoi(strListTemp.at(0).c_str());
                        auto statusCode = static_cast<SendStatusCode>(value);
                        UpdateSentBusinessStatus(SendStatus::NONE, statusCode);
                        m_parent->notifyEvent(EventType::MESSAGE_SEND, value);
                    }
                }
            }
            TimerManager::GetInstance().RemoveTimer(m_timerSendMessage);
            m_transmissionBusiness = TransmissionBusiness::NONE;
        }
    }

    /*
    *         1         2  3  4 5
    *         |         |  |  | |
    *  $--ABK,xxxxxxxxx,x,x.x,x,x*hh<CR><LF>
    *  Field Number:
    *  1)  MMSI of the addressed AIS unit
    *  2)  AIS channel of reception 
    *  3)  ITU-R M.1371 Message ID
    *  4)  Message sequence number
    *  5)  Type of acknowledgement:
    *      0 = Message (6 or 12) successfully received by the addressed AIS unit
    *      1 = Message (6 or 12) was broadcast, but no acknowledgement by the addressed AIS unit
    *      2 = message could not be broadcast (i.e. quantity of encapsulated data exceeds five slots)
    *      3 = requested broadcast of Message (8, 14, 15, 25 or 26) has been successfully completed
    *      4 = late reception of a Message 7 or 13 acknowledgement that was addressed to this AIS unit
    *          (own ship) and referenced as a valid transaction.
    */
    void VDESManager::Impl::ParseABK(const std::string &sentence)
    {
        if (TimerManager::IsTimerValid(m_timerSendAISMessage))
        {
            std::vector<std::string> seps {",", "*"};
            auto strList = UtilityInterface::SplitString(sentence, seps);
            if (7 == strList.size())
            {
                auto mmsi = atoi(strList.at(1).c_str());
                auto type = atoi(strList.at(5).c_str());

                auto sql = fmt::format("UPDATE MessageOutbox SET [Send Status] = {} WHERE ID = {}",
                                       (type == 2) ? (int)SendStatus::FAILURE : (int)SendStatus::SUCCESS, 
                                       m_dataIDLastSentAISMessage);
                m_database->exec(sql);
                m_parent->notifyEvent(EventType::MESSAGE_SEND, (type == 2) ? 2 : 0);
            }
            TimerManager::GetInstance().RemoveTimer(m_timerSendAISMessage);
        }
    }

    /*
    *         1  2  3      4        5      6  7  8   9   10
    *         |  |  |      |        |      |  |  |   |   |
    *  $--TDB,xx,xx,xx,xxxxxxxxx,xxxxxxxxx,x,xxx,xx,s--s,x*hh<CR><LF>
    *  Field Number:
    *  1)  Total number of sentences needed, 1 to 99
    *  2)  Sentence number, 1 to 99
    *  3)  Sequential message identifier, 0 to 99
    *  4)  Source MMSI
    *  5)  Destination MMSI
    *  6)  Sending Priority
    *  7)  Data type
    *  8)  Link ID (11, 17, 19)
    *  9)  Encapsulated ITU-R M.1371 radio message, longest 1400 6-bit ASCII codes
    * 10)  Number of fill-bits
    */
    void VDESManager::Impl::ParseTDB(const std::string &sentence)
    {
        auto strList = UtilityInterface::SplitString(sentence, std::vector<std::string>{",", "*"});
        if (12 == strList.size())
        {
            if (!strList.at(1).empty() && !strList.at(2).empty())
            {
                static std::string encapsulatedDatas;

                auto totalNum = atoi(strList.at(1).c_str());
                auto sentenceNum = atoi(strList.at(2).c_str());
                auto sequentialNum = atoi(strList.at(3).c_str());
                auto mmsiSource = atoi(strList.at(4).c_str());
                auto mmsiDest = atoi(strList.at(5).c_str());
                auto priority = atoi(strList.at(6).c_str());
				auto dataType = atoi(strList.at(7).c_str());
				auto linkID = atoi(strList.at(8).c_str());

                if (1 == sentenceNum)
                {
                    encapsulatedDatas.clear();
                }
                encapsulatedDatas += strList.at(9);
                if (totalNum == sentenceNum)
                {
                    auto fillBits = atoi(strList.at(10).c_str());
                    AISBitsManager manager;
                    manager.SetVDMPayloadToDecode(encapsulatedDatas, fillBits);

                    // VPFI
                    auto VPFI = manager.DecodeToNumerical(0, 16);
                    // Message no
                    auto messageNo = static_cast<uint16_t>(manager.DecodeToNumerical(16, 16));
                    if (m_businessMap.find(messageNo) != m_businessMap.end())
                    {
                        m_businessMap[messageNo](mmsiSource, mmsiDest, manager);
                    }
                }
            }
        }
    }

    /*
    *         1  2  3  4     5        6       7  8  9   10  11
    *         |  |  |  |     |        |       |  |  |   |   |
    *  $--TSB,xx,xx,xx,xx,xxxxxxxxx,xxxxxxxxx,x,xxx,xx,s--s,x*hh<CR><LF>
    *  Field Number:
    *  1)  Total number of sentences needed, 1 to 99
    *  2)  Sentence number, 1 to 99
    *  3)  Sequential message identifier, 0 to 99
    *  4)  Message ID
    *  5)  Source MMSI
    *  6)  Destination MMSI
    *  7)  Sending Priority
    *  8)  Data type
    *  9)  Link ID (11, 17, 19)
    * 10)  Encapsulated ITU-R M.1371 radio message, longest 1400 6-bit ASCII codes
    * 11)  Number of fill-bits
    */
    void VDESManager::Impl::ParseTSB(const std::string &sentence)
    {
        auto strList = UtilityInterface::SplitString(sentence, std::vector<std::string>{",", "*"});
        if (13 == strList.size())
        {
            if (!strList.at(1).empty() && !strList.at(2).empty())
            {
                static std::string encapsulatedDatas;

                auto totalNum = atoi(strList.at(1).c_str());
                auto sentenceNum = atoi(strList.at(2).c_str());
                auto sequentialNum = atoi(strList.at(3).c_str());
                auto mmsiSource = atoi(strList.at(5).c_str());
                auto mmsiDest = atoi(strList.at(6).c_str());
                auto priority = atoi(strList.at(7).c_str());
                auto dataType = atoi(strList.at(8).c_str());
                auto linkID = atoi(strList.at(9).c_str());

                if (1 == sentenceNum)
                {
                    encapsulatedDatas.clear();
                }
                encapsulatedDatas += strList.at(10);
                if (totalNum == sentenceNum)
                {
                    auto fillBits = atoi(strList.at(11).c_str());
                    AISBitsManager manager;
                    manager.SetVDMPayloadToDecode(encapsulatedDatas, fillBits);

                    // VPFI
                    auto VPFI = manager.DecodeToNumerical(0, 16);
                    // Message no
                    auto messageNo = static_cast<uint16_t>(manager.DecodeToNumerical(16, 16));
                    if (m_businessMap.find(messageNo) != m_businessMap.end())
                    {
                        m_businessMap[messageNo](mmsiSource, mmsiDest, manager);
                    }
                }
            }
        }
    }

    void VDESManager::Impl::ParseAISMessage6(const AISBitsManager &manager)
    {
        auto bitsNum = manager.GetBitsNumberToDecode();
        if (bitsNum > 1008)
        {
            return;
        }

        auto mmsiSource = manager.DecodeToNumerical(8, 30);
        auto mmsiDestination = manager.DecodeToNumerical(40, 30);
        auto ownShipInfo = ConfigureManager::GetInstance().GetOwnVesselInfo();
        // Check wheather the destination mmsi is equal to the ownship.
        if (ownShipInfo.mmsi != mmsiDestination)
        {
            return;
        }

        auto DAC = manager.DecodeToNumerical(72, 10);
        auto FI = manager.DecodeToNumerical(82, 6);
        auto bitPosStart = 0U;

        if (DAC == 1 && FI == 0)
        {
            bitPosStart = 100;
        }
        else if ((DAC == 413 && FI == 1) || (DAC == 413 && FI == 2) || 
                 (DAC == 413 && FI == 3) || (DAC == 413 && FI == 4))
        {
            bitPosStart = 88;
        }
        if (bitsNum >= bitPosStart && bitPosStart != 0)
        {
            auto text = manager.DecodeToString(bitPosStart, bitsNum - bitPosStart, (int)DAC, (int)FI);
            if (!text.empty())
            {
                UtilityInterface::RemoveTailCharacter(text, '@');

                Message message;

                message.mmsiSource = (uint32_t)mmsiSource;
                message.mmsiDestination = (uint32_t)mmsiDestination;
                message.content = UtilityInterface::GBKToUTF8(text);
                message.timestamp = UtilityInterface::GetCurrentTimeStamp();
                message.messageType = MessageType::AIS; 

                std::unique_lock<std::mutex> lock(m_mutexMessageInbox);
                SaveMessage(MailBox::INBOX, message, MessageBusinessType::NONE, 0);
                lock.unlock();

                m_parent->notifyEvent(EventType::MESSAGE_RECEIVE, 0);
            }
        }
    }

    void VDESManager::Impl::ParseAISMessage8(AISBitsManager &manager)
    {
        auto bitsNum = manager.GetBitsNumberToDecode();
        if (bitsNum > 1008)
        {
            return;
        }

        auto mmsiSource = manager.DecodeToNumerical(8, 30);
        auto ownShipInfo = ConfigureManager::GetInstance().GetOwnVesselInfo();
        if (mmsiSource == ownShipInfo.mmsi)
        {
            //return;
        }
        auto DAC = manager.DecodeToNumerical(40, 10);
        auto FI = manager.DecodeToNumerical(50, 6);

        if ((DAC == 1 && FI == 0) || (DAC == 413 && FI == 1) || 
            (DAC == 413 && FI == 3) || (DAC == 413 && FI == 4))
        {
            auto bitPosStart = 0U;

            if (DAC == 1 && FI == 0)
            {
                bitPosStart = 68;
            }
            else
            {
                bitPosStart = 56;
            }
            if (bitsNum >= bitPosStart)
            {
                auto text = manager.DecodeToString(bitPosStart, bitsNum - bitPosStart, (int)DAC, (int)FI);
                if (text.length())
                {
                    UtilityInterface::RemoveTailCharacter(text, '@');

                    Message message;

                    message.mmsiSource = (uint32_t)mmsiSource;
                    message.mmsiDestination = 0;
                    message.content = UtilityInterface::GBKToUTF8(text);
                    message.deadline = 0;
                    message.timestamp = UtilityInterface::GetCurrentTimeStamp();
                    message.messageType = MessageType::AIS;

                    std::unique_lock<std::mutex> lock(m_mutexMessageInbox);
                    SaveMessage(MailBox::INBOX, message, MessageBusinessType::NONE, 0);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::MESSAGE_RECEIVE, 0);
                }
            }
        }
        else if (DAC == 412 || DAC == 413 || DAC == 414)
        {
            //SPDLOG_DEBUG("ParseAISMessage8: {}", manager.GetEncodedVDMPayload());
            //manager.RemoveBits(0, 40);  
            //m_asmManager.ParsePayload(manager.GetEncodedVDMPayload(), manager.GetFillBitsNumberToEncode());
        }
    }

    void VDESManager::Impl::ParseAISMessage12(const AISBitsManager &manager)
    {
        auto bitsNum = manager.GetBitsNumberToDecode();
        if (bitsNum > 1008)
        {
            return;
        }

        auto mmsiSource = manager.DecodeToNumerical(8, 30);
        auto mmsiDestination = manager.DecodeToNumerical(40, 30);
        auto ownShipInfo = ConfigureManager::GetInstance().GetOwnVesselInfo();
        // Check wheather the destination mmsi is equal to the ownship.
        if (ownShipInfo.mmsi != mmsiDestination)
        {
            return;
        }
        if (bitsNum >= 72)
        {
            auto text = manager.DecodeToString(72, bitsNum - 72);

            UtilityInterface::RemoveTailCharacter(text, '@');

            Message message;

            message.mmsiSource = (uint32_t)mmsiSource;
            message.mmsiDestination = (uint32_t)mmsiDestination;
            message.content = UtilityInterface::GBKToUTF8(text);
            message.timestamp = UtilityInterface::GetCurrentTimeStamp();
            message.messageType = MessageType::AIS;

            std::unique_lock<std::mutex> lock(m_mutexMessageInbox);
            SaveMessage(MailBox::INBOX, message, MessageBusinessType::NONE, 0);
            lock.unlock();

            m_parent->notifyEvent(EventType::MESSAGE_RECEIVE, 0);
        }
    }

    void VDESManager::Impl::ParseAISMessage14(const AISBitsManager &manager)
    {
        auto bitsNum = manager.GetBitsNumberToDecode();
        if (bitsNum > 1008)
        {
            return;
        }

        auto mmsiSource = manager.DecodeToNumerical(8, 30);
        auto ownShipInfo = ConfigureManager::GetInstance().GetOwnVesselInfo();
        if (mmsiSource == ownShipInfo.mmsi)
        {
            return;
        }
        if (bitsNum >= 40)
        {
            auto text = manager.DecodeToString(40, bitsNum - 40);

            UtilityInterface::RemoveTailCharacter(text, '@');

            Message message;

            message.mmsiSource = (uint32_t)mmsiSource;
            message.mmsiDestination = 0;
            message.content = UtilityInterface::GBKToUTF8(text);
            message.timestamp = UtilityInterface::GetCurrentTimeStamp();
            message.messageType = MessageType::AIS;

            std::unique_lock<std::mutex> lock(m_mutexMessageInbox);
            SaveMessage(MailBox::INBOX, message, MessageBusinessType::NONE, 0);
            lock.unlock();

            m_parent->notifyEvent(EventType::MESSAGE_RECEIVE, 0);
        }
    }

    void VDESManager::Impl::ParseAISMessage21(const AISBitsManager &manager)
    {
        AtoN aton;

        aton.mmsi = static_cast<uint32_t>(manager.DecodeToNumerical(8, 30));
        aton.type = static_cast<uint8_t>(manager.DecodeToNumerical(38, 5));
        aton.name = manager.DecodeToString(43, 120);
        aton.coordinate.SetLongitude(DecodeCoordinate(manager, 164, 28));
        aton.coordinate.SetLatitude(DecodeCoordinate(manager, 192, 27));
        aton.refPoint.posA = static_cast<uint16_t>(manager.DecodeToNumerical(219, 9));
        aton.refPoint.posB = static_cast<uint16_t>(manager.DecodeToNumerical(228, 9));
        aton.refPoint.posC = static_cast<uint8_t>(manager.DecodeToNumerical(237, 6));
        aton.refPoint.posD = static_cast<uint8_t>(manager.DecodeToNumerical(243, 6));
        aton.isOffPosition = manager.DecodeToNumerical(259, 1) == 1;
        aton.status = static_cast<uint8_t>(manager.DecodeToNumerical(260, 8));
        aton.isVirtual = manager.DecodeToNumerical(269, 1) == 1;
        aton.isAssigned = manager.DecodeToNumerical(270, 1) == 1;

        auto totalBitNum = manager.GetBitsNumberToDecode();
        auto remainderBitNum = totalBitNum - 272;
        auto extensionNameLength = remainderBitNum / 6;
        if (extensionNameLength > 0)
        {
            auto extensionName = manager.DecodeToString(272, extensionNameLength * 6);
            aton.name.append(extensionName);
        }

        aton.timestamp = UtilityInterface::GetCurrentTimeStamp();
        aton.isCertified = true;

        std::unique_lock<std::mutex> lock(m_mutexAtoN);
        SaveAtoN(aton, true);
        lock.unlock();

        m_parent->notifyEvent(EventType::ATON, 0);
    }

    void VDESManager::Impl::BuildMessage(const uint32_t mmsiSource, const uint32_t mmsiDest,
                                         const MessageBusinessType &messageType, const bool needReply,
                                         const uint16_t sequenceNumber,
                                         const uint64_t deadline)
    {
        Message message;
        // Check integrity and assemble the content in order.
        auto key = fmt::format("{0:09d}{1:09d}", mmsiSource, mmsiDest);
        auto &contentMap = m_messageMap[key];

        auto iter = contentMap.begin();
        while (iter != contentMap.end())
        {
            message.content.append(iter->second);
            iter++;
        }
#if 0
        for (auto i = 0U; i < contentMap.size(); i++)
        {
            if (contentMap.find(i) == contentMap.end())
            {
                m_messageMap.erase(key);
                return;
            }
            message.content.append(contentMap.at(i));
        }
#endif
        message.mmsiSource = mmsiSource;
        message.mmsiDestination = mmsiDest;
        message.isCertified = true;
        message.needAck = needReply;
        message.replyStatus = ReplyStatus::UN_REPLIED;
        message.timestamp = UtilityInterface::GetCurrentTimeStamp();
        message.deadline = deadline;
        m_messageMap.erase(key);

        std::unique_lock<std::mutex> lock(m_mutexMessageInbox);
        SaveMessage(MailBox::INBOX, message, messageType, sequenceNumber);
        lock.unlock();
        m_parent->notifyEvent(EventType::MESSAGE_RECEIVE, 0);
    }

    void VDESManager::Impl::UpdateSentBusinessStatus(SendStatus status, const SendStatusCode statusCode)
    {
        try
        {   
            switch (m_transmissionBusiness)
            {
            case TransmissionBusiness::VTS_CONFIRM:
                if (m_businessIDToBeConfirmed != 0)
                {
                    std::lock_guard<std::mutex> lock(m_mutexVTSReply);

                    if (statusCode == SendStatusCode::SUCCESS ||
                        statusCode == SendStatusCode::SUCCESS_WITH_REPLY)
                    {
                        auto sql = fmt::format("UPDATE VTSRequest SET [Executing Status] = {} WHERE ID = {}",
                                               static_cast<uint32_t>(m_executingStatus), m_businessIDToBeConfirmed);
                        m_database->exec(sql);
                    }
                    m_businessIDToBeConfirmed = 0;
                }
                break;
            case TransmissionBusiness::SIX_BIT_ASCII:
            case TransmissionBusiness::UTF8:
            case TransmissionBusiness::FOURTEEN_BIT:
            case TransmissionBusiness::ADDRESS_MESSAGE_CONFIRM:
                if (m_businessIDToBeConfirmed != 0)
                {
                    std::lock_guard<std::mutex> lock(m_mutexMessageInbox);

                    if (statusCode == SendStatusCode::SUCCESS ||
                        statusCode == SendStatusCode::SUCCESS_WITH_REPLY)
                    {
                        auto sql = fmt::format("UPDATE MessageInbox SET Valid = {} WHERE ID = {}",
                                               static_cast<uint32_t>(m_replyStatus), m_businessIDToBeConfirmed);
                        m_database->exec(sql);
                    }
                    m_businessIDToBeConfirmed = 0;
                }
                else
                {
                    if (m_dataIDLastSentMessage != 0)
                    {
                        std::lock_guard<std::mutex> lock(m_mutexMessageOutbox);

                        if (statusCode != SendStatusCode::NONE)
                        {
                            if (statusCode == SendStatusCode::SUCCESS ||
                                statusCode == SendStatusCode::SUCCESS_WITH_REPLY)
                            {
                                status = SendStatus::SUCCESS;
                            }
                            else if (statusCode == SendStatusCode::FAILURE ||
                                     statusCode == SendStatusCode::QUEUE_FULL ||
                                     statusCode == SendStatusCode::RADIO_ERROR ||
                                     statusCode == SendStatusCode::DATA_LENGTH_ERROR ||
                                     statusCode == SendStatusCode::REFUSED_SERVICE)
                            {
                                status = SendStatus::FAILURE;
                            }
                        }
                        auto sql = fmt::format("UPDATE MessageOutbox SET [Send Status] = {}, [Status Code] = {} "
                                               "WHERE ID = {}",
                                               static_cast<int>(status), static_cast<int>(statusCode),
                                               m_dataIDLastSentMessage);
                        m_database->exec(sql);
                    }
                }
                break;
            }
        }
        catch (const SQLite::Exception &exception)
        {
            DatabaseErrorProcess(exception, "UpdateLastSentMessageStatus");
        }
    }

    uint32_t VDESManager::Impl::UpdateSentVTSRequest(const uint16_t sequenceNo, const uint8_t replyResult)
    {
        if (m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_mutexVTSRequest);

                auto sql = fmt::format("SELECT ID FROM VTSRequest WHERE [Text Sequence Number] IN "
                                       "(SELECT [Text Sequence Number] FROM VTSRequest ORDER BY "
                                       "[Timestamp Request] DESC LIMIT 1 OFFSET 0)");
                SQLite::Statement query(*m_database, sql);
                if (query.executeStep())
                {
                    auto dataID = static_cast<uint32_t>(query.getColumn(0).getInt64());

                    auto sqlUpdate = fmt::format("UPDATE VTSRequest SET [Reply Result] = {} "
                                                 "WHERE ID = {}",
                                                 replyResult, dataID);
                    m_database->exec(sqlUpdate);
                    return dataID;
                }
            }
            catch (const SQLite::Exception &exception)
            {
                DatabaseErrorProcess(exception, "UpdateSentVTSRequest");
            }
        }
        return 0;
    }

    void VDESManager::Impl::LoadInboxFromQueryResult(Message &message, const SQLite::Statement &query)
    {
        message.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        message.mmsiSource = static_cast<uint32_t>(query.getColumn("Source").getInt64());
        message.mmsiDestination = static_cast<uint32_t>(query.getColumn("Destination").getInt64());
        message.isCertified = query.getColumn("Certified").getInt() == 1;
        message.timestamp = query.getColumn("Timestamp Receive").getInt64();
        message.deadline = query.getColumn("Timestamp Deadline").getInt64();
        message.content = query.getColumn("Content").getString();
        message.needAck = query.getColumn("Need Ack").getInt() == 1;
        message.replyStatus = static_cast<ReplyStatus>(query.getColumn("Valid").getInt());
        message.read = query.getColumn("Read").getInt() == 1;
        auto type = query.getColumn("Type").getInt();
        message.messageType = (type == 0) ? MessageType::AIS : MessageType::VDES;
    }

    void VDESManager::Impl::LoadOutboxFromQueryResult(Message &message, const SQLite::Statement &query)
    {
        message.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        message.mmsiSource = static_cast<uint32_t>(query.getColumn("Source").getInt64());
        message.mmsiDestination = static_cast<uint32_t>(query.getColumn("Destination").getInt64());
        message.isCertified = query.getColumn("Certified").getInt() == 1;
        message.timestamp = query.getColumn("Timestamp Send").getInt64();
        message.deadline = query.getColumn("Timestamp Deadline").getInt64();
        message.content = query.getColumn("Content").getString();
        message.needAck = query.getColumn("Need Ack").getInt() == 1;
        message.replyStatus = static_cast<ReplyStatus>(query.getColumn("Valid").getInt());
        message.statusCode = static_cast<SendStatusCode>(query.getColumn("Status Code").getInt());
        message.status = static_cast<SendStatus>(query.getColumn("Send Status").getInt());
    }
    
    void VDESManager::Impl::LoadAtoNFromQueryResult(AtoN &aton, const SQLite::Statement &query)
    {
        aton.mmsi = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        aton.dataID = aton.mmsi;
        aton.coordinate.SetLatitude(query.getColumn("Latitude").getDouble());
        aton.coordinate.SetLongitude(query.getColumn("Longitude").getDouble());
        aton.type = query.getColumn("Type").getInt();
        aton.status = query.getColumn("Status").getInt();
        aton.isVirtual = query.getColumn("Virtual").getInt() == 1;
        aton.isAssigned = query.getColumn("OffPosition").getInt() == 1;
        aton.isCertified = query.getColumn("Certified").getInt() == 1;
        aton.name = query.getColumn("Name").getString();
        aton.timestamp = query.getColumn("Timestamp").getInt64();
        aton.deadline = query.getColumn("Deadline").getInt64();
        aton.read = query.getColumn("Read").getInt() == 1;
    }

    void VDESManager::Impl::SaveAtoN(const AtoN &aton, const bool isVDM)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());
                auto                deadline = aton.deadline;

                if (isVDM)
                {
                    auto sql = fmt::format("SELECT Deadline FROM AtonS WHERE ID = {}", aton.mmsi);
                    SQLite::Statement query(*m_database.get(), sql);
                    if (query.executeStep())
                    {
                        deadline = query.getColumn(0).getInt64();
                    }
                }

                auto sql = fmt::format("REPLACE INTO AtoNs VALUES ({ID}, {Latitude}, {Longitude},"
                    "{Type}, {Status}, {Virtual}, {OffPosition}, {Assigned}, {Certified}, '{Name}',"
                    "{Timestamp}, {Deadline}, {Read})",
                    fmt::arg("ID", aton.mmsi),
                    fmt::arg("Latitude", aton.coordinate.GetLatitude()),
                    fmt::arg("Longitude", aton.coordinate.GetLongitude()),
                    fmt::arg("Type", aton.type),
                    fmt::arg("Status", aton.status),
                    fmt::arg("Virtual", aton.isVirtual ? 1 : 0),
                    fmt::arg("OffPosition", aton.isOffPosition ? 1 : 0),
                    fmt::arg("Assigned", aton.isAssigned ? 1 : 0),
                    fmt::arg("Certified", aton.isCertified ? 1 : 0),
                    fmt::arg("Name", aton.name),
                    fmt::arg("Timestamp", aton.timestamp),
                    fmt::arg("Deadline", deadline),
                    fmt::arg("Read", aton.read ? 1 : 0));
                m_database->exec(sql);

                // Update the bounding box
                sql = fmt::format("REPLACE INTO AtoNBBox VALUES ({ID}, {Left}, {Right}, {Bottom}, {Top})",
                    fmt::arg("ID", aton.mmsi),
                    fmt::arg("Left", aton.coordinate.GetLongitude()),
                    fmt::arg("Right", aton.coordinate.GetLongitude()),
                    fmt::arg("Bottom", aton.coordinate.GetLatitude()),
                    fmt::arg("Top", aton.coordinate.GetLatitude()));
                m_database->exec(sql);
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveAtoN");
            }
        }
    }

    void VDESManager::Impl::DeleteAtoN(const uint32_t mmsi)
    {
        if (m_database)
        {
            try
            {
                auto sql = fmt::format("DELETE FROM AtoNs WHERE ID = {}", mmsi);
                m_database->exec(sql);

                sql = fmt::format("DELETE FROM AtoNBBox WHERE ID = {}", mmsi);
                m_database->exec(sql);
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "DeleteAtoN");
            }
        }
    }

    void VDESManager::Impl::LoadMSIObstacleFromQueryResult(MSIObstacle &obstacle, 
        const SQLite::Statement &query)
    {
        obstacle.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        obstacle.type = static_cast<uint8_t>(query.getColumn("Type").getInt());
        obstacle.coordinate.SetLatitude(query.getColumn("Latitude").getDouble());
        obstacle.coordinate.SetLongitude(query.getColumn("Longitude").getDouble());
        obstacle.range = query.getColumn("Range").getDouble();
        obstacle.timestampStart = query.getColumn("Timestamp Start").getInt64();
        obstacle.timestampEnd = query.getColumn("Timestamp End").getInt64();
        obstacle.comment = static_cast<uint8_t>(query.getColumn("Comment").getInt());
        obstacle.isCertified = query.getColumn("Certified").getInt() == 1;
        obstacle.timestamp = query.getColumn("Timestamp Receive").getInt64();
        obstacle.read = query.getColumn("Read").getInt() == 1;

        obstacle.geometryType = static_cast<uint8_t>(query.getColumn("GeometryType").getInt());
        obstacle.sectorStartAngle = static_cast<uint16_t>(query.getColumn("SectorStartAngle").getInt());
        obstacle.sectorEndAngle = static_cast<uint16_t>(query.getColumn("SectorEndAngle").getInt());
        if (!query.getColumn("Polygon").isNull())
        {
            auto blob = query.getColumn("Polygon").getBlob();
            auto size = query.getColumn("Polygon").size();
            auto pointNum = size / sizeof(CoordinateStruct);
            for (auto i = 0U; i < pointNum; i++)
            {
                auto temp = ((CoordinateStruct *)blob)[i];
                obstacle.polygon.emplace_back(Coordinate(temp.latitude, temp.longitude));
            }
        }
    }

    void VDESManager::Impl::LoadMSIMaritineOperationFromQueryResult(MSIMaritimeOperation &operation, 
        const SQLite::Statement &query)
    {
        operation.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        operation.type = static_cast<uint8_t>(query.getColumn("Type").getInt());
        operation.mmsi = query.getColumn("MMSI").getInt();
        operation.status = static_cast<uint8_t>(query.getColumn("Status").getInt());
        operation.coordinateNE.SetLatitude(query.getColumn("LatitudeNE").getDouble());
        operation.coordinateNE.SetLongitude(query.getColumn("LongitudeNE").getDouble());
        operation.coordinateSW.SetLatitude(query.getColumn("LatitudeSW").getDouble());
        operation.coordinateSW.SetLongitude(query.getColumn("LongitudeSW").getDouble());
        operation.range = query.getColumn("Range").getDouble();
        operation.timestampStart = query.getColumn("Timestamp Start").getInt64();
        operation.timestampEnd = query.getColumn("Timestamp End").getInt64();
        operation.isCertified = query.getColumn("Certified").getInt() == 1;
        operation.timestamp = query.getColumn("Timestamp Receive").getInt64();
        operation.read = query.getColumn("Read").getInt() == 1;

        operation.geometryType = static_cast<uint8_t>(query.getColumn("GeometryType").getInt());
        operation.sectorStartAngle = static_cast<uint16_t>(query.getColumn("SectorStartAngle").getInt());
        operation.sectorEndAngle = static_cast<uint16_t>(query.getColumn("SectorEndAngle").getInt());
        operation.timeType = static_cast<uint8_t>(query.getColumn("TimeType").getInt());
        operation.cautionCode = static_cast<uint8_t>(query.getColumn("Caution").getInt());

        if (!query.getColumn("Polygon").isNull())
        {
            auto blob = query.getColumn("Polygon").getBlob();
            auto size = query.getColumn("Polygon").size();
            auto pointNum = size / sizeof(CoordinateStruct);
            for (auto i = 0U; i < pointNum; i++)
            {
                auto temp = ((CoordinateStruct *)blob)[i];
                operation.polygon.emplace_back(Coordinate(temp.latitude, temp.longitude));
            }
        }
    }

    void VDESManager::Impl::LoadMSIMilitaryActivityFromQueryResult(MSIMilitaryActivity &activity, 
        const SQLite::Statement &query)
    {
        activity.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        activity.theme = static_cast<uint8_t>(query.getColumn("Theme").getInt());
        activity.isContinuing = static_cast<uint8_t>(query.getColumn("Duration").getInt()) == 1;
        auto blob = query.getColumn("Coordinates").getBlob();
        auto size = query.getColumn("Coordinates").size();
        auto pointNum =  size / sizeof(CoordinateStruct);
        for (auto i = 0U; i < pointNum; i++)
        {
            auto temp = ((CoordinateStruct *)blob)[i];
            activity.polygon.emplace_back(Coordinate(temp.latitude, temp.longitude));
        }

        activity.geometryType = static_cast<uint8_t>(query.getColumn("GeometryType").getInt());
        activity.sectorStartAngle = static_cast<uint16_t>(query.getColumn("SectorStartAngle").getInt());
        activity.sectorEndAngle = static_cast<uint16_t>(query.getColumn("SectorEndAngle").getInt());
        activity.range = query.getColumn("Range").getDouble();
        activity.timeType = static_cast<uint8_t>(query.getColumn("TimeType").getInt());

        if ((activity.geometryType == 0 || activity.geometryType == 1) && !activity.polygon.empty())
        {
            activity.coordinateNE = activity.polygon[0];
        }

        activity.timestampStart = query.getColumn("Timestamp Start").getInt64();
        activity.timestampEnd = query.getColumn("Timestamp End").getInt64();
        activity.cautionCode = static_cast<uint8_t>(query.getColumn("Caution").getInt());
        activity.isCertified = query.getColumn("Certified").getInt() == 1;
        activity.timestamp = query.getColumn("Timestamp Receive").getInt64();
        activity.read = query.getColumn("Read").getInt() == 1;
    }

    void VDESManager::Impl::LoadMSIMaritimeDistressFromQueryResult(MSIMaritimeDistress &distress, 
        const SQLite::Statement &query)
    {
        distress.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        distress.type = static_cast<uint8_t>(query.getColumn("Type").getInt());
        distress.situation = static_cast<uint8_t>(query.getColumn("Situation").getInt());
        distress.status = static_cast<uint8_t>(query.getColumn("Status").getInt());
        distress.additional = static_cast<uint8_t>(query.getColumn("Additional").getInt());
        distress.coordinate.SetLatitude(query.getColumn("Latitude").getDouble());
        distress.coordinate.SetLongitude(query.getColumn("Longitude").getDouble());
        distress.timestampValid = query.getColumn("Timestamp").getInt64();
        distress.isCertified = query.getColumn("Certified").getInt() == 1;
        distress.timestamp = query.getColumn("Timestamp Receive").getInt64();
        distress.read = query.getColumn("Read").getInt() == 1;

        distress.statusDescription = static_cast<uint16_t>(query.getColumn("StatusDescription").getInt());
        distress.judgment = static_cast<uint8_t>(query.getColumn("Judgment").getInt());
        distress.locationInstruction = static_cast<uint8_t>(query.getColumn("LocationInstruction").getInt());
        distress.duration = static_cast<uint8_t>(query.getColumn("Duration").getInt());
        distress.cautionCode = static_cast<uint8_t>(query.getColumn("Caution").getInt());
    }

    void VDESManager::Impl::LoadMSIMaritimeTowingFromQueryResult(MSIMaritimeTowing &towing, 
        const SQLite::Statement &query)
    {
        towing.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        towing.mmsi = query.getColumn("MMSI").getInt();
        towing.coordinateStart.SetLatitude(query.getColumn("LatitudeStart").getDouble());
        towing.coordinateStart.SetLongitude(query.getColumn("LongitudeStart").getDouble());
        towing.coordinateEnd.SetLatitude(query.getColumn("LatitudeEnd").getDouble());
        towing.coordinateEnd.SetLongitude(query.getColumn("LongitudeEnd").getDouble());
        towing.length = query.getColumn("Length").getInt();
        towing.width = static_cast<uint8_t>(query.getColumn("Width").getInt());
        towing.speed = query.getColumn("Speed").getDouble();
        towing.timestampStart = query.getColumn("Timestamp Start").getInt64();
        towing.timestampEnd = query.getColumn("Timestamp End").getInt64();
        towing.cautionCode = static_cast<uint8_t>(query.getColumn("Caution").getInt());
        towing.isCertified = query.getColumn("Certified").getInt() == 1;
        towing.timestamp = query.getColumn("Timestamp Receive").getInt64();
        towing.read = query.getColumn("Read").getInt() == 1;
    }

    void VDESManager::Impl::LoadMSIDesignatedAreaFromQueryResult(MSIDesignatedArea &area, 
        const SQLite::Statement &query)
    {
        area.dataID = query.getColumn("ID").getUInt();
        area.MRN = query.getColumn("MRN").getUInt();
        area.fragment = static_cast<uint8_t>(query.getColumn("Fragment").getUInt());
        area.areaType = static_cast<uint8_t>(query.getColumn("AreaType").getUInt());
        area.geometryType = static_cast<uint8_t>(query.getColumn("GeometryType").getUInt());
        area.coordinateNE.SetLatitude(query.getColumn("Latitude").getDouble());
        area.coordinateNE.SetLongitude(query.getColumn("Longitude").getDouble());
        area.range = query.getColumn("Range").getDouble();
        area.sectorStartAngle = static_cast<uint16_t>(query.getColumn("SectorStartAngle").getUInt());
        area.sectorEndAngle = static_cast<uint16_t>(query.getColumn("SectorEndAngle").getUInt());

        auto blob = query.getColumn("Polygon");
        auto size = blob.getBytes();
        if (size > 0)
        {
            auto ptr = (const CoordinateStruct *)blob.getBlob();
            auto count = size / sizeof(CoordinateStruct);
            area.polygon.clear();
            for (auto i = 0U; i < count; i++)
            {
                area.polygon.push_back(Coordinate(ptr[i].latitude, ptr[i].longitude));
            }
        }

        area.timeType = static_cast<uint8_t>(query.getColumn("TimeType").getUInt());
        area.timestampStart = query.getColumn("TimestampStart").getInt64();
        area.timestampEnd = query.getColumn("TimestampEnd").getInt64();
        area.cautionCode = static_cast<uint8_t>(query.getColumn("Caution").getUInt());
        area.isCertified = query.getColumn("Certified").getUInt() != 0;
        area.timestamp = query.getColumn("Timestamp Receive").getInt64();
        area.read = query.getColumn("Read").getUInt() != 0;
    }

    void VDESManager::Impl::LoadBridgeFromQueryResult(Bridge &bridge, 
        const SQLite::Statement &query)
    {
        bridge.MRN = query.getColumn("MRN").getUInt();
        bridge.fragment = static_cast<uint8_t>(query.getColumn("Fragment").getUInt());
        bridge.status = 0;
        bridge.center.SetLatitude(query.getColumn("Latitude").getDouble());
        bridge.center.SetLongitude(query.getColumn("Longitude").getDouble());
        bridge.height = query.getColumn("Height").getDouble();
        bridge.width = query.getColumn("Width").getDouble();
        bridge.directionToPass = static_cast<uint16_t>(query.getColumn("DirectionToPass").getUInt());
        bridge.passAbility = static_cast<uint8_t>(query.getColumn("PassAbility").getUInt());
        bridge.enableMeeting = query.getColumn("EnableMeeting").getUInt() != 0;
        bridge.enableOvertaking = query.getColumn("EnableOvertaking").getUInt() != 0;
        bridge.flowVelocity = static_cast<uint16_t>(query.getColumn("FlowVelocity").getUInt());
        bridge.flowDirection = static_cast<uint16_t>(query.getColumn("FlowDirection").getUInt());
        bridge.timestamp = query.getColumn("Timestamp Receive").getInt64();
    }

    void VDESManager::Impl::LoadChannelCenterlineFromQueryResult(ChannelCenterline &centerline, 
        const SQLite::Statement &query)
    {
        centerline.dataID = query.getColumn("ID").getUInt();
        centerline.MRN = query.getColumn("MRN").getUInt();
        centerline.fragment = static_cast<uint8_t>(query.getColumn("Fragment").getUInt());
        centerline.width = static_cast<uint16_t>(query.getColumn("Width").getUInt());
        centerline.timestamp = query.getColumn("Timestamp Receive").getInt64();

        SQLite::Column column = query.getColumn("Coordinates");
        auto size = column.getBytes();
        if (size > 0)
        {
            auto ptr = (const CoordinateStruct *)column.getBlob();
            auto count = size / sizeof(CoordinateStruct);
            centerline.coordinates.clear();
            for (auto i = 0U; i < count; i++)
            {
                centerline.coordinates.push_back(Coordinate(ptr[i].latitude, ptr[i].longitude));
            }
        }
    }

    void VDESManager::Impl::LoadChannelBoundaryFromQueryResult(ChannelBoundary &boundary, 
        const SQLite::Statement &query)
    {
        boundary.dataID = query.getColumn("ID").getUInt();
        boundary.MRN = query.getColumn("MRN").getUInt();
        boundary.fragment = static_cast<uint8_t>(query.getColumn("Fragment").getUInt());
        boundary.edgeType = static_cast<uint8_t>(query.getColumn("EdgeType").getUInt());
        boundary.timestamp = query.getColumn("Timestamp Receive").getInt64();

        SQLite::Column column = query.getColumn("Coordinates");
        auto size = column.getBytes();
        if (size > 0)
        {
            auto ptr = (const CoordinateStruct *)column.getBlob();
            auto count = size / sizeof(CoordinateStruct);
            boundary.coordinates.clear();
            for (auto i = 0U; i < count; i++)
            {
                boundary.coordinates.push_back(Coordinate(ptr[i].latitude, ptr[i].longitude));
            }
        }
    }

    void VDESManager::Impl::LoadNetSounderFromQueryResult(NetSounder &netSounder, 
        const SQLite::Statement &query)
    {
        netSounder.dataID = query.getColumn("ID").getUInt();
        netSounder.MRN = query.getColumn("MRN").getUInt();
        netSounder.fragment = static_cast<uint8_t>(query.getColumn("Fragment").getUInt());
        netSounder.type = static_cast<uint8_t>(query.getColumn("Type").getUInt());
        netSounder.isContinous = query.getColumn("IsContinous").getUInt() != 0;
        netSounder.timestamp = query.getColumn("Timestamp Receive").getInt64();

        SQLite::Column column = query.getColumn("Coordinates");
        auto size = column.getBytes();
        if (size > 0)
        {
            auto ptr = (const CoordinateStruct *)column.getBlob();
            auto count = size / sizeof(CoordinateStruct);
            netSounder.coordinates.clear();
            for (auto i = 0U; i < count; i++)
            {
                netSounder.coordinates.push_back(Coordinate(ptr[i].latitude, ptr[i].longitude));
            }
        }
    }

    void VDESManager::Impl::LoadFrontendPromptFromQueryResult(FrontendPrompt &prompt, 
        const SQLite::Statement &query)
    {
        prompt.dataID = query.getColumn("ID").getUInt();
        prompt.promptContent = static_cast<uint8_t>(query.getColumn("PromptContent").getUInt());
        prompt.displayDuration = static_cast<uint8_t>(query.getColumn("DisplayDuration").getUInt());
        prompt.text = query.getColumn("Text").getText();
        prompt.timestamp = query.getColumn("Timestamp Receive").getInt64();
    }

    void VDESManager::Impl::LoadHydrometeorologyFromQueryResult(Hydrometeorology &hydro, 
        const SQLite::Statement &query)
    {
        hydro.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        hydro.coordinate.SetLatitude(query.getColumn("Latitude").getDouble());
        hydro.coordinate.SetLongitude(query.getColumn("Longitude").getDouble());
        hydro.timestampHT = query.getColumn("Timestamp HT").getInt64();
        hydro.timestampLT = query.getColumn("Timestamp LT").getInt64();
        hydro.waterLevel = query.getColumn("Water Level").getDouble();
        hydro.source = query.getColumn("Source").getInt();
        hydro.timestampForecast = query.getColumn("Timestamp Forecast").getInt64();
        hydro.isCertified = query.getColumn("Certified").getInt() == 1;
        hydro.timestamp = query.getColumn("Timestamp Receive").getInt64();
        hydro.read = query.getColumn("Read").getInt() == 1;
        hydro.timestampPublished = query.getColumn("Timestamp Published").getInt64();
    }

    void VDESManager::Impl::LoadAbnormalShipFromQueryResult(AbnormalShip &ship, 
        const SQLite::Statement &query)
    {
        auto ID = static_cast<uint32_t>(query.getColumn("ID").getInt());
        ship.dataID = ID;
        ship.mmsi = static_cast<uint32_t>(query.getColumn("MMSI").getInt());
        ship.alertType = static_cast<uint8_t>(query.getColumn("Type").getInt());
        ship.contentAlert = query.getColumn("Content").getString();
        ship.isCertified = query.getColumn("Certified").getInt() == 1;
        ship.timestamp = query.getColumn("Timestamp Receive").getInt64();
        ship.read = query.getColumn("Read").getInt() == 1;

        auto sql = fmt::format("SELECT * FROM CoordinateInfo WHERE ID = {};", ID);
        SQLite::Statement queryTemp(*m_database.get(), sql);
        while (queryTemp.executeStep())
        {
            Coordinate coordinate;
            coordinate.SetLatitude(queryTemp.getColumn("Latitude").getDouble());
            coordinate.SetLongitude(queryTemp.getColumn("Longitude").getDouble());
            auto timestamp = queryTemp.getColumn("Timestamp Alert").getInt64();
            AbnormalShip::CoordinateInfo info;
            info.coordinate = coordinate;
            info.timestampAlert = timestamp;
            ship.coordinateInfos.emplace_back(info);
        }
    }

    void VDESManager::Impl::LoadMarineMeteorologyFCSTFromQueryResult(MarineMeteorologyFCST &fcst, 
        const SQLite::Statement &query)
    {
        fcst.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        fcst.weatherCode = static_cast<uint8_t>(query.getColumn("Weather Code").getInt());
        fcst.coordinate.SetLatitude(query.getColumn("Latitude").getDouble());
        fcst.coordinate.SetLongitude(query.getColumn("Longitude").getDouble());
        fcst.windSpeed = static_cast<uint8_t>(query.getColumn("Wind Speed").getInt());
        fcst.windDirection = static_cast<uint16_t>(query.getColumn("Wind Direction").getInt());
        fcst.temperature = static_cast<int16_t>(query.getColumn("Temperature").getInt());
        fcst.airPressure = static_cast<uint16_t>(query.getColumn("Air Pressure").getInt());
        fcst.visibility = static_cast<uint8_t>(query.getColumn("Visibility").getInt());
        fcst.infoSource = static_cast<uint8_t>(query.getColumn("Info Source").getInt());
        fcst.timestampFCST = static_cast<uint64_t>(query.getColumn("Timestamp Forecast").getInt64());
        fcst.timestamp = static_cast<uint64_t>(query.getColumn("Timestamp Receive").getInt64());
        fcst.read = query.getColumn("Read").getInt() == 1;
    }

    void VDESManager::Impl::LoadMarineEnvironmentFCSTFromQueryResult(MarineEnvironmentFCST &fcst, 
        const SQLite::Statement &query)
    {
        fcst.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        fcst.coordinate.SetLatitude(query.getColumn("Latitude").getDouble());
        fcst.coordinate.SetLongitude(query.getColumn("Longitude").getDouble());
        fcst.flowVelocity = query.getColumn("Flow Velocity").getDouble();
        fcst.flowDirection = static_cast<uint16_t>(query.getColumn("Flow Direction").getInt());
        fcst.waveHeight = static_cast<uint8_t>(query.getColumn("Wave Height").getInt());
        fcst.waveDirection = static_cast<uint16_t>(query.getColumn("Wave Direction").getInt());
        fcst.temperature = static_cast<uint16_t>(query.getColumn("Temperature").getInt());
        fcst.infoSource = static_cast<uint8_t>(query.getColumn("Info Source").getInt());
        fcst.timestampFCST = static_cast<uint64_t>(query.getColumn("Timestamp Forecast").getInt64());
        fcst.timestamp = static_cast<uint64_t>(query.getColumn("Timestamp Receive").getInt64());
        fcst.read = query.getColumn("Read").getInt() == 1;
    }

    void VDESManager::Impl::LoadVTSRequestFromQueryResult(VTSRequest &request, const SQLite::Statement &query)
    {
        request.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        request.requestType = static_cast<uint8_t>(query.getColumn("Request Type").getInt());
        request.timestamp = query.getColumn("Timestamp Request").getInt64();
        request.replyResult = static_cast<uint8_t>(query.getColumn("VTS Reply").getInt());
        request.timestampReply = query.getColumn("Timestamp Reply").getInt64();
        request.executingStatus = static_cast<uint8_t>(query.getColumn("Executing Status").getInt());
    }

    void VDESManager::Impl::LoadVTSReplyFromQueryResult(VTSReply &reply, const SQLite::Statement &query)
    {
        reply.dataID = static_cast<uint32_t>(query.getColumn("ID").getInt64());
        reply.requestDataID = static_cast<uint32_t>(query.getColumn("VTS Request ID").getInt64());
        //reply.textSequenceNo = static_cast<uint32_t>(query.getColumn("[Text Sequence Number]").getInt64());
        reply.isAgreed = query.getColumn("Reply Result").getInt() == 0;
        reply.mmsiRequestor = static_cast<uint32_t>(query.getColumn("Requestor MMSI").getInt64());
        reply.shipNameChnRequestor = query.getColumn("Chinese Remark").getString();
        reply.shipNameEngRequestor = query.getColumn("English Remark").getString();
        reply.queueNo = static_cast<uint8_t>(query.getColumn("Queue Number").getInt());

        auto blob = query.getColumn("Waypoints").getBlob();
        auto size = query.getColumn("Waypoints").size();
        auto pointNum =  size / sizeof(WaypointStruct);
        for (auto i = 0U; i < pointNum; i++)
        {
            auto     temp = ((WaypointStruct *)blob)[i];
            Waypoint waypoint;
            waypoint.coordinate.SetLatitude(temp.coordinate.latitude);
            waypoint.coordinate.SetLongitude(temp.coordinate.longitude);
            waypoint.course = temp.course;
            waypoint.timestamp = temp.timestamp;
            reply.waypoints.emplace_back(waypoint);
        }
    }

    uint32_t VDESManager::Impl::SaveMessage(const MailBox mailbox, const Message &message,
                                            const MessageBusinessType &messageType,
                                            const uint32_t sequenceNumber)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());
                std::string         sql;

                if (mailbox == MailBox::INBOX)
                {
                    sql = fmt::format("REPLACE INTO MessageInbox VALUES ({ID}, {Source}, {Destination}, "
                        "{Type}, {SequenceNum}, '{Content}', {NeedAck}, {Valid}, {Certified}, {TimestampRcv}, "
                        "{Deadline}, {Read})",
                        fmt::arg("ID", (message.dataID == 0) ? "NULL" : fmt::format("{}", message.dataID)),
                        fmt::arg("Source", message.mmsiSource),
                        fmt::arg("Destination", message.mmsiDestination),
                        fmt::arg("Type", static_cast<int>(messageType)),
                        fmt::arg("SequenceNum", sequenceNumber),
                        fmt::arg("Content", message.content),
                        fmt::arg("NeedAck", message.needAck ? 1 : 0),
                        fmt::arg("Valid", static_cast<uint32_t>(message.replyStatus)),
                        fmt::arg("Certified", message.isCertified ? 1 : 0),
                        fmt::arg("TimestampRcv", message.timestamp),
                        fmt::arg("Deadline", message.deadline),
                        fmt::arg("Read", message.read ? 1 : 0));
                    m_database->exec(sql);
                }
                else if (mailbox == MailBox::OUTBOX)
                {
                    sql = fmt::format("REPLACE INTO MessageOutbox VALUES ({ID}, {Source}, {Destination}, "
                        "{Type}, {SequenceNum}, '{Content}', {SendStatus}, {StatusCode}, {NeedAck}, {Valid}, "
                        "{Certified}, {TimestampSend}, {Deadline})",
                        fmt::arg("ID", (message.dataID == 0) ? "NULL" : fmt::format("{}", message.dataID)),
                        fmt::arg("Source", message.mmsiSource),
                        fmt::arg("Destination", message.mmsiDestination),
                        fmt::arg("Type", static_cast<int>(messageType)),
                        fmt::arg("SequenceNum", sequenceNumber),
                        fmt::arg("Content", message.content),
                        fmt::arg("SendStatus", static_cast<int>(message.status)),
                        fmt::arg("StatusCode", static_cast<int>(message.statusCode)),
                        fmt::arg("NeedAck", message.needAck ? 1 : 0),
                        fmt::arg("Valid", static_cast<uint32_t>(message.replyStatus)),
                        fmt::arg("Certified", message.isCertified ? 1 : 0),
                        fmt::arg("TimestampSend", message.timestamp),
                        fmt::arg("Deadline", message.deadline));
                    m_database->exec(sql);
                }
                transaction.commit();
                return message.dataID == 0 ? (uint32_t)m_database->getLastInsertRowid() : message.dataID;
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMessage");
            }
        }
        return 0;
    }    void VDESManager::Impl::SaveMSIObstacle(const MSIObstacle &obstacle)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());
                BoundingBox         bbox;

                auto sql = "REPLACE INTO MSIObstacle VALUES (@ID, @Type, @Latitude, @Longitude, "
                           "@Range, @TimestampStart, @TimestampEnd, @Comment, @Certified, @TimestampRcv, @Read, "
                           "@GeometryType, @SectorStartAngle, @SectorEndAngle, @Polygon)";
                auto stmt = m_database->buildStatement(sql);

                if (obstacle.dataID != 0)
                {
                    stmt.bind("@ID", obstacle.dataID);
                }
                stmt.bind("@Type", obstacle.type);
                stmt.bind("@Latitude", obstacle.coordinate.GetLatitude());
                stmt.bind("@Longitude", obstacle.coordinate.GetLongitude());
                stmt.bind("@Range", obstacle.range);
                stmt.bind("@TimestampStart", obstacle.timestampStart);
                stmt.bind("@TimestampEnd", obstacle.timestampEnd);
                stmt.bind("@Comment", obstacle.comment);
                stmt.bind("@Certified", obstacle.isCertified ? 1 : 0);
                stmt.bind("@TimestampRcv", obstacle.timestamp);
                stmt.bind("@Read", obstacle.read ? 1 : 0);
                stmt.bind("@GeometryType", obstacle.geometryType);
                stmt.bind("@SectorStartAngle", obstacle.sectorStartAngle);
                stmt.bind("@SectorEndAngle", obstacle.sectorEndAngle);

                if (obstacle.geometryType == 2 || obstacle.geometryType == 3)
                {
                    if (!obstacle.polygon.empty())
                    {
                        auto polygon = std::unique_ptr<CoordinateStruct[]>(new CoordinateStruct[obstacle.polygon.size()]);
                        if (polygon)
                        {
                            for (auto i = 0U; i < obstacle.polygon.size(); i++)
                            {
                                polygon[i].latitude = obstacle.polygon[i].GetLatitude();
                                polygon[i].longitude = obstacle.polygon[i].GetLongitude();
                                bbox.Update(polygon[i].latitude, polygon[i].longitude);
                            }
                            auto size = obstacle.polygon.size() * sizeof(CoordinateStruct);
                            stmt.bind("@Polygon", (const uint8_t *)polygon.get(), static_cast<int>(size));
                        }
                    }
                    else
                    {
                        stmt.bind("@Polygon", nullptr);
                        bbox.Update(obstacle.coordinate.GetLatitude(), obstacle.coordinate.GetLongitude());
                    }
                }
                else
                {
                    stmt.bind("@Polygon", nullptr);
                    bbox.Update(obstacle.coordinate.GetLatitude(), obstacle.coordinate.GetLongitude());
                }

                stmt.exec();
                auto lastRowID = m_database->getLastInsertRowid();

                // Update the bounding box
                auto rects = bbox.GetRects();
                for (auto &rect : rects)
                {
                    auto ID = rect.right >= 0 ? lastRowID : -lastRowID;
                    auto sqlBBox = fmt::format("REPLACE INTO MSIObstacleBBox VALUES ({ID}, {Left}, {Right}, {Bottom}, {Top})",
                        fmt::arg("ID", ID),
                        fmt::arg("Left", rect.left),
                        fmt::arg("Right", rect.right),
                        fmt::arg("Bottom", rect.bottom),
                        fmt::arg("Top", rect.top));
                    m_database->exec(sqlBBox);
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMSIObstacle");
            }
        }
    }

    void VDESManager::Impl::SaveMSIMaritineOperation(const MSIMaritimeOperation &operation)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                auto sql = "REPLACE INTO MSIMaritimeOperation VALUES (@ID, @Type, @Status, @MMSI, "
                           "@LatitudeNE, @LongitudeNE, @LatitudeSW, @LongitudeSW, @Range, "
                           "@TimestampStart, @TimestampEnd, @Certified, @TimestampRcv, @Read, "
                           "@GeometryType, @SectorStartAngle, @SectorEndAngle, @Polygon, @TimeType, @Caution)";
                auto stmt = m_database->buildStatement(sql);

                if (operation.dataID != 0)
                {
                    stmt.bind("@ID", operation.dataID);
                }
                stmt.bind("@Type", operation.type);
                stmt.bind("@Status", operation.status);
                stmt.bind("@MMSI", operation.mmsi);
                stmt.bind("@LatitudeNE", operation.coordinateNE.GetLatitude());
                stmt.bind("@LongitudeNE", operation.coordinateNE.GetLongitude());
                stmt.bind("@LatitudeSW", operation.coordinateSW.GetLatitude());
                stmt.bind("@LongitudeSW", operation.coordinateSW.GetLongitude());
                stmt.bind("@Range", operation.range);
                stmt.bind("@TimestampStart", operation.timestampStart);
                stmt.bind("@TimestampEnd", operation.timestampEnd);
                stmt.bind("@Certified", operation.isCertified ? 1 : 0);
                stmt.bind("@TimestampRcv", operation.timestamp);
                stmt.bind("@Read", operation.read ? 1 : 0);
                stmt.bind("@GeometryType", operation.geometryType);
                stmt.bind("@SectorStartAngle", operation.sectorStartAngle);
                stmt.bind("@SectorEndAngle", operation.sectorEndAngle);
                stmt.bind("@TimeType", operation.timeType);
                stmt.bind("@Caution", operation.cautionCode);

                BoundingBox bbox;
                if (operation.geometryType == 2 || operation.geometryType == 3)
                {
                    if (!operation.polygon.empty())
                    {
                        auto polygon = std::unique_ptr<CoordinateStruct[]>(new CoordinateStruct[operation.polygon.size()]);
                        if (polygon)
                        {
                            for (auto i = 0U; i < operation.polygon.size(); i++)
                            {
                                polygon[i].latitude = operation.polygon[i].GetLatitude();
                                polygon[i].longitude = operation.polygon[i].GetLongitude();
                                bbox.Update(polygon[i].latitude, polygon[i].longitude);
                            }
                            auto size = operation.polygon.size() * sizeof(CoordinateStruct);
                            stmt.bind("@Polygon", (const uint8_t *)polygon.get(), static_cast<int>(size));
                        }
                    }
                    else
                    {
                        stmt.bind("@Polygon", nullptr);
                        bbox.Update(operation.coordinateNE);
                    }
                }
                else
                {
                    stmt.bind("@Polygon", nullptr);
                    bbox.Update(operation.coordinateNE);
                }

                stmt.exec();
                auto lastRowID = m_database->getLastInsertRowid();

                // Update the bounding box
                auto rects = bbox.GetRects();
                for (auto &rect : rects)
                {
                    auto ID = rect.right >= 0 ? lastRowID : -lastRowID;
                    auto sqlBBox = fmt::format("REPLACE INTO MSIMaritimeOperationBBox VALUES ({ID}, {Left}, {Right}, {Bottom}, {Top})",
                        fmt::arg("ID", ID),
                        fmt::arg("Left", rect.left),
                        fmt::arg("Right", rect.right),
                        fmt::arg("Bottom", rect.bottom),
                        fmt::arg("Top", rect.top));
                    m_database->exec(sqlBBox);
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMSIMaritineOperation");
            }
        }
    }

    void VDESManager::Impl::SaveMSIMilitaryActivity(const MSIMilitaryActivity &activity)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());
                std::unique_ptr<BoundingBox> bbox;

                auto sql = "REPLACE INTO MSIMilitaryActivity VALUES (@ID, @Theme, @Duration, @Polygon,"
                    "@TimestampStart, @TimestampEnd, @Caution, @Certified, @TimestampRcv, @Read, "
                    "@GeometryType, @SectorStartAngle, @SectorEndAngle, @Range, @TimeType)";
                auto stmt = m_database->buildStatement(sql);
                
                if (activity.dataID != 0)
                {
                    stmt.bind("@ID", activity.dataID);
                }
                stmt.bind("@Theme", activity.theme);
                stmt.bind("@Duration", activity.isContinuing);

                // Handle coordinates blob and bbox
                std::vector<Coordinate> pts = activity.polygon;
                if ((activity.geometryType == 0 || activity.geometryType == 1) && pts.empty())
                {
                    pts.push_back(activity.coordinateNE);
                }

                auto polygon = std::unique_ptr<CoordinateStruct[]>(new CoordinateStruct[pts.size()]);
                if (polygon)
                {
                    for (auto i = 0U; i < pts.size(); i++)
                    {
                        polygon[i].latitude = pts[i].GetLatitude();
                        polygon[i].longitude = pts[i].GetLongitude();
                    }
                    auto size = pts.size() * sizeof(CoordinateStruct);
                    stmt.bind("@Polygon", (const uint8_t *)polygon.get(), static_cast<int>(size));
                }

                if (activity.geometryType == 0 || activity.geometryType == 1)
                {
                    bbox = UtilityInterface::MakeUnique<BoundingBox>(BoundingBox::Build(activity.coordinateNE.GetLatitude(), 
                                                                             activity.coordinateNE.GetLongitude(), 
                                                                             activity.range));
                }
                else
                {
                    bbox = UtilityInterface::MakeUnique<BoundingBox>();
                    for (const auto &pt : pts)
                    {
                        bbox->Update(pt.GetLatitude(), pt.GetLongitude());
                    }
                }

                stmt.bind("@TimestampStart", activity.timestampStart);
                stmt.bind("@TimestampEnd", activity.timestampEnd);
                stmt.bind("@Caution", activity.cautionCode);
                stmt.bind("@Certified", activity.isCertified ? 1 : 0);
                stmt.bind("@TimestampRcv", activity.timestamp);
                stmt.bind("@Read", activity.read ? 1 : 0);

                stmt.bind("@GeometryType", activity.geometryType);
                stmt.bind("@SectorStartAngle", activity.sectorStartAngle);
                stmt.bind("@SectorEndAngle", activity.sectorEndAngle);
                stmt.bind("@Range", activity.range);
                stmt.bind("@TimeType", activity.timeType);

                stmt.exec();

                auto lastRowID = m_database->getLastInsertRowid();

                // Update the bounding box
                auto rects = bbox->GetRects();
                for (auto &rect : rects)
                {
                    auto ID = rect.right >= 0 ? lastRowID : -lastRowID;
                    auto sqlBBox = fmt::format("REPLACE INTO MSIMilitaryActivityBBox VALUES ({ID}, {Left}, {Right}, {Bottom}, {Top})",
                        fmt::arg("ID", ID),
                        fmt::arg("Left", rect.left),
                        fmt::arg("Right", rect.right),
                        fmt::arg("Bottom", rect.bottom),
                        fmt::arg("Top", rect.top));
                    m_database->exec(sqlBBox);
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMSIMilitaryActivity");
            }
        }
    }

    void VDESManager::Impl::SaveMSIMaritimeDistress(const MSIMaritimeDistress &distress)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                auto sql = "REPLACE INTO MSIMaritimeDistress VALUES (@ID, @Type, @Situation, @Status,"
                           "@Additional, @Latitude, @Longitude, @TimestampValid, @Certified, @TimestampRcv, @Read, "
                           "@StatusDescription, @Judgment, @LocationInstruction, @Duration, @Caution)";
                auto stmt = m_database->buildStatement(sql);

                if (distress.dataID != 0)
                {
                    stmt.bind("@ID", distress.dataID);
                }
                stmt.bind("@Type", distress.type);
                stmt.bind("@Situation", distress.situation);
                stmt.bind("@Status", distress.status);
                stmt.bind("@Additional", distress.additional);
                stmt.bind("@Latitude", distress.coordinate.GetLatitude());
                stmt.bind("@Longitude", distress.coordinate.GetLongitude());
                stmt.bind("@TimestampValid", distress.timestampValid);
                stmt.bind("@Certified", distress.isCertified ? 1 : 0);
                stmt.bind("@TimestampRcv", distress.timestamp);
                stmt.bind("@Read", distress.read ? 1 : 0);
                stmt.bind("@StatusDescription", distress.statusDescription);
                stmt.bind("@Judgment", distress.judgment);
                stmt.bind("@LocationInstruction", distress.locationInstruction);
                stmt.bind("@Duration", distress.duration);
                stmt.bind("@Caution", distress.cautionCode);

                stmt.exec();
                auto lastRowID = m_database->getLastInsertRowid();

                // Update the bounding box
                auto sqlBBox = fmt::format("REPLACE INTO MSIMaritimeDistressBBox VALUES ({ID}, {Left}, {Right}, {Bottom}, {Top})",
                    fmt::arg("ID", lastRowID),
                    fmt::arg("Left", distress.coordinate.GetLongitude()),
                    fmt::arg("Right", distress.coordinate.GetLongitude()),
                    fmt::arg("Bottom", distress.coordinate.GetLatitude()),
                    fmt::arg("Top", distress.coordinate.GetLatitude()));
                m_database->exec(sqlBBox);
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMSIMaritimeDistress");
            }
        }
    }

    void VDESManager::Impl::SaveMSIMaritimeTowing(const MSIMaritimeTowing &towing)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());
                BoundingBox         bbox;

                auto sql = "REPLACE INTO MSIMaritimeTowing VALUES (@ID, @MMSI, @LatitudeStart, @LongitudeStart, "
                           "@LatitudeEnd, @LongitudeEnd, @Length, @Width, @Speed, @TimestampStart, @TimestampEnd, "
                           "@Caution, @Certified, @TimestampRcv, @Read)";
                auto stmt = m_database->buildStatement(sql);

                if (towing.dataID != 0)
                {
                    stmt.bind("@ID", towing.dataID);
                }
                stmt.bind("@MMSI", towing.mmsi);
                stmt.bind("@LatitudeStart", towing.coordinateStart.GetLatitude());
                stmt.bind("@LongitudeStart", towing.coordinateStart.GetLongitude());
                stmt.bind("@LatitudeEnd", towing.coordinateEnd.GetLatitude());
                stmt.bind("@LongitudeEnd", towing.coordinateEnd.GetLongitude());
                stmt.bind("@Length", towing.length);
                stmt.bind("@Width", towing.width);
                stmt.bind("@Speed", towing.speed);
                stmt.bind("@TimestampStart", towing.timestampStart);
                stmt.bind("@TimestampEnd", towing.timestampEnd);
                stmt.bind("@Caution", towing.cautionCode);
                stmt.bind("@Certified", towing.isCertified ? 1 : 0);
                stmt.bind("@TimestampRcv", towing.timestamp);
                stmt.bind("@Read", towing.read ? 1 : 0);

                stmt.exec();
                auto lastRowID = m_database->getLastInsertRowid();

                // Update bounding box (includes start & end points)
                bbox.Update(towing.coordinateStart);
                bbox.Update(towing.coordinateEnd);

                auto rects = bbox.GetRects();
                for (auto &rect : rects)
                {
                    auto ID = rect.right >= 0 ? lastRowID : -lastRowID;
                    auto sqlBBox = fmt::format("REPLACE INTO MSIMaritimeTowingBBox VALUES ({ID}, {Left}, {Right}, {Bottom}, {Top})",
                        fmt::arg("ID", ID),
                        fmt::arg("Left", rect.left),
                        fmt::arg("Right", rect.right),
                        fmt::arg("Bottom", rect.bottom),
                        fmt::arg("Top", rect.top));
                    m_database->exec(sqlBBox);
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMSIMaritimeTowing");
            }
        }
    }

    void VDESManager::Impl::SaveMSIDesignatedArea(const MSIDesignatedArea &area)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());
                std::unique_ptr<BoundingBox> bbox;

                auto sql = "REPLACE INTO MSIDesignatedArea VALUES (@ID, @MRN, @Fragment, @AreaType, "
                           "@GeometryType, @Latitude, @Longitude, @Range, @SectorStartAngle, @SectorEndAngle, "
                           "@Polygon, @TimeType, @TimestampStart, @TimestampEnd, @Caution, @Certified, "
                           "@TimestampRcv, @Read)";
                auto stmt = m_database->buildStatement(sql);

                if (area.dataID != 0)
                {
                    stmt.bind("@ID", area.dataID);
                }
                stmt.bind("@MRN", area.MRN);
                stmt.bind("@Fragment", area.fragment);
                stmt.bind("@AreaType", area.areaType);
                stmt.bind("@GeometryType", area.geometryType);
                stmt.bind("@Latitude", area.coordinateNE.GetLatitude());
                stmt.bind("@Longitude", area.coordinateNE.GetLongitude());
                stmt.bind("@Range", area.range);
                stmt.bind("@SectorStartAngle", area.sectorStartAngle);
                stmt.bind("@SectorEndAngle", area.sectorEndAngle);

                std::vector<Coordinate> pts = area.polygon;
                if ((area.geometryType == 0 || area.geometryType == 1) && pts.empty())
                {
                    pts.push_back(area.coordinateNE);
                }

                auto polygon = std::unique_ptr<CoordinateStruct[]>(new CoordinateStruct[pts.size()]);
                if (polygon)
                {
                    for (auto i = 0U; i < pts.size(); i++)
                    {
                        polygon[i].latitude = pts[i].GetLatitude();
                        polygon[i].longitude = pts[i].GetLongitude();
                    }
                    auto size = pts.size() * sizeof(CoordinateStruct);
                    stmt.bind("@Polygon", (const uint8_t *)polygon.get(), static_cast<int>(size));
                }

                if (area.geometryType == 0 || area.geometryType == 1)
                {
                    bbox = UtilityInterface::MakeUnique<BoundingBox>(BoundingBox::Build(area.coordinateNE.GetLatitude(), 
                                                                             area.coordinateNE.GetLongitude(), 
                                                                             area.range));
                }
                else
                {
                    bbox = UtilityInterface::MakeUnique<BoundingBox>();
                    for (const auto &pt : pts)
                    {
                        bbox->Update(pt.GetLatitude(), pt.GetLongitude());
                    }
                }

                stmt.bind("@TimeType", area.timeType);
                stmt.bind("@TimestampStart", area.timestampStart);
                stmt.bind("@TimestampEnd", area.timestampEnd);
                stmt.bind("@Caution", area.cautionCode);
                stmt.bind("@Certified", area.isCertified ? 1 : 0);
                stmt.bind("@TimestampRcv", area.timestamp);
                stmt.bind("@Read", area.read ? 1 : 0);

                stmt.exec();

                auto lastRowID = m_database->getLastInsertRowid();

                // Update the bounding box
                auto rects = bbox->GetRects();
                for (auto &rect : rects)
                {
                    auto ID = rect.right >= 0 ? lastRowID : -lastRowID;
                    auto sqlBBox = fmt::format("REPLACE INTO MSIDesignatedAreaBBox VALUES ({ID}, {Left}, {Right}, {Bottom}, {Top})",
                        fmt::arg("ID", ID),
                        fmt::arg("Left", rect.left),
                        fmt::arg("Right", rect.right),
                        fmt::arg("Bottom", rect.bottom),
                        fmt::arg("Top", rect.top));
                    m_database->exec(sqlBBox);
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMSIDesignatedArea");
            }
        }
    }

    void VDESManager::Impl::SaveBridge(const Bridge &bridge)
    {
        if (m_database)
        {
            try
            {
                auto sql = "INSERT INTO Bridge (MRN, Fragment, Latitude, Longitude, Height, Width, "
                           "DirectionToPass, PassAbility, EnableMeeting, EnableOvertaking, "
                           "FlowVelocity, FlowDirection, [Timestamp Receive]) "
                           "VALUES (@MRN, @Fragment, @Latitude, @Longitude, @Height, @Width, "
                           "@DirectionToPass, @PassAbility, @EnableMeeting, @EnableOvertaking, "
                           "@FlowVelocity, @FlowDirection, @TimestampRcv)";
                auto stmt = m_database->buildStatement(sql);

                stmt.bind("@MRN", bridge.MRN);
                stmt.bind("@Fragment", bridge.fragment);
                stmt.bind("@Latitude", bridge.center.GetLatitude());
                stmt.bind("@Longitude", bridge.center.GetLongitude());
                stmt.bind("@Height", bridge.height);
                stmt.bind("@Width", bridge.width);
                stmt.bind("@DirectionToPass", bridge.directionToPass);
                stmt.bind("@PassAbility", bridge.passAbility);
                stmt.bind("@EnableMeeting", bridge.enableMeeting ? 1 : 0);
                stmt.bind("@EnableOvertaking", bridge.enableOvertaking ? 1 : 0);
                stmt.bind("@FlowVelocity", bridge.flowVelocity);
                stmt.bind("@FlowDirection", bridge.flowDirection);
                stmt.bind("@TimestampRcv", bridge.timestamp);

                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveBridge");
            }
        }
    }

    void VDESManager::Impl::SaveChannelCenterline(const ChannelCenterline &centerline)
    {
        if (m_database)
        {
            try
            {
                auto sql = "INSERT INTO ChannelCenterline (MRN, Fragment, Width, Coordinates, [Timestamp Receive]) "
                           "VALUES (@MRN, @Fragment, @Width, @Coordinates, @TimestampRcv)";
                auto stmt = m_database->buildStatement(sql);

                stmt.bind("@MRN", centerline.MRN);
                stmt.bind("@Fragment", centerline.fragment);
                stmt.bind("@Width", centerline.width);

                auto pointsCount = centerline.coordinates.size();
                auto size = pointsCount * sizeof(CoordinateStruct);
                auto blob = std::unique_ptr<CoordinateStruct[]>(new CoordinateStruct[pointsCount]);
                for (size_t i = 0; i < pointsCount; ++i)
                {
                    blob[i].latitude = centerline.coordinates[i].GetLatitude();
                    blob[i].longitude = centerline.coordinates[i].GetLongitude();
                }
                stmt.bind("@Coordinates", blob.get(), size);
                stmt.bind("@TimestampRcv", centerline.timestamp);

                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveChannelCenterline");
            }
        }
    }

    void VDESManager::Impl::SaveChannelBoundary(const ChannelBoundary &boundary)
    {
        if (m_database)
        {
            try
            {
                auto sql = "INSERT INTO ChannelBoundary (MRN, Fragment, EdgeType, Coordinates, [Timestamp Receive]) "
                           "VALUES (@MRN, @Fragment, @EdgeType, @Coordinates, @TimestampRcv)";
                auto stmt = m_database->buildStatement(sql);

                stmt.bind("@MRN", boundary.MRN);
                stmt.bind("@Fragment", boundary.fragment);
                stmt.bind("@EdgeType", boundary.edgeType);

                auto pointsCount = boundary.coordinates.size();
                auto size = pointsCount * sizeof(CoordinateStruct);
                auto blob = std::unique_ptr<CoordinateStruct[]>(new CoordinateStruct[pointsCount]);
                for (size_t i = 0; i < pointsCount; ++i)
                {
                    blob[i].latitude = boundary.coordinates[i].GetLatitude();
                    blob[i].longitude = boundary.coordinates[i].GetLongitude();
                }
                stmt.bind("@Coordinates", blob.get(), size);
                stmt.bind("@TimestampRcv", boundary.timestamp);

                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveChannelBoundary");
            }
        }
    }

    void VDESManager::Impl::SaveNetSounder(const NetSounder &netSounder)
    {
        if (m_database)
        {
            try
            {
                auto sql = "INSERT INTO NetSounder (MRN, Fragment, Type, IsContinous, Coordinates, [Timestamp Receive]) "
                           "VALUES (@MRN, @Fragment, @Type, @IsContinous, @Coordinates, @TimestampRcv)";
                auto stmt = m_database->buildStatement(sql);

                stmt.bind("@MRN", netSounder.MRN);
                stmt.bind("@Fragment", netSounder.fragment);
                stmt.bind("@Type", netSounder.type);
                stmt.bind("@IsContinous", netSounder.isContinous ? 1 : 0);

                auto pointsCount = netSounder.coordinates.size();
                auto size = pointsCount * sizeof(CoordinateStruct);
                auto blob = std::unique_ptr<CoordinateStruct[]>(new CoordinateStruct[pointsCount]);
                for (size_t i = 0; i < pointsCount; ++i)
                {
                    blob[i].latitude = netSounder.coordinates[i].GetLatitude();
                    blob[i].longitude = netSounder.coordinates[i].GetLongitude();
                }
                stmt.bind("@Coordinates", blob.get(), size);
                stmt.bind("@TimestampRcv", netSounder.timestamp);

                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveNetSounder");
            }
        }
    }

    void VDESManager::Impl::SaveFrontendPrompt(const FrontendPrompt &prompt)
    {
        if (m_database)
        {
            try
            {
                auto sqlDel = "DELETE FROM FrontendPrompt WHERE PromptContent = @PromptContent AND Text = @Text";
                auto stmtDel = m_database->buildStatement(sqlDel);
                stmtDel.bind("@PromptContent", prompt.promptContent);
                stmtDel.bind("@Text", prompt.text);
                stmtDel.exec();

                auto sql = "INSERT INTO FrontendPrompt (PromptContent, DisplayDuration, Text, [Timestamp Receive], [Timestamp Expiry]) "
                           "VALUES (@PromptContent, @DisplayDuration, @Text, @TimestampRcv, @TimestampExp)";
                auto stmt = m_database->buildStatement(sql);

                stmt.bind("@PromptContent", prompt.promptContent);
                stmt.bind("@DisplayDuration", prompt.displayDuration);
                stmt.bind("@Text", prompt.text);
                stmt.bind("@TimestampRcv", prompt.timestamp);

                int64_t timestampExpiry = 0;
                if (prompt.displayDuration > 0)
                {
                    timestampExpiry = prompt.timestamp + prompt.displayDuration * 60;
                }
                stmt.bind("@TimestampExp", timestampExpiry);

                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveFrontendPrompt");
            }
        }
    }

    void VDESManager::Impl::SaveHydrometeoroloies(const Hydrometeorologies &container)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                for (auto &hydro : container)
                {
                    auto sql = fmt::format("REPLACE INTO Hydrometeorology VALUES ({ID}, {Latitude}, {Longitude}, "
                        "{TimestampHT}, {TimestampLT}, {WaterLevel}, {Source}, {TimestampFore}, {Certified}, "
                        "{TimestampRcv}, {Read}, {TimestampPublished})",
                        fmt::arg("ID", (hydro.dataID == 0) ? "NULL" : fmt::format("{}", hydro.dataID)),
                        fmt::arg("Latitude", hydro.coordinate.GetLatitude()),
                        fmt::arg("Longitude", hydro.coordinate.GetLongitude()),
                        fmt::arg("TimestampHT", hydro.timestampHT),
                        fmt::arg("TimestampLT", hydro.timestampLT),
                        fmt::arg("WaterLevel", hydro.waterLevel),
                        fmt::arg("Source", hydro.source),
                        fmt::arg("TimestampFore", hydro.timestampForecast),
                        fmt::arg("Certified", hydro.isCertified ? 1 : 0),
                        fmt::arg("TimestampRcv", hydro.timestamp),
                        fmt::arg("Read", hydro.read ? 1 : 0),
                        fmt::arg("TimestampPublished", hydro.timestampPublished));

                    m_database->exec(sql);
                    auto lastRowID = m_database->getLastInsertRowid();

                    // Update the bounding box
                    sql = fmt::format("REPLACE INTO HydrometeorologyBBox VALUES ({ID}, {Left}, {Right}, {Bottom}, {Top})",
                        fmt::arg("ID", lastRowID),
                        fmt::arg("Left", hydro.coordinate.GetLongitude()),
                        fmt::arg("Right", hydro.coordinate.GetLongitude()),
                        fmt::arg("Bottom", hydro.coordinate.GetLatitude()),
                        fmt::arg("Top", hydro.coordinate.GetLatitude()));
                    m_database->exec(sql);
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveHydrometeoroloies");
            }
        }
    }

    void VDESManager::Impl::SaveAbnormalShip(const AbnormalShip &ship)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                auto sql = fmt::format("INSERT INTO AbnormalShip VALUES (NULL, {MMSI}, {Type}, '{Content}', "
                    "{Certified}, {TimestampRcv}, {Read})",
                    fmt::arg("MMSI", ship.mmsi),
                    fmt::arg("Type", ship.alertType),
                    fmt::arg("Content", ship.contentAlert),
                    fmt::arg("Certified", ship.isCertified ? 1 : 0),
                    fmt::arg("TimestampRcv", ship.timestamp),
                    fmt::arg("Read", ship.read ? 1 : 0));
                m_database->exec(sql);
                auto rowID = m_database->getLastInsertRowid();
               
                for (auto &info : ship.coordinateInfos)
                {
                    sql = fmt::format("INSERT INTO CoordinateInfo VALUES ({ID}, {Latitude}, {Longitude}, "
                        "{TimestampAlert})",
                        fmt::arg("ID", rowID),
                        fmt::arg("Latitude", info.coordinate.GetLatitude()),
                        fmt::arg("Longitude", info.coordinate.GetLongitude()),
                        fmt::arg("TimestampAlert", info.timestampAlert));
                    m_database->exec(sql);
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveAbnormalShip");
            }
        }
    }

    void VDESManager::Impl::SaveMarineMeteorologyFCST(const MarineMeteorologyFCSTs &container)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                // Delete data from three days ago.
                auto timeZone = UtilityInterface::GetTimeZone();
                auto timestampNow = UtilityInterface::GetCurrentTimeStamp();
                auto timestampDayBegin = timestampNow - (timestampNow % (24 * 3600)) - timeZone;
                auto timestampThreeDaysAgo = timestampDayBegin - 3 * 24 * 3600;
                auto sql = fmt::format("DELETE FROM MarineMeteorologyFCST WHERE [Timestamp Forecast] <= {}",
                                       timestampThreeDaysAgo);
                m_database->exec(sql);

                auto sqlInfo = "REPLACE INTO MarineMeteorologyFCST VALUES (@ID, @WeatherCode, @Latitude, "
                    "@Longitude, @WindSpeed, @WindDirection, @Temperature, @AirPressure, @Visibility, "
                    "@InfoSource, @TimestampFCST, @TimestampRcv, @Read)";
                auto stmtUpdateInfo = m_database->buildStatement(sqlInfo);

                auto sqlBBox = "REPLACE INTO MeteorologyFCSTBBox VALUES (@ID, @Left, @Right, @Bottom, @Top)";
                auto stmtUpdateBBox = m_database->buildStatement(sqlBBox);

                for (auto &fcstInfo : container)
                {
                    auto longitude = fcstInfo.coordinate.GetLongitude();
                    auto latitude = fcstInfo.coordinate.GetLatitude();

                    int64_t dataID = 0;
                    auto sqlCheck = fmt::format("SELECT ID FROM MarineMeteorologyFCST WHERE ABS(Latitude - {}) < 0.00001 AND ABS(Longitude - {}) < 0.00001;", latitude, longitude);
                    SQLite::Statement queryCheck(*m_database.get(), sqlCheck);
                    if (queryCheck.executeStep())
                    {
                        dataID = queryCheck.getColumn(0).getInt64();
                    }

                    if (dataID != 0)
                    {
                        stmtUpdateInfo.bind("@ID", dataID);
                    }
                    else if (fcstInfo.dataID != 0)
                    {
                        stmtUpdateInfo.bind("@ID", fcstInfo.dataID);
                    }
                    stmtUpdateInfo.bind("@WeatherCode", fcstInfo.weatherCode);
                    stmtUpdateInfo.bind("@Latitude", latitude);
                    stmtUpdateInfo.bind("@Longitude", longitude);
                    stmtUpdateInfo.bind("@WindSpeed", fcstInfo.windSpeed);
                    stmtUpdateInfo.bind("@WindDirection", fcstInfo.windDirection);
                    stmtUpdateInfo.bind("@Temperature", fcstInfo.temperature);
                    stmtUpdateInfo.bind("@AirPressure", fcstInfo.airPressure);
                    stmtUpdateInfo.bind("@Visibility", fcstInfo.visibility);
                    stmtUpdateInfo.bind("@InfoSource", fcstInfo.infoSource);
                    stmtUpdateInfo.bind("@TimestampFCST", fcstInfo.timestampFCST);
                    stmtUpdateInfo.bind("@TimestampRcv", fcstInfo.timestamp);
                    stmtUpdateInfo.bind("@Read", fcstInfo.read ? 1 : 0);
                    stmtUpdateInfo.exec();
                    stmtUpdateInfo.reset();

                    // Update the bounding box
                    auto lastRowID = m_database->getLastInsertRowid();
                    stmtUpdateBBox.bind("@ID", lastRowID);
                    stmtUpdateBBox.bind("@Left", longitude);
                    stmtUpdateBBox.bind("@Right", longitude);
                    stmtUpdateBBox.bind("@Bottom", latitude);
                    stmtUpdateBBox.bind("@Top", latitude);
                    stmtUpdateBBox.exec();
                    stmtUpdateBBox.reset();
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMarineMeteorologyFCST");
            }
        }
    }

    void VDESManager::Impl::SaveMarineEnvironmentFCST(const MarineEnvironmentFCSTs &container)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                // Delete data from three days ago.
                auto timeZone = UtilityInterface::GetTimeZone();
                auto timestampNow = UtilityInterface::GetCurrentTimeStamp();
                auto timestampDayBegin = timestampNow - (timestampNow % (24 * 3600)) - timeZone;
                auto timestampThreeDaysAgo = timestampDayBegin - 3 * 24 * 3600;
                auto sql = fmt::format("DELETE FROM MarineEnvironmentFCST WHERE [Timestamp Forecast] <= {}",
                    timestampThreeDaysAgo);
                m_database->exec(sql);

                auto sqlInfo = "REPLACE INTO MarineEnvironmentFCST VALUES (@ID, @Latitude, @Longitude, @FlowVelocity, "
                               "@FlowDirection, @WaveHeight, @WaveDirection, @Temperature, @InfoSource, @TimestampFCST, "
                               "@TimestampRcv, @Read)";
                auto stmtUpdateInfo = m_database->buildStatement(sqlInfo);

                auto sqlBBox = "REPLACE INTO EnvironmentFCSTBBox VALUES (@ID, @Left, @Right, @Bottom, @Top)";
                auto stmtUpdateBBox = m_database->buildStatement(sqlBBox); 

                for (auto &fcstInfo : container)
                {
                    auto longitude = fcstInfo.coordinate.GetLongitude();
                    auto latitude = fcstInfo.coordinate.GetLatitude();

                    int64_t dataID = 0;
                    auto sqlCheck = fmt::format("SELECT ID FROM MarineEnvironmentFCST WHERE ABS(Latitude - {}) < 0.00001 AND ABS(Longitude - {}) < 0.00001;", latitude, longitude);
                    SQLite::Statement queryCheck(*m_database.get(), sqlCheck);
                    if (queryCheck.executeStep())
                    {
                        dataID = queryCheck.getColumn(0).getInt64();
                    }

                    if (dataID != 0)
                    {
                        stmtUpdateInfo.bind("@ID", dataID);
                    }
                    else if (fcstInfo.dataID != 0)
                    {
                        stmtUpdateInfo.bind("@ID", fcstInfo.dataID);
                    }
                    stmtUpdateInfo.bind("@Latitude", latitude);
                    stmtUpdateInfo.bind("@Longitude", longitude);
                    stmtUpdateInfo.bind("@FlowVelocity", fcstInfo.flowVelocity);
                    stmtUpdateInfo.bind("@FlowDirection", fcstInfo.flowDirection);
                    stmtUpdateInfo.bind("@WaveHeight", fcstInfo.waveHeight);
                    stmtUpdateInfo.bind("@WaveDirection", fcstInfo.waveDirection);
                    stmtUpdateInfo.bind("@Temperature", fcstInfo.temperature);
                    stmtUpdateInfo.bind("@InfoSource", fcstInfo.infoSource);
                    stmtUpdateInfo.bind("@TimestampFCST", fcstInfo.timestampFCST);
                    stmtUpdateInfo.bind("@TimestampRcv", fcstInfo.timestamp);
                    stmtUpdateInfo.bind("@Read", fcstInfo.read ? 1 : 0);
                    stmtUpdateInfo.exec();
                    stmtUpdateInfo.reset(); 

                    // Update the bounding box
                    auto lastRowID = m_database->getLastInsertRowid();
                    stmtUpdateBBox.bind("@ID", lastRowID);
                    stmtUpdateBBox.bind("@Left", longitude);
                    stmtUpdateBBox.bind("@Right", longitude);
                    stmtUpdateBBox.bind("@Bottom", latitude);
                    stmtUpdateBBox.bind("@Top", latitude);
                    stmtUpdateBBox.exec();
                    stmtUpdateBBox.reset();
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMarineEnvironmentFCST");
            }
        }
    }

    void VDESManager::Impl::SaveMarineMeteorologyFCSTArea(const MarineMeteorologyFCSTAreas &container)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                // Delete data from three days ago.
                auto timeZone = UtilityInterface::GetTimeZone();
                auto timestampNow = UtilityInterface::GetCurrentTimeStamp();
                auto timestampDayBegin = timestampNow - (timestampNow % (24 * 3600)) - timeZone;
                auto timestampThreeDaysAgo = timestampDayBegin - 3 * 24 * 3600;
                auto sql = fmt::format("DELETE FROM MarineMeteorologyFCSTArea WHERE [Timestamp Forecast] <= {}",
                                       timestampThreeDaysAgo);
                m_database->exec(sql);

                auto sqlInfo = "REPLACE INTO MarineMeteorologyFCSTArea VALUES (@AreaCode, @WeatherCode, "
                    "@WindDirection, @WindScaleLow, @WindScaleHigh, @Visibility, @InfoSource, @TimestampFCST, "
                    "@TimestampRcv)";
                auto stmtUpdateInfo = m_database->buildStatement(sqlInfo);

                for (auto &area : container)
                {
                    stmtUpdateInfo.bind("@AreaCode", area.areaCode);
                    stmtUpdateInfo.bind("@WeatherCode", area.weatherCode);
                    stmtUpdateInfo.bind("@WindDirection", area.windDirection);
                    stmtUpdateInfo.bind("@WindScaleLow", area.windScaleLow);
                    stmtUpdateInfo.bind("@WindScaleHigh", area.windScaleHigh);
                    stmtUpdateInfo.bind("@Visibility", area.visibility);
                    stmtUpdateInfo.bind("@InfoSource", area.infoSource);
                    stmtUpdateInfo.bind("@TimestampFCST", area.timestampFCST);
                    stmtUpdateInfo.bind("@TimestampRcv", area.timestamp);
                    stmtUpdateInfo.exec();
                    stmtUpdateInfo.reset();
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMarineMeteorologyFCSTArea");
            }
        }
    }

    void VDESManager::Impl::LoadMarineMeteorologyFCSTAreaFromQueryResult(MarineMeteorologyFCSTArea &area, const SQLite::Statement &query)
    {
        area.areaCode = static_cast<uint8_t>(query.getColumn("Area Code").getInt());
        area.weatherCode = static_cast<uint8_t>(query.getColumn("Weather Code").getInt());
        area.windDirection = static_cast<uint16_t>(query.getColumn("Wind Direction").getInt());
        area.windScaleLow = static_cast<uint8_t>(query.getColumn("Wind Scale Low").getInt());
        area.windScaleHigh = static_cast<uint8_t>(query.getColumn("Wind Scale High").getInt());
        area.visibility = static_cast<uint8_t>(query.getColumn("Visibility").getInt());
        area.infoSource = static_cast<uint8_t>(query.getColumn("Info Source").getInt());
        area.timestampFCST = query.getColumn("Timestamp Forecast").getInt64();
        area.timestamp = query.getColumn("Timestamp Receive").getInt64();
    }

    void VDESManager::Impl::SaveMarineEnvironmentFCSTArea(const MarineEnvironmentFCSTAreas &container)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                // Delete data from three days ago.
                auto timeZone = UtilityInterface::GetTimeZone();
                auto timestampNow = UtilityInterface::GetCurrentTimeStamp();
                auto timestampDayBegin = timestampNow - (timestampNow % (24 * 3600)) - timeZone;
                auto timestampThreeDaysAgo = timestampDayBegin - 3 * 24 * 3600;
                auto sql = fmt::format("DELETE FROM MarineEnvironmentFCSTArea WHERE [Timestamp Forecast] <= {}",
                                       timestampThreeDaysAgo);
                m_database->exec(sql);

                auto sqlInfo = "REPLACE INTO MarineEnvironmentFCSTArea VALUES (@AreaCode, @TemperatureLow, "
                    "@TemperatureHigh, @FlowDirectionAvg, @FlowDirectionMax, @FlowVelocityAvg, @FlowVelocityMax, "
                    "@WaveHeight, @SwellHeight, @InfoSource, @TimestampFCST, @TimestampRcv)";
                auto stmtUpdateInfo = m_database->buildStatement(sqlInfo);

                for (auto &area : container)
                {
                    stmtUpdateInfo.bind("@AreaCode", area.areaCode);
                    stmtUpdateInfo.bind("@TemperatureLow", area.temperatureLow);
                    stmtUpdateInfo.bind("@TemperatureHigh", area.temperatureHigh);
                    stmtUpdateInfo.bind("@FlowDirectionAvg", area.flowDirectionAvg);
                    stmtUpdateInfo.bind("@FlowDirectionMax", area.flowDirctionMax);
                    stmtUpdateInfo.bind("@FlowVelocityAvg", area.flowVelocityAvg);
                    stmtUpdateInfo.bind("@FlowVelocityMax", area.flowVelocityMax);
                    stmtUpdateInfo.bind("@WaveHeight", area.waveHeight);
                    stmtUpdateInfo.bind("@SwellHeight", area.swellHeight);
                    stmtUpdateInfo.bind("@InfoSource", area.infoSource);
                    stmtUpdateInfo.bind("@TimestampFCST", area.timestampFCST);
                    stmtUpdateInfo.bind("@TimestampRcv", area.timestamp);
                    stmtUpdateInfo.exec();
                    stmtUpdateInfo.reset();
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMarineEnvironmentFCSTArea");
            }
        }
    }

    void VDESManager::Impl::LoadMarineEnvironmentFCSTAreaFromQueryResult(MarineEnvironmentFCSTArea &area, const SQLite::Statement &query)
    {
        area.areaCode = static_cast<uint8_t>(query.getColumn("Area Code").getInt());
        area.temperatureLow = static_cast<uint16_t>(query.getColumn("Temperature Low").getInt());
        area.temperatureHigh = static_cast<uint8_t>(query.getColumn("Temperature High").getInt());
        area.flowDirectionAvg = static_cast<uint32_t>(query.getColumn("Flow Direction Avg").getInt64());
        area.flowDirctionMax = static_cast<uint32_t>(query.getColumn("Flow Direction Max").getInt64());
        area.flowVelocityAvg = static_cast<uint8_t>(query.getColumn("Flow Velocity Avg").getInt());
        area.flowVelocityMax = static_cast<uint8_t>(query.getColumn("Flow Velocity Max").getInt());
        area.waveHeight = static_cast<uint8_t>(query.getColumn("Wave Height").getInt());
        area.swellHeight = static_cast<uint8_t>(query.getColumn("Swell Height").getInt());
        area.infoSource = static_cast<uint8_t>(query.getColumn("Info Source").getInt());
        area.timestampFCST = query.getColumn("Timestamp Forecast").getInt64();
        area.timestamp = query.getColumn("Timestamp Receive").getInt64();
    }

    void VDESManager::Impl::SaveMarineEnvironmentFCSTAlongshore(const MarineEnvironmentFCSTAlongshores &container)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                // Delete data from three days ago.
                auto timeZone = UtilityInterface::GetTimeZone();
                auto timestampNow = UtilityInterface::GetCurrentTimeStamp();
                auto timestampDayBegin = timestampNow - (timestampNow % (24 * 3600)) - timeZone;
                auto timestampThreeDaysAgo = timestampDayBegin - 3 * 24 * 3600;
                auto sql = fmt::format("DELETE FROM MarineEnvironmentFCSTAlongshore WHERE [Timestamp Forecast] <= {}",
                                       timestampThreeDaysAgo);
                m_database->exec(sql);

                auto sqlInfo = "REPLACE INTO MarineEnvironmentFCSTAlongshore VALUES (@AreaCode, @TemperatureLow, "
                    "@TemperatureHigh, @WaveHeightLow, @WaveHeightHigh, @TideHigh, @TimestampTideHigh, "
                    "@TideLow, @TimestampTideLow, @InfoSource, @TimestampFCST, @TimestampRcv)";
                auto stmtUpdateInfo = m_database->buildStatement(sqlInfo);

                for (auto &area : container)
                {
                    stmtUpdateInfo.bind("@AreaCode", area.areaCode);
                    stmtUpdateInfo.bind("@TemperatureLow", area.temperatureLow);
                    stmtUpdateInfo.bind("@TemperatureHigh", area.temperatureHigh);
                    stmtUpdateInfo.bind("@WaveHeightLow", area.waveHeightLow);
                    stmtUpdateInfo.bind("@WaveHeightHigh", area.waveHeightHigh);
                    stmtUpdateInfo.bind("@TideHigh", area.tideHigh);
                    stmtUpdateInfo.bind("@TimestampTideHigh", static_cast<int64_t>(area.timestampTideHigh));
                    stmtUpdateInfo.bind("@TideLow", area.tideLow);
                    stmtUpdateInfo.bind("@TimestampTideLow", static_cast<int64_t>(area.timestampTideLow));
                    stmtUpdateInfo.bind("@InfoSource", area.infoSource);
                    stmtUpdateInfo.bind("@TimestampFCST", area.timestampFCST);
                    stmtUpdateInfo.bind("@TimestampRcv", area.timestamp);
                    stmtUpdateInfo.exec();
                    stmtUpdateInfo.reset();
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMarineEnvironmentFCSTAlongshore");
            }
        }
    }

    void VDESManager::Impl::LoadMarineEnvironmentFCSTAlongshoreFromQueryResult(MarineEnvironmentFCSTAlongshore &area, const SQLite::Statement &query)
    {
        area.areaCode = static_cast<uint8_t>(query.getColumn("Area Code").getInt());
        area.temperatureLow = static_cast<uint16_t>(query.getColumn("Temperature Low").getInt());
        area.temperatureHigh = static_cast<uint8_t>(query.getColumn("Temperature High").getInt());
        area.waveHeightLow = static_cast<uint8_t>(query.getColumn("Wave Height Low").getInt());
        area.waveHeightHigh = static_cast<uint8_t>(query.getColumn("Wave Height High").getInt());
        area.tideHigh = static_cast<uint16_t>(query.getColumn("Tide High").getInt());
        area.timestampTideHigh = query.getColumn("Timestamp Tide High").getInt64();
        area.tideLow = static_cast<uint16_t>(query.getColumn("Tide Low").getInt());
        area.timestampTideLow = query.getColumn("Timestamp Tide Low").getInt64();
        area.infoSource = static_cast<uint8_t>(query.getColumn("Info Source").getInt());
        area.timestampFCST = query.getColumn("Timestamp Forecast").getInt64();
        area.timestamp = query.getColumn("Timestamp Receive").getInt64();
    }

    void VDESManager::Impl::SaveMarineMeteorologyEW(const MarineMeteorologyEW &ew)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                auto sql = fmt::format("INSERT INTO MarineMeteorologyEW VALUES (NULL, {MRN}, {Fragment}, {WarningType}, "
                    "{TimestampPublished}, {TimestampStart}, {TimestampEnd}, {WarningDuration}, {InfoSource}, {TimestampRcv}, {Read})",
                    fmt::arg("MRN", ew.MRN),
                    fmt::arg("Fragment", ew.fragment),
                    fmt::arg("WarningType", ew.warningType),
                    fmt::arg("TimestampPublished", ew.timestampPublished),
                    fmt::arg("TimestampStart", ew.timestampStart),
                    fmt::arg("TimestampEnd", ew.timestampEnd),
                    fmt::arg("WarningDuration", ew.warningDuration),
                    fmt::arg("InfoSource", ew.infoSource),
                    fmt::arg("TimestampRcv", ew.timestamp),
                    fmt::arg("Read", ew.read ? 1 : 0));
                m_database->exec(sql);
                auto rowID = m_database->getLastInsertRowid();

                if (ew.warningType == 1)
                {
                    for (const auto &pt : ew.cyclonePathPoints)
                    {
                        sql = fmt::format("INSERT INTO EW_TropicalCyclone VALUES (NULL, {WarningID}, {Timestamp}, {CenterLon}, {CenterLat}, {CycloneType}, "
                            "{Wind7Radius}, {Wind10Radius}, {Wind12Radius}, {MoveSpeed}, {MoveDirection}, {MaxWindScale}, {CenterPressure})",
                            fmt::arg("WarningID", rowID),
                            fmt::arg("Timestamp", pt.timestamp),
                            fmt::arg("CenterLon", pt.centerLongitude),
                            fmt::arg("CenterLat", pt.centerLatitude),
                            fmt::arg("CycloneType", pt.cycloneType),
                            fmt::arg("Wind7Radius", pt.radiusWindScale7),
                            fmt::arg("Wind10Radius", pt.radiusWindScale10),
                            fmt::arg("Wind12Radius", pt.radiusWindScale12),
                            fmt::arg("MoveSpeed", pt.moveSpeed),
                            fmt::arg("MoveDirection", pt.moveDirection),
                            fmt::arg("MaxWindScale", pt.maxWindScale),
                            fmt::arg("CenterPressure", pt.centerPressure));
                        m_database->exec(sql);
                    }
                }
                else
                {
                    for (const auto &elem : ew.elements)
                    {
                        sql = fmt::format("INSERT INTO EW_WarningElement VALUES ({WarningID}, {MRN}, {AreaCode}, {WarningLevel}, {SurgeHeight})",
                            fmt::arg("WarningID", rowID),
                            fmt::arg("MRN", elem.MRN),
                            fmt::arg("AreaCode", elem.areaCode),
                            fmt::arg("WarningLevel", elem.warningLevel),
                            fmt::arg("SurgeHeight", elem.surgeHeight));
                        m_database->exec(sql);
                    }
                }
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveMarineMeteorologyEW");
            }
        }
    }

    void VDESManager::Impl::LoadMarineMeteorologyEWFromQueryResult(MarineMeteorologyEW &ew,
        const SQLite::Statement &query)
    {
        auto ID = static_cast<uint32_t>(query.getColumn("ID").getInt());
        ew.dataID = ID;
        ew.MRN = static_cast<uint32_t>(query.getColumn("MRN").getInt());
        ew.fragment = static_cast<uint8_t>(query.getColumn("Fragment").getInt());
        ew.warningType = static_cast<uint8_t>(query.getColumn("Warning Type").getInt());
        ew.timestampPublished = query.getColumn("Timestamp Published").getInt64();
        ew.timestampStart = query.getColumn("Timestamp Start").getInt64();
        ew.timestampEnd = query.getColumn("Timestamp End").getInt64();
        ew.warningDuration = static_cast<uint8_t>(query.getColumn("Warning Duration").getInt());
        ew.infoSource = static_cast<uint8_t>(query.getColumn("Info Source").getInt());
        ew.timestamp = query.getColumn("Timestamp Receive").getInt64();
        ew.read = query.getColumn("Read").getInt() == 1;

        if (ew.warningType == 1)
        {
            auto sql = fmt::format("SELECT * FROM EW_TropicalCyclone WHERE Warning_ID = {};", ID);
            SQLite::Statement queryTC(*m_database.get(), sql);
            while (queryTC.executeStep())
            {
                MarineMeteorologyEW::TropicalCyclonePathPoint pt;
                pt.timestamp = queryTC.getColumn("Timestamp").getInt64();
                pt.centerLongitude = queryTC.getColumn("Center Lon").getDouble();
                pt.centerLatitude = queryTC.getColumn("Center Lat").getDouble();
                pt.cycloneType = static_cast<uint8_t>(queryTC.getColumn("Cyclone Type").getInt());
                pt.radiusWindScale7 = static_cast<uint16_t>(queryTC.getColumn("Wind 7 Radius").getInt());
                pt.radiusWindScale10 = static_cast<uint16_t>(queryTC.getColumn("Wind 10 Radius").getInt());
                pt.radiusWindScale12 = static_cast<uint16_t>(queryTC.getColumn("Wind 12 Radius").getInt());
                pt.moveSpeed = static_cast<uint8_t>(queryTC.getColumn("Move Speed").getInt());
                pt.moveDirection = static_cast<uint16_t>(queryTC.getColumn("Move Direction").getInt());
                pt.maxWindScale = static_cast<uint8_t>(queryTC.getColumn("Max Wind Scale").getInt());
                pt.centerPressure = static_cast<uint16_t>(queryTC.getColumn("Center Pressure").getInt());
                ew.cyclonePathPoints.push_back(pt);
            }
        }
        else
        {
            auto sql = fmt::format("SELECT * FROM EW_WarningElement WHERE Warning_ID = {};", ID);
            SQLite::Statement queryWE(*m_database.get(), sql);
            while (queryWE.executeStep())
            {
                MarineMeteorologyEW::WarningElement elem;
                elem.MRN = static_cast<uint32_t>(queryWE.getColumn("MRN").getInt());
                elem.areaCode = static_cast<uint8_t>(queryWE.getColumn("Area Code").getInt());
                elem.warningLevel = static_cast<uint8_t>(queryWE.getColumn("Warning Level").getInt());
                elem.surgeHeight = queryWE.getColumn("Surge Height").getDouble();
                ew.elements.emplace_back(elem);
            }
        }
    }

    void VDESManager::Impl::SaveTideForecast(const TideForecast &forecast)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                auto sql = fmt::format("INSERT INTO TideForecast VALUES (NULL, {HourPublish}, {InfoSource}, {TimestampRcv}, {Read})",
                                       fmt::arg("HourPublish", forecast.hourPublish),
                                       fmt::arg("InfoSource", forecast.infoSource),
                                       fmt::arg("TimestampRcv", forecast.timestamp),
                                       fmt::arg("Read", forecast.read ? 1 : 0));
                m_database->exec(sql);
                auto rowID = m_database->getLastInsertRowid();

                for (const auto &station : forecast.stations)
                {
                    sql = fmt::format("INSERT INTO TideStation VALUES ({ForecastID}, {Lon}, {Lat}, {TidalDatum}, {TideHigh}, {TimestampTideHigh}, {TideLow}, {TimestampTideLow})",
                                      fmt::arg("ForecastID", rowID),
                                      fmt::arg("Lon", station.coordinate.GetLongitude()),
                                      fmt::arg("Lat", station.coordinate.GetLatitude()),
                                      fmt::arg("TidalDatum", station.tidalDatum),
                                      fmt::arg("TideHigh", station.tideHigh),
                                      fmt::arg("TimestampTideHigh", station.timestampTideHigh),
                                      fmt::arg("TideLow", station.tideLow),
                                      fmt::arg("TimestampTideLow", station.timestampTideLow));
                    m_database->exec(sql);
                }

                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveTideForecast");
            }
        }
    }

    void VDESManager::Impl::LoadTideForecastFromQueryResult(TideForecast &forecast, const SQLite::Statement &query)
    {
        auto ID = static_cast<uint32_t>(query.getColumn("ID").getInt());
        forecast.dataID = ID;
        forecast.hourPublish = static_cast<uint8_t>(query.getColumn("Hour Publish").getInt());
        forecast.infoSource = static_cast<uint8_t>(query.getColumn("Info Source").getInt());
        forecast.timestamp = query.getColumn("Timestamp Receive").getInt64();
        forecast.read = query.getColumn("Read").getInt() == 1;

        auto sql = fmt::format("SELECT * FROM TideStation WHERE Forecast_ID = {};", ID);
        SQLite::Statement queryStation(*m_database.get(), sql);
        while (queryStation.executeStep())
        {
            TideForecast::TideStation station;
            station.coordinate.SetLongitude(queryStation.getColumn("Longitude").getDouble());
            station.coordinate.SetLatitude(queryStation.getColumn("Latitude").getDouble());
            station.tidalDatum = queryStation.getColumn("Tidal Datum").getDouble();
            station.tideHigh = queryStation.getColumn("Tide High").getDouble();
            station.timestampTideHigh = queryStation.getColumn("Timestamp Tide High").getInt64();
            station.tideLow = queryStation.getColumn("Tide Low").getDouble();
            station.timestampTideLow = queryStation.getColumn("Timestamp Tide Low").getInt64();
            forecast.stations.emplace_back(station);
        }
    }

    void VDESManager::Impl::SaveAtoNDynamics(const AtoNDynamics &dynamics)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                auto sql = fmt::format("INSERT INTO AtoNDynamics VALUES (NULL, {MRN}, {Fragment}, {Status}, {Precaution}, {TimestampRcv}, {Read})",
                                       fmt::arg("MRN", dynamics.MRN),
                                       fmt::arg("Fragment", dynamics.fragment),
                                       fmt::arg("Status", dynamics.status),
                                       fmt::arg("Precaution", dynamics.precaution),
                                       fmt::arg("TimestampRcv", dynamics.timestamp),
                                       fmt::arg("Read", dynamics.read ? 1 : 0));
                m_database->exec(sql);
                auto rowID = m_database->getLastInsertRowid();

                for (const auto &elem : dynamics.elements)
                {
                    sql = fmt::format("INSERT INTO AtoNDynamicElement VALUES ({DynamicsID}, {MRN}, {Type}, {Lon}, {Lat}, "
                                      "{RhythmNameCode}, {RhythmParamCode}, {BodyColor}, {LightColor}, {LightPeriod}, "
                                      "{Range}, {MorseCode}, {PrevLon}, {PrevLat}, {IsRoughPosition}, {FailureType}, "
                                      "{TimestampStart}, {TimestampEnd})",
                                      fmt::arg("DynamicsID", rowID),
                                      fmt::arg("MRN", elem.MRN),
                                      fmt::arg("Type", elem.type),
                                      fmt::arg("Lon", elem.coordinate.GetLongitude()),
                                      fmt::arg("Lat", elem.coordinate.GetLatitude()),
                                      fmt::arg("RhythmNameCode", elem.rhythmNameCode),
                                      fmt::arg("RhythmParamCode", elem.rhythmParamCode),
                                      fmt::arg("BodyColor", elem.bodyColor),
                                      fmt::arg("LightColor", elem.lightColor),
                                      fmt::arg("LightPeriod", elem.lightPeriod),
                                      fmt::arg("Range", elem.range),
                                      fmt::arg("MorseCode", elem.morseCode),
                                      fmt::arg("PrevLon", elem.prevCoordinate.GetLongitude()),
                                      fmt::arg("PrevLat", elem.prevCoordinate.GetLatitude()),
                                      fmt::arg("IsRoughPosition", elem.isRoughPosition ? 1 : 0),
                                      fmt::arg("FailureType", elem.failureType),
                                      fmt::arg("TimestampStart", elem.timestampStart),
                                      fmt::arg("TimestampEnd", elem.timestampEnd));
                    m_database->exec(sql);
                }

                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveAtoNDynamics");
            }
        }
    }

    void VDESManager::Impl::LoadAtoNDynamicsFromQueryResult(AtoNDynamics &dynamics, const SQLite::Statement &query)
    {
        auto ID = static_cast<uint32_t>(query.getColumn("ID").getInt());
        dynamics.dataID = ID;
        dynamics.MRN = static_cast<uint32_t>(query.getColumn("MRN").getInt());
        dynamics.fragment = static_cast<uint8_t>(query.getColumn("Fragment").getInt());
        dynamics.status = static_cast<uint8_t>(query.getColumn("Status").getInt());
        dynamics.precaution = static_cast<uint8_t>(query.getColumn("Precaution").getInt());
        dynamics.timestamp = query.getColumn("Timestamp Receive").getInt64();
        dynamics.read = query.getColumn("Read").getInt() == 1;

        auto sql = fmt::format("SELECT * FROM AtoNDynamicElement WHERE Dynamics_ID = {};", ID);
        SQLite::Statement queryElement(*m_database.get(), sql);
        while (queryElement.executeStep())
        {
            AtoNDynamics::Element elem;
            elem.MRN = static_cast<uint32_t>(queryElement.getColumn("MRN").getInt());
            elem.type = static_cast<uint8_t>(queryElement.getColumn("Type").getInt());
            elem.coordinate.SetLongitude(queryElement.getColumn("Longitude").getDouble());
            elem.coordinate.SetLatitude(queryElement.getColumn("Latitude").getDouble());
            elem.rhythmNameCode = static_cast<uint8_t>(queryElement.getColumn("Rhythm Name Code").getInt());
            elem.rhythmParamCode = static_cast<uint8_t>(queryElement.getColumn("Rhythm Param Code").getInt());
            elem.bodyColor = static_cast<uint8_t>(queryElement.getColumn("Body Color").getInt());
            elem.lightColor = static_cast<uint8_t>(queryElement.getColumn("Light Color").getInt());
            elem.lightPeriod = static_cast<uint8_t>(queryElement.getColumn("Light Period").getInt());
            elem.range = static_cast<uint8_t>(queryElement.getColumn("Range").getInt());
            elem.morseCode = static_cast<uint8_t>(queryElement.getColumn("Morse Code").getInt());
            elem.prevCoordinate.SetLongitude(queryElement.getColumn("Prev Longitude").getDouble());
            elem.prevCoordinate.SetLatitude(queryElement.getColumn("Prev Latitude").getDouble());
            elem.isRoughPosition = queryElement.getColumn("Is Rough Position").getInt() == 1;
            elem.failureType = static_cast<uint8_t>(queryElement.getColumn("Failure Type").getInt());
            elem.timestampStart = queryElement.getColumn("Timestamp Start").getInt64();
            elem.timestampEnd = queryElement.getColumn("Timestamp End").getInt64();
            dynamics.elements.emplace_back(elem);
        }
    }

    void VDESManager::Impl::SaveAISAtoNDynamics(const AISAtoNDynamics &dynamics)
    {
        if (m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_database.get());

                auto sql = fmt::format("INSERT INTO AISAtoNDynamics VALUES (NULL, {MRN}, {Fragment}, {AtoNAttribute}, {Status}, {Precaution}, {TimestampRcv}, {Read})",
                                       fmt::arg("MRN", dynamics.MRN),
                                       fmt::arg("Fragment", dynamics.fragment),
                                       fmt::arg("AtoNAttribute", dynamics.atonAttribute),
                                       fmt::arg("Status", dynamics.status),
                                       fmt::arg("Precaution", dynamics.precaution),
                                       fmt::arg("TimestampRcv", dynamics.timestamp),
                                       fmt::arg("Read", dynamics.read ? 1 : 0));
                m_database->exec(sql);
                auto rowID = m_database->getLastInsertRowid();

                for (const auto &elem : dynamics.elements)
                {
                    sql = fmt::format("INSERT INTO AISAtoNDynamicElement VALUES ({DynamicsID}, {MMSI}, {MRN}, {Type}, {Lon}, {Lat}, "
                                      "{Range}, {PrevLon}, {PrevLat}, {IsRoughPosition})",
                                      fmt::arg("DynamicsID", rowID),
                                      fmt::arg("MMSI", elem.mmsi),
                                      fmt::arg("MRN", elem.MRN),
                                      fmt::arg("Type", elem.type),
                                      fmt::arg("Lon", elem.coordinate.GetLongitude()),
                                      fmt::arg("Lat", elem.coordinate.GetLatitude()),
                                      fmt::arg("Range", elem.range),
                                      fmt::arg("PrevLon", elem.prevCoordinate.GetLongitude()),
                                      fmt::arg("PrevLat", elem.prevCoordinate.GetLatitude()),
                                      fmt::arg("IsRoughPosition", elem.isRoughPosition ? 1 : 0));
                    m_database->exec(sql);
                }

                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveAISAtoNDynamics");
            }
        }
    }

    void VDESManager::Impl::LoadAISAtoNDynamicsFromQueryResult(AISAtoNDynamics &dynamics, const SQLite::Statement &query)
    {
        auto ID = static_cast<uint32_t>(query.getColumn("ID").getInt());
        dynamics.dataID = ID;
        dynamics.MRN = static_cast<uint32_t>(query.getColumn("MRN").getInt());
        dynamics.fragment = static_cast<uint8_t>(query.getColumn("Fragment").getInt());
        dynamics.atonAttribute = static_cast<uint8_t>(query.getColumn("AtoNAttribute").getInt());
        dynamics.status = static_cast<uint8_t>(query.getColumn("Status").getInt());
        dynamics.precaution = static_cast<uint8_t>(query.getColumn("Precaution").getInt());
        dynamics.timestamp = query.getColumn("Timestamp Receive").getInt64();
        dynamics.read = query.getColumn("Read").getInt() == 1;

        auto sql = fmt::format("SELECT * FROM AISAtoNDynamicElement WHERE Dynamics_ID = {};", ID);
        SQLite::Statement queryElement(*m_database.get(), sql);
        while (queryElement.executeStep())
        {
            AISAtoNDynamics::Element elem;
            elem.mmsi = static_cast<uint32_t>(queryElement.getColumn("MMSI").getInt());
            elem.MRN = static_cast<uint32_t>(queryElement.getColumn("MRN").getInt());
            elem.type = static_cast<uint8_t>(queryElement.getColumn("Type").getInt());
            elem.coordinate.SetLongitude(queryElement.getColumn("Longitude").getDouble());
            elem.coordinate.SetLatitude(queryElement.getColumn("Latitude").getDouble());
            elem.range = static_cast<uint8_t>(queryElement.getColumn("Range").getInt());
            elem.prevCoordinate.SetLongitude(queryElement.getColumn("Prev Longitude").getDouble());
            elem.prevCoordinate.SetLatitude(queryElement.getColumn("Prev Latitude").getDouble());
            elem.isRoughPosition = queryElement.getColumn("Is Rough Position").getInt() == 1;
            dynamics.elements.emplace_back(elem);
        }
    }

    void VDESManager::Impl::SaveVTSRequest(const VTSRequest &request)
    {
        if (m_database)
        {
            try
            {
                auto sql = fmt::format("REPLACE INTO VTSRequest VALUES ({ID}, {SequenceNum}, {RequestType}, "
                                       "{TimestampRequest}, {VTSReply}, {TimestampReply}, {ExecutingStatus})",
                                       fmt::arg("ID", (request.dataID == 0) ? "NULL" : fmt::format("{}", request.dataID)),
                                       fmt::arg("SequenceNum", m_textSequenceNoVTS),
                                       fmt::arg("RequestType", request.requestType),
                                       fmt::arg("TimestampRequest", request.timestamp),
                                       fmt::arg("VTSReply", request.replyResult),
                                       fmt::arg("TimestampReply", request.timestampReply),
                                       fmt::arg("ExecutingStatus", request.executingStatus));
                m_database->exec(sql);
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveVTSRequest");
            }
        }
    }

    void VDESManager::Impl::SaveVTSReply(const VTSReply &reply, const uint16_t sequenceNo)
    {
        if (m_database)
        {
            try
            {
                std::vector<WaypointStruct> waypoints;
                for (const auto &point : reply.waypoints)
                {
                    WaypointStruct wp;
                    wp.coordinate.latitude = point.coordinate.GetLatitude();
                    wp.coordinate.longitude = point.coordinate.GetLongitude();
                    wp.course = point.course;
                    wp.timestamp = point.timestamp;
                    waypoints.emplace_back(wp);
                }

                std::lock_guard<std::mutex> lock(m_mutexVTSReply);

                auto sql = "REPLACE INTO VTSReply VALUES (@ID, @VTSRequestID, @SequenceNum, @Result, "
                           "@MMSI, @ChineseRemark, @EnglishRemark, @QueueNum, @Waypoints, @Timestamp)";
                auto stmt = m_database->buildStatement(sql);

                if (reply.dataID != 0)
                {
                    stmt.bind("@ID", reply.dataID);
                }
                stmt.bind("@VTSRequestID", reply.requestDataID);
                stmt.bind("@SequenceNum", sequenceNo);
                stmt.bind("@Result", reply.isAgreed ? 0 : 1);
                stmt.bind("@MMSi", reply.mmsiRequestor);
                stmt.bind("@ChineseRemark", reply.remarkChn);
                stmt.bind("@EnglishRemark", reply.remarkEng);
                stmt.bind("@QueueNum", reply.queueNo);
                auto size = waypoints.size() * sizeof(WaypointStruct);
                stmt.bind("@waypoints", waypoints.empty() ? nullptr : waypoints.data(), size);
                stmt.bind("@Timestamp", UtilityInterface::GetCurrentTimeStamp());
                stmt.exec();
            }
            catch (const SQLite::Exception &exception)
            {
                DatabaseErrorProcess(exception, "SaveVTSReply");
            }
        }
    }

    void VDESManager::Impl::ParseBusinessSixBitASCIIMessage(const uint32_t mmsiSource,
        const uint32_t mmsiDest, const AISBitsManager &manager)
    {
        auto VPFI = manager.DecodeToNumerical(0, 16);
        auto responserIdentifier = manager.DecodeToNumerical(32, 1);
        auto spare1 = manager.DecodeToNumerical(33, 2);
        auto sequenceNum = static_cast<uint16_t>(manager.DecodeToNumerical(35, 11));
        auto totalBits = manager.GetBitsNumberToDecode();
        auto sixBitCodeNum = (totalBits - 590) / 6;
        auto text = manager.DecodeToString(46, sixBitCodeNum * 6);
        UtilityInterface::RemoveTailCharacter(text, '@');
        auto key = fmt::format("{0:09d}{1:09d}", mmsiSource, mmsiDest);
        m_messageMap[key][sequenceNum] = std::string(text.begin(), text.end());
        auto deadline = manager.DecodeToNumerical(totalBits - 512 - 32, 32);
        BuildMessage(mmsiSource, mmsiDest, MessageBusinessType::SIX_BIT_ASCII, responserIdentifier,
                     sequenceNum, deadline);
    }

    void VDESManager::Impl::ParseBusinessUTF8Message(const uint32_t mmsiSource,
        const uint32_t mmsiDest, const AISBitsManager &manager)
    {
        auto VPFI = manager.DecodeToNumerical(0, 16);
        auto responserIdentifier = manager.DecodeToNumerical(32, 1);
        auto spare1 = manager.DecodeToNumerical(33, 2);
        auto sequenceNum = static_cast<uint16_t>(manager.DecodeToNumerical(35, 11));
        auto spare2 = manager.DecodeToNumerical(46, 2);

        auto totalBits = manager.GetBitsNumberToDecode();
        auto textBits = totalBits - 48 - 32 - 512;
        auto text = manager.GetBytesEncoded(48, 48 + textBits - 1);
        auto deadline = manager.DecodeToNumerical(48 + textBits, 32);
        auto key = fmt::format("{0:09d}{1:09d}", mmsiSource, mmsiDest);
        m_messageMap[key][sequenceNum] = std::string(text.begin(), text.end());
        BuildMessage(mmsiSource, mmsiDest, MessageBusinessType::UTF8, responserIdentifier, sequenceNum, deadline);
    }

    void VDESManager::Impl::ParseBusiness14BitMessage(const uint32_t mmsiSource, const uint32_t mmsiDest, 
                                                      const AISBitsManager &manager)
    {
        auto VPFI = manager.DecodeToNumerical(0, 16);
        auto responserIdentifier = manager.DecodeToNumerical(32, 1);
        auto spare1 = manager.DecodeToNumerical(33, 2);
        auto sequenceNum = static_cast<uint16_t>(manager.DecodeToNumerical(35, 11));
        auto spare2 = manager.DecodeToNumerical(46, 2);
        
        auto totalBits = manager.GetBitsNumberToDecode();
        auto suplusBits = totalBits - 48 - 32 - 512;
        auto characterNum = suplusBits / 7;
        auto remainder = suplusBits % 7;
        auto spare3 = (remainder == 0) ? 0 : (8 - remainder);
        auto text = manager.DecodeToString(48, characterNum * 7, 413, 1);
        UtilityInterface::RemoveTailCharacter(text, '@');
        auto textUTF8 = UtilityInterface::GBKToUTF8(text);
        SPDLOG_DEBUG(textUTF8);
        auto key = fmt::format("{0:09d}{1:09d}", mmsiSource, mmsiDest);
        m_messageMap[key][sequenceNum] = std::string(textUTF8.begin(), textUTF8.end());
        auto deadline = manager.DecodeToNumerical(totalBits - 512 - 32, 32);
        BuildMessage(mmsiSource, mmsiDest, MessageBusinessType::FOURTEEN_BIT, responserIdentifier,
                     sequenceNum, deadline);
    }

    void VDESManager::Impl::ParseBusinessMessageConfirmation(const uint32_t mmsiSource, 
        const uint32_t mmsiDest, const AISBitsManager &manager)
    {
        auto VPFI = manager.DecodeToNumerical(0, 16);
        auto messageID = manager.DecodeToNumerical(32, 16);
        auto spare1 = manager.DecodeToNumerical(48, 2);
        auto sequenceNum = manager.DecodeToNumerical(50, 11);
        auto messageValid = manager.DecodeToNumerical(61, 1);
        auto spare2 = manager.DecodeToNumerical(62, 2);

        MessageBusinessType messageType = MessageBusinessType::NONE;

        switch (messageID)
        {
        case 0:
            messageType = MessageBusinessType::SIX_BIT_ASCII;
            break;
        case 1:
            messageType = MessageBusinessType::UTF8;
            break;
        case 11:
            messageType = MessageBusinessType::FOURTEEN_BIT;
            break;
        default:
            return;
        }

        try
        {
            std::lock_guard<std::mutex> lockGuard(m_mutexMessageOutbox);
            auto sql = fmt::format("UPDATE MessageOutbox SET Valid = {} WHERE Type = {} AND "
                                   "[Text Sequence Number] = {}",
                                   (messageValid == 0) ? 2 : 1,
                                   static_cast<int>(messageType),
                                   sequenceNum);
            m_database->exec(sql);
            m_parent->notifyEvent(EventType::MESSAGE_RECEIVE, 1);
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "ParseBusinessMessageConfirmation");
        }   
    }

    void VDESManager::Impl::ParseBusinessHydrometeorology(const uint32_t mmsiSource, const uint32_t mmsiDest, 
                                                          const AISBitsManager &manager)
    {
        std::vector<Hydrometeorology> hydroContainer;
        Hydrometeorology              hydro;

        auto year = manager.DecodeToNumerical(32, 11);
        auto month = manager.DecodeToNumerical(43, 4);
        auto day = manager.DecodeToNumerical(47, 5);
        auto hour = manager.DecodeToNumerical(52, 9);
        
        auto timestamp = UtilityInterface::GetCurrentTimeStamp();
        tm   timeUTC = { 0 };

        timeUTC = *gmtime((time_t *)&timestamp);
        
        if (year >= 1900)
        {
            timeUTC.tm_year = static_cast<int>(year - 1900);
        }
        if (month >= 1 && month <= 12)
        {
            timeUTC.tm_mon = static_cast<int>(month - 1);
        }
        if (day >= 1 && day <= 31)
        {
            timeUTC.tm_mday = static_cast<int>(day);
        }
        timeUTC.tm_hour = 0;
        timeUTC.tm_min = 0;
        timeUTC.tm_sec = 0;
 
        timestamp = UtilityInterface::MakeGmtime(&timeUTC);
        timestamp -= 8 * 3600;

        hydro.timestampPublished = timestamp;
        hydro.timestampForecast = timestamp + hour * 3600;

        auto pointNum = static_cast<uint8_t>(manager.DecodeToNumerical(61, 4));

        hydro.source = static_cast<uint8_t>(manager.DecodeToNumerical(65 + pointNum * 67, 3));
        if (pointNum >= 1 && pointNum <= 13)
        {
            for (auto i = 0U; i < pointNum; i++)
            {
                auto value1 = manager.DecodeToNumerical(65 + i * 67, 13);
                auto longitude = value1 / 60.0 + 60.0;
               
                auto value2 = manager.DecodeToNumerical(78 + i * 67, 13);
                auto latitude = value2 / 60.0 - 50.0;
                SPDLOG_DEBUG("Longitude raw value = {}, converted value = {:.6f}", value1, longitude);
                SPDLOG_DEBUG("Latitude raw value = {}, converted value = {:.6f}", value2, latitude);
                hydro.coordinate.SetLongitude(longitude);
                hydro.coordinate.SetLatitude(latitude);
                hydro.timestampHT = DecodeTime(manager, 91 + i * 67, 16);
                hydro.timestampLT = DecodeTime(manager, 107 + i * 67, 16);
                auto value = manager.DecodeToNumerical(123 + i * 67, 9) / 10.0;
                hydro.waterLevel = -25.0 + value;
                hydroContainer.emplace_back(hydro);
            } 
        }
        
        std::unique_lock<std::mutex> lockGuard(m_mutexHydrometeorology);
        SaveHydrometeoroloies(hydroContainer);
        lockGuard.unlock();
        m_parent->notifyEvent(EventType::HYDROLOGICAL_METEOROLOGICAL, 0);
    }

    void VDESManager::Impl::ParseBusinessAtoN(const uint32_t mmsiSource, const uint32_t mmsiDest,
        const AISBitsManager &manager)
    {
        AtoN aton;

        aton.mmsi = static_cast<uint32_t>(manager.DecodeToNumerical(32, 30));
        aton.type = static_cast<uint8_t>(manager.DecodeToNumerical(62, 5));
        aton.coordinate.SetLongitude(DecodeCoordinate(manager, 67, 28));
        aton.coordinate.SetLatitude(DecodeCoordinate(manager, 95, 27));
        aton.status = static_cast<uint8_t>(manager.DecodeToNumerical(122, 8));
        aton.isVirtual = manager.DecodeToNumerical(130, 1) == 1;
        auto isCancelled = manager.DecodeToNumerical(131, 1) == 1;

        auto totalBits = manager.GetBitsNumberToDecode();

        auto surplusBits = totalBits - 132 - 32 - 512;
        auto sixBitCodeNum = surplusBits / 6;
        aton.name = manager.DecodeToString(132, sixBitCodeNum * 6);
        auto spareBitsNum = surplusBits % 6;
        aton.deadline = manager.DecodeToNumerical(132 + sixBitCodeNum * 6 + spareBitsNum, 32);
        aton.timestamp = UtilityInterface::GetCurrentTimeStamp();

        std::unique_lock<std::mutex> lock(m_mutexAtoN);
        if (isCancelled)
        {
            DeleteAtoN(aton.mmsi);
        }
        else
        {
            SaveAtoN(aton);
        }
        lock.unlock();

        SPDLOG_DEBUG("Aton mmsi = {}, name = {}, deadline = {}", aton.mmsi, aton.name, aton.deadline);
        m_parent->notifyEvent(EventType::ATON, 0);
    }

    void VDESManager::Impl::ParseBusinessMSI(const uint32_t mmsiSource, const uint32_t mmsiDest, 
        const AISBitsManager &manager)
    {
        auto msiType = manager.DecodeToNumerical(32, 3);
        switch (msiType)
        {
        case 0: // Obstacle information
            ParseMSIObstacle(manager);
            break;
        case 1: // Maritime Operations
            ParseMSIMaritimeOperations(manager);
            break;
        case 2: // Naval Operations
            ParseMSIMilitaryActivity(manager);
            break;
        case 3: // Maritime Distress
            ParseMSIMaritimeDistress(manager);
            break;
        }
    }

    void VDESManager::Impl::ParseMSIObstacle(const AISBitsManager &manager)
    {
        MSIObstacle obstacle;

        obstacle.type = static_cast<uint8_t>(manager.DecodeToNumerical(35, 4));
        obstacle.coordinate.SetLongitude(DecodeCoordinate(manager, 39, 28));
        obstacle.coordinate.SetLatitude(DecodeCoordinate(manager, 67, 27));
        obstacle.range = manager.DecodeToNumerical(94, 7) / 10.0;
        obstacle.timestampStart = DecodeTime(manager, 101, 20);
        obstacle.timestampEnd = DecodeTime(manager, 121, 20);
        obstacle.comment = static_cast<uint8_t>(manager.DecodeToNumerical(141, 3));
        obstacle.isCertified = true;
        obstacle.timestamp = UtilityInterface::GetCurrentTimeStamp();

        std::unique_lock<std::mutex> lock(m_mutexMSIObstacle);
        SaveMSIObstacle(obstacle); 
        lock.unlock();

        m_parent->notifyEvent(EventType::MSI_OBSTACLE, 0);
    }

    void VDESManager::Impl::ParseMSIMaritimeOperations(const AISBitsManager &manager)
    {
        MSIMaritimeOperation operation;
        Coordinate           coordiante;

        operation.type = static_cast<uint8_t>(manager.DecodeToNumerical(35, 4));
        operation.status = static_cast<uint8_t>(manager.DecodeToNumerical(39, 3));
        operation.mmsi = static_cast<uint32_t>(manager.DecodeToNumerical(42, 30));
        
        operation.coordinateNE.SetLongitude(DecodeCoordinate(manager, 72, 28));
        operation.coordinateNE.SetLatitude(DecodeCoordinate(manager, 100, 27));
      
        operation.coordinateSW.SetLongitude(DecodeCoordinate(manager, 127, 28));
        operation.coordinateSW.SetLatitude(DecodeCoordinate(manager, 155, 27));
        
        auto value = manager.DecodeToNumerical(182, 7);
        operation.range = value / 10.0;
        operation.timestampStart = DecodeTime(manager, 189, 20);
        operation.timestampEnd = DecodeTime(manager, 209, 20);
        operation.isCertified = true;
        operation.timestamp = UtilityInterface::GetCurrentTimeStamp();

        std::unique_lock<std::mutex> lock(m_mutexMSIMaritimeOperation);
        SaveMSIMaritineOperation(operation);
        lock.unlock();

        m_parent->notifyEvent(EventType::MSI_MARITIME_OPERATION, 0);
    }

    void VDESManager::Impl::ParseMSIMilitaryActivity(const AISBitsManager &manager)
    {
        MSIMilitaryActivity activity;
        Coordinate          coordiante;

        activity.theme = static_cast<uint8_t>(manager.DecodeToNumerical(35, 4));
        activity.isContinuing = (manager.DecodeToNumerical(39, 1)) == 0;
        auto pointNum = static_cast<uint8_t>(manager.DecodeToNumerical(40, 5));
        if (pointNum >= 3 && pointNum <= 16)
        {
            for (auto i = 0U; i < pointNum; i++)
            {
                coordiante.SetLongitude(DecodeCoordinate(manager, 45 + i * 55, 28));
                coordiante.SetLatitude(DecodeCoordinate(manager, 73 + i * 55, 27));
                activity.polygon.push_back(coordiante);
            }
            activity.timestampStart = DecodeTime(manager, 45 + pointNum * 55, 20);
            activity.timestampEnd = DecodeTime(manager, 65 + pointNum * 55, 20);
            activity.cautionCode = static_cast<uint8_t>(manager.DecodeToNumerical(85 + pointNum * 55, 4));
            activity.isCertified = true;
            activity.timestamp = UtilityInterface::GetCurrentTimeStamp();

            std::unique_lock<std::mutex> lock(m_mutexMSIMilitaryActivity);
            SaveMSIMilitaryActivity(activity);
            lock.unlock();

            m_parent->notifyEvent(EventType::MSI_MILITARY_ACTIVITY, 0);
        }
    }
   
    void VDESManager::Impl::ParseMSIMaritimeDistress(const AISBitsManager &manager)
    {
        MSIMaritimeDistress distress;

        distress.type = static_cast<uint8_t>(manager.DecodeToNumerical(35, 4));
        distress.situation = static_cast<uint8_t>(manager.DecodeToNumerical(39, 4));
        distress.status = static_cast<uint8_t>(manager.DecodeToNumerical(43, 3));
        distress.additional = static_cast<uint8_t>(manager.DecodeToNumerical(46, 4));
        distress.coordinate.SetLongitude(DecodeCoordinate(manager, 50, 28));
        distress.coordinate.SetLatitude(DecodeCoordinate(manager, 78, 27));
        distress.timestampValid = DecodeTime(manager, 105, 20);
        distress.isCertified = true;
        distress.timestamp = UtilityInterface::GetCurrentTimeStamp();

        std::unique_lock<std::mutex> lock(m_mutexMSIMaritimeDistress);
        SaveMSIMaritimeDistress(distress);
        lock.unlock();

        m_parent->notifyEvent(EventType::MSI_MARITIME_DISTRESS, 0);
    }

    void VDESManager::Impl::ParseBusinessAbnormalShip(const uint32_t mmsiSource, const uint32_t mmsiDest, 
        const AISBitsManager &manager)
    {
        AbnormalShip ship;

        ship.alertType = static_cast<uint8_t>(manager.DecodeToNumerical(32, 5));
        if (ship.alertType == 5) // one ship, multiple mmsi
        {
            auto count = static_cast<uint8_t>(manager.DecodeToNumerical(37, 3));
            if (count >= 2 && count <= 7)
            {
                auto textBitsNum = manager.GetBitsNumberToDecode() - 40 - 120 * count - 512;
                auto startBitPos = 40 + 120 * count;
                auto text = manager.GetBytesEncoded(startBitPos, startBitPos + textBitsNum - 1);
                auto timestampRcv = UtilityInterface::GetCurrentTimeStamp();

                for (auto i = 0; i < count; i++)
                {
                    auto mmsi = static_cast<uint32_t>(manager.DecodeToNumerical(40 + i * 120, 30));
                    Coordinate coordinate;
                    coordinate.SetLongitude(DecodeCoordinate(manager, 70 + i * 120, 28));
                    coordinate.SetLatitude(DecodeCoordinate(manager, 98 + i * 120, 27));
                    auto timestamp = manager.DecodeToNumerical(125 + i * 120, 32);

                    ship.mmsi = mmsi;
                    AbnormalShip::CoordinateInfo coordinateInfo;
                    coordinateInfo.coordinate = coordinate;
                    coordinateInfo.timestampAlert = timestamp;
                    ship.coordinateInfos.emplace_back(coordinateInfo);
                    ship.contentAlert = std::string(text.begin(), text.end());
                    ship.timestamp = timestampRcv;
                    ship.isCertified = true;
                    
                    std::unique_lock<std::mutex> lock(m_mutexAbnormalShip);  
                    SaveAbnormalShip(ship);
                    ship.coordinateInfos.clear();
                    lock.unlock();
                    m_parent->notifyEvent(EventType::ABNORMAL_SHIP, 0);
                } 
            }
        }
        else if (ship.alertType == 4) // one mmsi, multiple coordinates
        {
            ship.mmsi = static_cast<uint32_t>(manager.DecodeToNumerical(37, 30));
            auto count = static_cast<uint8_t>(manager.DecodeToNumerical(67, 5));
            if (count >= 2 && count <= 7)
            {
                for (auto i = 0U; i < count; i++)
                {
                    Coordinate coordinate;
                    coordinate.SetLongitude(DecodeCoordinate(manager, 72 + i * 88, 28));
                    coordinate.SetLatitude(DecodeCoordinate(manager, 100 + i * 88, 27));
                    auto timestamp = manager.DecodeToNumerical(127 + i * 88, 32);
                    AbnormalShip::CoordinateInfo coordinateInfo;
                    coordinateInfo.coordinate = coordinate;
                    coordinateInfo.timestampAlert = timestamp;
                    ship.coordinateInfos.emplace_back(coordinateInfo);
                }
            }
            auto textBitsNum = manager.GetBitsNumberToDecode() - 72 - 88 * count - 512;
            auto statBitPos = 72 + 88 * count;
            auto text = manager.GetBytesEncoded(statBitPos, statBitPos + textBitsNum - 1);
            ship.contentAlert = std::string(text.begin(), text.end());
            ship.isCertified = true;
            ship.timestamp = UtilityInterface::GetCurrentTimeStamp();
            std::unique_lock<std::mutex> lock(m_mutexAbnormalShip);
            SaveAbnormalShip(ship);
            lock.unlock();
            m_parent->notifyEvent(EventType::ABNORMAL_SHIP, 0);
        }
        else if ((ship.alertType >= 0 && ship.alertType <= 3) ||
                 (ship.alertType >= 6 && ship.alertType <= 8))
        {
            ship.mmsi = static_cast<uint32_t>(manager.DecodeToNumerical(37, 30));
            Coordinate coordinate;
            coordinate.SetLongitude(DecodeCoordinate(manager, 67, 28));
            coordinate.SetLatitude(DecodeCoordinate(manager, 95, 27));

            auto timestamp = manager.DecodeToNumerical(122, 32);
         
            AbnormalShip::CoordinateInfo coordinateInfo;
            coordinateInfo.coordinate = coordinate;
            coordinateInfo.timestampAlert = timestamp;
            ship.coordinateInfos.emplace_back(coordinateInfo);
            auto textBitsNum = manager.GetBitsNumberToDecode() - 160 - 512;
            auto text = manager.GetBytesEncoded(154, 154 + textBitsNum - 1);
            ship.contentAlert = std::string(text.begin(), text.end());
            ship.isCertified = true;
            ship.timestamp = UtilityInterface::GetCurrentTimeStamp();
            std::unique_lock<std::mutex> lock(m_mutexAbnormalShip);
            SaveAbnormalShip(ship);  
            lock.unlock();
            m_parent->notifyEvent(EventType::ABNORMAL_SHIP, 0);
        }
    }

    void VDESManager::Impl::ParseBusinessVTSReply(const uint32_t mmsiSource, const uint32_t mmsiDest,
        const AISBitsManager &manager)
    {
        VTSReply replyInfo;

        auto textSerialNo = static_cast<uint16_t>(manager.DecodeToNumerical(32, 11));
        auto messageType = static_cast<uint8_t>(manager.DecodeToNumerical(43, 2));
        if (messageType == 1/*&& textSerialNo == m_textSequenceNoVTS*/)
        {
            replyInfo.isAgreed = manager.DecodeToNumerical(45, 1) == 0;
            replyInfo.mmsiRequestor = static_cast<uint32_t>(manager.DecodeToNumerical(46, 30));
            auto length = static_cast<int>(manager.DecodeToNumerical(76, 9));
            auto bitPos = 85;
            if (length > 0)
            {
                auto text = manager.GetBytesEncoded(bitPos, bitPos + length * 8 - 1);
                replyInfo.shipNameChnRequestor = std::string(text.begin(), text.end());
            }
            bitPos += length * 8;
            replyInfo.shipNameEngRequestor = manager.DecodeToString(bitPos, 120);
            UtilityInterface::RemoveTailCharacter(replyInfo.shipNameEngRequestor, '@');
            replyInfo.queueNo = static_cast<uint8_t>(manager.DecodeToNumerical(bitPos + 120, 7));
            auto waypointNum = static_cast<uint8_t>(manager.DecodeToNumerical(bitPos + 127, 6));
            for (auto i = 0U; i < waypointNum; i++)
            {
                Waypoint waypoint;
                auto     index = bitPos + 133 + i * 104;
                waypoint.coordinate.SetLongitude(DecodeCoordinate(manager, index, 28));
                waypoint.coordinate.SetLatitude(DecodeCoordinate(manager, index + 28, 27));
                waypoint.course = manager.DecodeToNumerical(index + 55, 12) / 10.0;
                waypoint.timestamp = manager.DecodeToNumerical(index + 67, 32);
                replyInfo.waypoints.emplace_back(waypoint);
            }
            auto index = bitPos + 133 + waypointNum * 104;
            length = static_cast<int>(manager.DecodeToNumerical(index, 11));
            if (length > 0)
            {
                index += 11;
                auto text = manager.GetBytesEncoded(index, index + length * 8 - 1);
                replyInfo.remarkChn = std::string(text.begin(), text.end());
            }
            index += length * 8;
            length = static_cast<int>(manager.DecodeToNumerical(index, 10));
            if (length > 0)
            {
                index += 10;
                auto text = manager.GetBytesEncoded(index, index + length * 8 - 1);
                auto temp = manager.DecodeToString(index, length * 6);
                replyInfo.remarkEng = std::string(text.begin(), text.end());
            }
            replyInfo.requestDataID = UpdateSentVTSRequest(textSerialNo, replyInfo.isAgreed ? 0 : 1);

            std::unique_lock<std::mutex> lock(m_mutexVTSReply);
            SaveVTSReply(replyInfo, textSerialNo);
            lock.unlock();

            m_parent->vtsReplyEvent(replyInfo);
        }
    }

    void VDESManager::Impl::HandleASMMessage(ASMManager::ASMPtr asmData)
    {
        if (asmData)
        {
            if (asmData->DAC == 412 && asmData->FI == 46)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_46>(asmData);
                if (info)
                {
                    RouteRecommendationResponse response;
                    response.effectiveTime = info->effectiveTime;
                    response.coordinates = info->coordinates;
                    response.timestamp = UtilityInterface::GetCurrentTimeStamp();
                    response.read = false;

                    std::unique_lock<std::mutex> lock(m_mutexRouteRecommendationResponse);
                    SaveRouteRecommendationResponse(response);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::ASM_ROUTE_RECOMMENDATION_RESPONSE, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 49)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_49>(asmData);
                if (info)
                {
                    HydrometeorologyResponse response;
                    response.MRN = info->MRN;
                    response.forecastTime = info->forecastTime;
                    response.hasWindSpeed = info->hasWindSpeed;
                    response.hasWindDirection = info->hasWindDirection;
                    response.hasVisibility = info->hasVisibility;
                    response.hasWaveHeight = info->hasWaveHeight;
                    response.hasWaveDirection = info->hasWaveDirection;
                    response.hasSwellHeight = info->hasSwellHeight;
                    
                    response.points.clear();
                    for (const auto &p : info->points)
                    {
                        HydrometeorologyResponse::PointForecast pt;
                        pt.windSpeed = p.windSpeed;
                        pt.windDirection = p.windDirection;
                        pt.visibility = p.visibility;
                        pt.waveHeight = p.waveHeight;
                        pt.waveDirection = p.waveDirection;
                        pt.swellHeight = p.swellHeight;
                        response.points.push_back(pt);
                    }
                    response.timestamp = UtilityInterface::GetCurrentTimeStamp();
                    response.read = false;

                    std::unique_lock<std::mutex> lock(m_mutexHydrometeorologyResponse);
                    SaveHydrometeorologyResponse(response);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::ASM_HYDROMETEOROLOGY_RESPONSE, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 50)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_50>(asmData);
                if (info)
                {
                    std::unique_lock<std::mutex> lock(m_mutexOtherVesselExtendedInfo);
                    SaveOtherVesselExtendedInfoPartA(*info);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::ASM_EXTENDED_VESSEL_A, info->mmsi);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 51)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_51>(asmData);
                if (info)
                {
                    std::unique_lock<std::mutex> lock(m_mutexOtherVesselExtendedInfo);
                    SaveOtherVesselExtendedInfoPartB(*info);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::ASM_EXTENDED_VESSEL_B, info->mmsi);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 26)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC412_FI26>(asmData);
                if (info)
                {
                    MarineMeteorologyFCSTs container;
                    auto                   timestampNow = UtilityInterface::GetCurrentTimeStamp();
                    auto                   timeZone = UtilityInterface::GetTimeZone();
                    auto                   timestampDayBegin = timestampNow - (timestampNow % (24 * 3600)) - timeZone;

                    if (info->hourPublish < 24)
                    {
                        int currentHour = static_cast<int>(((timestampNow + timeZone) % 86400) / 3600);
                        if (info->hourPublish > currentHour)
                        {
                            timestampDayBegin -= 24 * 3600;
                        }
                    }
                    timestampDayBegin += info->hourPublish * 3600;

                    for (auto i = 0U; i < info->locationInfos.size(); i++)
                    {
                        MarineMeteorologyFCST fcst;
                        auto &locationInfo = info->locationInfos.at(i);
                        fcst.weatherCode = locationInfo.weatherCode;
                        fcst.coordinate = locationInfo.coordinate;
                        fcst.windSpeed = locationInfo.windSpeed;
                        fcst.windDirection = locationInfo.windDirection;
                        fcst.temperature = locationInfo.temperature;
                        fcst.airPressure = locationInfo.airPressure;
                        fcst.visibility = locationInfo.visibility;
                        fcst.infoSource = info->infoSource;
                        fcst.timestampFCST = timestampDayBegin + info->hoursOffsetFCST * 3600;
                        fcst.timestamp = timestampDayBegin;
                        container.emplace_back(fcst);
                    }
                    if (!container.empty())
                    {
                        std::unique_lock<std::mutex> lock(m_mutexMarineMeteorologyFCST);
                        SaveMarineMeteorologyFCST(container);
                        lock.unlock();
                        m_parent->notifyEvent(EventType::ASM_MARINE_METEOROLOGY_FCST, 0);
                    }
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 27)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_27>(asmData);
                if (info)
                {
                    MarineMeteorologyFCSTAreas container;

                    auto timestampNow      = UtilityInterface::GetCurrentTimeStamp();
                    auto timeZone          = UtilityInterface::GetTimeZone();
                    auto timestampDayBegin = timestampNow - (timestampNow % (24 * 3600)) - timeZone;

                    if (info->hourPublish < 24)
                    {
                        int currentHour = static_cast<int>(((timestampNow + timeZone) % 86400) / 3600);
                        if (info->hourPublish > currentHour)
                        {
                            timestampDayBegin -= 24 * 3600;
                        }
                    }
                    timestampDayBegin += info->hourPublish * 3600;
                    for (const auto &seaAreaInfo : info->seaAreaInfos)
                    {
                        MarineMeteorologyFCSTArea area;
                        area.areaCode = seaAreaInfo.areaCode;
                        area.weatherCode = seaAreaInfo.weatherCode;
                        area.windDirection = seaAreaInfo.windDirection;
                        area.windScaleLow = seaAreaInfo.windScaleLow;
                        area.windScaleHigh = seaAreaInfo.windScaleHigh;
                        area.visibility = seaAreaInfo.visibility;
                        area.infoSource = info->infoSource;
                        area.timestampFCST = timestampDayBegin + info->hoursOffsetFCST * 3600;
                        area.timestamp = timestampDayBegin;
                        container.emplace_back(area);
                    }

                    if (!container.empty())
                    {
                        std::unique_lock<std::mutex> lock(m_mutexMarineMeteorologyFCSTArea);
                        SaveMarineMeteorologyFCSTArea(container);
                        lock.unlock();
                        m_parent->notifyEvent(EventType::ASM_MARINE_METEOROLOGY_FCST_AREA, 0);
                    }
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 28)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC412_FI28>(asmData);
                if (info)
                {
                    MarineEnvironmentFCSTs container;

                    auto timestampNow      = UtilityInterface::GetCurrentTimeStamp();
                    auto timeZone          = UtilityInterface::GetTimeZone();
                    auto timestampDayBegin = timestampNow - (timestampNow % (24 * 3600)) - timeZone;

                    if (info->hourPublish < 24)
                    {
                        int currentHour = static_cast<int>(((timestampNow + timeZone) % 86400) / 3600);
                        if (info->hourPublish > currentHour)
                        {
                            timestampDayBegin -= 24 * 3600;
                        }
                    }
                    timestampDayBegin += info->hourPublish * 3600;

                    for (const auto &locationInfo : info->locationInfos)
                    {
                        MarineEnvironmentFCST fcst;
                        fcst.coordinate = locationInfo.coordinate;
                        fcst.flowVelocity = locationInfo.flowVelocity;
                        fcst.flowDirection = locationInfo.flowDirection;
                        fcst.waveHeight = locationInfo.waveHeight;
                        fcst.waveDirection = locationInfo.waveDirection;
                        fcst.temperature = locationInfo.temperature;
                        fcst.infoSource = info->infoSource;
                        fcst.timestampFCST = timestampDayBegin + info->hoursOffsetFCST * 3600;
                        fcst.timestamp = timestampDayBegin;
                        container.emplace_back(fcst);
                    }
                    if (!container.empty())
                    {
                        std::unique_lock<std::mutex> lock(m_mutexMarineEnvironmentFCST);
                        SaveMarineEnvironmentFCST(container);
                        lock.unlock();
                        m_parent->notifyEvent(EventType::ASM_MARINE_ENVIRONMENT_FCST, 0);
                    }
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 29)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_29>(asmData);
                if (info)
                {
                    MarineEnvironmentFCSTAreas container;

                    auto timestampNow      = UtilityInterface::GetCurrentTimeStamp();
                    auto timeZone          = UtilityInterface::GetTimeZone();
                    auto timestampDayBegin = timestampNow - (timestampNow % (24 * 3600)) - timeZone;

                    if (info->hourPublish < 24)
                    {
                        int currentHour = static_cast<int>(((timestampNow + timeZone) % 86400) / 3600);
                        if (info->hourPublish > currentHour)
                        {
                            timestampDayBegin -= 24 * 3600;
                        }
                    }
                    timestampDayBegin += info->hourPublish * 3600;

                    for (const auto &seaAreaInfo : info->seaAreaInfos)
                    {
                        MarineEnvironmentFCSTArea area;
                        area.areaCode = seaAreaInfo.areaCode;
                        area.temperatureLow = seaAreaInfo.temperatureLow;
                        area.temperatureHigh = seaAreaInfo.temperatureHigh;
                        area.flowDirectionAvg = seaAreaInfo.flowDirectionAvg;
                        area.flowDirctionMax = seaAreaInfo.flowDirctionMax;
                        area.flowVelocityAvg = seaAreaInfo.flowVelocityAvg;
                        area.flowVelocityMax = seaAreaInfo.flowVelocityMax;
                        area.waveHeight = seaAreaInfo.waveHeight;
                        area.swellHeight = seaAreaInfo.swellHeight;
                        area.infoSource = info->infoSource;
                        area.timestampFCST = timestampDayBegin + info->hoursOffsetFCST * 3600;
                        area.timestamp = timestampDayBegin;
                        container.emplace_back(area);
                    }

                    if (!container.empty())
                    {
                        std::unique_lock<std::mutex> lock(m_mutexMarineEnvironmentFCSTArea);
                        SaveMarineEnvironmentFCSTArea(container);
                        lock.unlock();
                        m_parent->notifyEvent(EventType::ASM_MARINE_ENVIRONMENT_FCST_AREA, 0);
                    }
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 30)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_30>(asmData);
                if (info)
                {
                    MarineEnvironmentFCSTAlongshores container;

                    auto timestampNow      = UtilityInterface::GetCurrentTimeStamp();
                    auto timeZone          = UtilityInterface::GetTimeZone();
                    auto timestampDayBegin = timestampNow - (timestampNow % (24 * 3600)) - timeZone;

                    if (info->hourPublish < 24)
                    {
                        int currentHour = static_cast<int>(((timestampNow + timeZone) % 86400) / 3600);
                        if (info->hourPublish > currentHour)
                        {
                            timestampDayBegin -= 24 * 3600;
                        }
                    }
                    timestampDayBegin += info->hourPublish * 3600;

                    for (const auto &waterAreaInfo : info->waterAreaInfos)
                    {
                        MarineEnvironmentFCSTAlongshore area;
                        area.areaCode = waterAreaInfo.areaCode;
                        area.temperatureLow = waterAreaInfo.temperatureLow;
                        area.temperatureHigh = waterAreaInfo.temperatureHigh;
                        area.waveHeightLow = waterAreaInfo.waveHeightLow;
                        area.waveHeightHigh = waterAreaInfo.waveHeightHigh;
                        area.tideHigh = waterAreaInfo.tideHigh;
                        area.timestampTideHigh = waterAreaInfo.timestampTideHigh;
                        area.tideLow = waterAreaInfo.tideLow;
                        area.timestampTideLow = waterAreaInfo.timestampTideLow;
                        area.infoSource = info->infoSource;
                        area.timestampFCST = timestampDayBegin + info->hoursOffsetFCST * 3600;
                        area.timestamp = timestampDayBegin;
                        container.emplace_back(area);
                    }

                    if (!container.empty())
                    {
                        std::unique_lock<std::mutex> lock(m_mutexMarineEnvironmentFCSTAlongshore);
                        SaveMarineEnvironmentFCSTAlongshore(container);
                        lock.unlock();
                        m_parent->notifyEvent(EventType::ASM_MARINE_ENVIRONMENT_FCST_ALONGSHORE, 0);
                    }
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 35)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_35>(asmData);
                if (info)
                {
                    MSIObstacle obstacle;

                    obstacle.type = info->type;
                    obstacle.coordinate = info->coordinate;
                    obstacle.timestampStart = info->timestampActivate;
                    
                    if (info->duration == 0)
                    {
                        obstacle.timestampEnd = 0;
                    }
                    else
                    {
                        obstacle.timestampEnd = info->timestampActivate + info->duration * 24 * 60 * 60;
                    }

                    obstacle.comment = info->cautionCode;
                    obstacle.isCertified = true;
                    obstacle.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    obstacle.geometryType = info->geometryType;

                    if (info->geometryType == 0)
                    {
                        obstacle.range = info->range * 0.00054; // meters to nm
                    }
                    else if (info->geometryType == 1)
                    {
                        obstacle.range = info->sector.range; // already in nm
                        obstacle.sectorStartAngle = info->sector.startAngle;
                        obstacle.sectorEndAngle = info->sector.endAngle;
                    }
                    else if (info->geometryType == 2 || info->geometryType == 3)
                    {
                        obstacle.range = 0.0;
                        for (const auto &pt : info->polygon)
                        {
                            obstacle.polygon.push_back(pt);
                        }
                    }

                    std::unique_lock<std::mutex> lock(m_mutexMSIObstacle);
                    SaveMSIObstacle(obstacle); 
                    lock.unlock();

                    m_parent->notifyEvent(EventType::MSI_OBSTACLE, 0);
                }
            }
            if (asmData->DAC == 412 && asmData->FI == 36)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_36>(asmData);
                if (info)
                {
                    MSIMaritimeOperation operation;

                    operation.type = info->type;
                    operation.status = 0;
                    operation.mmsi = info->mmsi;
                    operation.geometryType = info->geometryType;
                    operation.timeType = info->timeType;
                    operation.cautionCode = info->cautionCode;

                    if (info->geometryType == 0)
                    {
                        operation.coordinateNE = info->coordinate;
                        operation.range = info->range;
                    }
                    else if (info->geometryType == 1)
                    {
                        operation.coordinateNE = info->coordinate;
                        operation.range = info->sector.range;
                        operation.sectorStartAngle = info->sector.startAngle;
                        operation.sectorEndAngle = info->sector.endAngle;
                    }
                    else if (info->geometryType == 2 || info->geometryType == 3)
                    {
                        if (!info->polygon.empty())
                        {
                            operation.coordinateNE = info->polygon[0];
                        }
                        operation.polygon = info->polygon;
                    }

                    operation.timestampStart = info->timestampStart;
                    operation.timestampEnd = info->timestampEnd;
                    operation.isCertified = true;
                    operation.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    std::unique_lock<std::mutex> lock(m_mutexMSIMaritimeOperation);
                    SaveMSIMaritineOperation(operation);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::MSI_MARITIME_OPERATION, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 37)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_37>(asmData);
                if (info)
                {
                    MSIMaritimeTowing towing;

                    towing.mmsi = info->mmsi;
                    towing.coordinateStart = info->node1;
                    towing.coordinateEnd = info->node2;
                    towing.length = info->length;
                    towing.width = info->width;
                    towing.speed = info->speed;
                    towing.timestampStart = info->timestampStart;
                    towing.timestampEnd = info->timestampEnd;
                    towing.cautionCode = info->cautionCode;
                    towing.isCertified = true;
                    towing.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    std::unique_lock<std::mutex> lock(m_mutexMSIMaritimeTowing);
                    SaveMSIMaritimeTowing(towing);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::ASM_MARITIME_TOWING, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 38)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_38>(asmData);
                if (info)
                {
                    MSIMilitaryActivity activity;
                    
                    activity.theme = info->type;
                    activity.geometryType = info->geoType;
                    activity.timeType = info->timeType;
                    activity.isContinuing = (info->timeType == 0);
                    activity.cautionCode = info->cautionCode;

                    if (info->geoType == 0)
                    {
                        activity.coordinateNE = info->coordinate;
                        activity.range = info->range;
                    }
                    else if (info->geoType == 1)
                    {
                        activity.coordinateNE = info->coordinate;
                        activity.range = info->sector.range;
                        activity.sectorStartAngle = info->sector.startAngle;
                        activity.sectorEndAngle = info->sector.endAngle;
                    }
                    else if (info->geoType == 2 || info->geoType == 3)
                    {
                        if (!info->polygon.empty())
                        {
                            activity.coordinateNE = info->polygon[0];
                        }
                        activity.polygon = info->polygon;
                    }

                    activity.timestampStart = info->timestampStart;
                    activity.timestampEnd = info->timestampEnd;
                    activity.isCertified = true;
                    activity.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    std::unique_lock<std::mutex> lock(m_mutexMSIMilitaryActivity);
                    SaveMSIMilitaryActivity(activity);
                    lock.unlock();
                    SPDLOG_DEBUG("Send event:MSI_MILITARY_ACTIVITY");
                    m_parent->notifyEvent(EventType::MSI_MILITARY_ACTIVITY, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 39)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_39>(asmData);
                if (info)
                {
                    MSIMaritimeDistress distress;

                    distress.type = info->distressType;
                    distress.situation = 0;
                    distress.status = 0;
                    distress.additional = 0;

                    distress.coordinate = info->coordinate;
                    distress.timestampValid = info->distressTime;

                    distress.statusDescription = info->statusDescription;
                    distress.judgment = info->judgment;
                    distress.locationInstruction = info->locationInstruction;
                    distress.duration = info->duration;
                    distress.cautionCode = info->cautionCode;

                    distress.isCertified = true;
                    distress.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    std::unique_lock<std::mutex> lock(m_mutexMSIMaritimeDistress);
                    SaveMSIMaritimeDistress(distress);
                    lock.unlock();

                    SPDLOG_DEBUG("Send event:MSI_MARITIME_DISTRESS");
                    m_parent->notifyEvent(EventType::MSI_MARITIME_DISTRESS, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 40)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_40>(asmData);
                if (info)
                {
                    MSIDesignatedArea area;
                    area.MRN = info->MRN;
                    area.fragment = info->fragment;
                    area.areaType = info->type;
                    area.geometryType = info->geoType;
                    area.coordinateNE = info->coordinate;
                    area.range = info->range;
                    area.sectorStartAngle = info->sector.startAngle;
                    area.sectorEndAngle = info->sector.endAngle;
                    area.polygon = info->polygon;
                    area.timeType = info->timeType;
                    area.timestampStart = info->timestampStart;
                    area.timestampEnd = info->timestampEnd;
                    area.cautionCode = info->cautionCode;
                    area.isCertified = true;
                    area.timestamp = UtilityInterface::GetCurrentTimeStamp();
                    area.read = false;

                    std::unique_lock<std::mutex> lock(m_mutexMSIDesignatedArea);
                    SaveMSIDesignatedArea(area);
                    lock.unlock();

                    SPDLOG_DEBUG("Send event:MSI_DESIGNATED_AREA");
                    m_parent->notifyEvent(EventType::MSI_DESIGNATED_AREA, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 41)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_41>(asmData);
                if (info)
                {
                    std::unique_lock<std::mutex> lock(m_mutexBridge);
                    try
                    {
                        if (m_database)
                        {
                            auto sqlDel = fmt::format("DELETE FROM Bridge WHERE MRN = {}", info->MRN);
                            m_database->exec(sqlDel);
                        }
                    }
                    catch (const SQLite::Exception &execption)
                    {
                        DatabaseErrorProcess(execption, "HandleASMMessage_DeleteBridgeSpans");
                    }

                    for (const auto &span : info->bridgeSpans)
                    {
                        Bridge bridge;
                        bridge.MRN = info->MRN;
                        bridge.fragment = info->fragment;
                        bridge.status = 0;
                        bridge.center = span.center;
                        bridge.height = span.height;
                        bridge.width = span.width;
                        bridge.directionToPass = span.directionToPass;
                        bridge.passAbility = span.passAbility;
                        bridge.enableMeeting = span.enableMeeting;
                        bridge.enableOvertaking = span.enableOvertaking;
                        bridge.flowVelocity = info->flowVelocity;
                        bridge.flowDirection = info->flowDirection;
						bridge.timestamp = UtilityInterface::GetCurrentTimeStamp();
                        
                        SaveBridge(bridge);
                    }
                    lock.unlock();

                    if (!info->bridgeSpans.empty())
                    {
                        m_parent->notifyEvent(EventType::ASM_BRIDGE, 0);
                    }
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 44)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_44>(asmData);
                if (info)
                {
                    NetSounder netSounder;
                    netSounder.MRN = info->MRN;
                    netSounder.fragment = info->fragment;
                    netSounder.type = info->type;
                    netSounder.isContinous = info->isContinous;
                    netSounder.coordinates = info->coordinates;
					netSounder.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    std::unique_lock<std::mutex> lock(m_mutexNetSounder);
                    try
                    {
                        if (m_database)
                        {
                            auto sqlDel = fmt::format("DELETE FROM NetSounder WHERE MRN = {}", info->MRN);
                            m_database->exec(sqlDel);
                        }
                    }
                    catch (const SQLite::Exception &execption)
                    {
                        DatabaseErrorProcess(execption, "HandleASMMessage_DeleteNetSounder");
                    }

                    SaveNetSounder(netSounder);
                    lock.unlock();

                    if (!info->coordinates.empty())
                    {
                        m_parent->notifyEvent(EventType::ASM_NET_SOUNDER, 0);
                    }
                }
            }

            if (asmData->DAC == 413 && asmData->FI == 5)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_413_FI_5>(asmData);
                if (info)
                {
                    FrontendPrompt prompt;
                    prompt.promptContent = info->promptContent;
                    prompt.displayDuration = info->displayDuration;
                    prompt.text = info->text;
                    prompt.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    std::unique_lock<std::mutex> lock(m_mutexFrontendPrompt);
                    SaveFrontendPrompt(prompt);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::ASM_FRONTEND_PROMPT, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 42)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_42>(asmData);
                if (info)
                {
                    ChannelCenterline centerline;
                    centerline.MRN = info->MRN;
                    centerline.fragment = info->fragment;
                    centerline.width = info->width;
                    centerline.coordinates = info->coordinates;
					centerline.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    std::unique_lock<std::mutex> lock(m_mutexChannelCenterline);
                    try
                    {
                        if (m_database)
                        {
                            auto sqlDel = fmt::format("DELETE FROM ChannelCenterline WHERE MRN = {}", info->MRN);
                            m_database->exec(sqlDel);
                        }
                    }
                    catch (const SQLite::Exception &execption)
                    {
                        DatabaseErrorProcess(execption, "HandleASMMessage_DeleteChannelCenterline");
                    }

                    SaveChannelCenterline(centerline);
                    lock.unlock();

                    if (!info->coordinates.empty())
                    {
                        m_parent->notifyEvent(EventType::ASM_CHANNEL_CENTERLINE, 0);
                    }
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 43)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_43>(asmData);
                if (info)
                {
                    ChannelBoundary boundary;
                    boundary.MRN = info->MRN;
                    boundary.fragment = info->fragment;
                    boundary.edgeType = info->edgeType;
                    boundary.coordinates = info->coordinates;
					boundary.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    std::unique_lock<std::mutex> lock(m_mutexChannelBoundary);
                    try
                    {
                        if (m_database)
                        {
                            auto sqlDel = fmt::format("DELETE FROM ChannelBoundary WHERE MRN = {} AND EdgeType = {}", info->MRN, info->edgeType);
                            m_database->exec(sqlDel);
                        }
                    }
                    catch (const SQLite::Exception &execption)
                    {
                        DatabaseErrorProcess(execption, "HandleASMMessage_DeleteChannelBoundary");
                    }

                    SaveChannelBoundary(boundary);
                    lock.unlock();

                    if (!info->coordinates.empty())
                    {
                        m_parent->notifyEvent(EventType::ASM_CHANNEL_BOUNDARY, 0);
                    }
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 31)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_31>(asmData);
                if (info)
                {
                    MarineMeteorologyEW ew;
                    ew.DAC = info->DAC;
                    ew.FI = info->FI;
                    ew.warningType = info->warningType;

                    if (info->warningType == 1)
                    {
                        ew.MRN = info->cyclone.MRN;
                        ew.fragment = info->cyclone.fragment;
                        for (const auto &pt : info->cyclone.pathPoints)
                        {
                            MarineMeteorologyEW::TropicalCyclonePathPoint dbPt;
                            dbPt.timestamp = pt.timestamp;
                            dbPt.centerLongitude = pt.centerLongitude;
                            dbPt.centerLatitude = pt.centerLatitude;
                            dbPt.cycloneType = pt.cycloneType;
                            dbPt.radiusWindScale7 = pt.radiusWindScale7;
                            dbPt.radiusWindScale10 = pt.radiusWindScale10;
                            dbPt.radiusWindScale12 = pt.radiusWindScale12;
                            dbPt.moveSpeed = pt.moveSpeed;
                            dbPt.moveDirection = pt.moveDirection;
                            dbPt.maxWindScale = pt.maxWindScale;
                            dbPt.centerPressure = pt.centerPressure;
                            ew.cyclonePathPoints.push_back(dbPt);
                        }
                    }
                    else if (info->warningType >= 2 && info->warningType <= 4)
                    {
                        for (const auto &elem : info->generalWarnings)
                        {
                            MarineMeteorologyEW::WarningElement we;
                            we.MRN = elem.MRN;
                            we.areaCode = elem.seaAreaCode;
                            we.warningLevel = elem.warningLevel;
                            we.surgeHeight = 0.0;
                            ew.elements.push_back(we);
                        }
                    }
                    else if (info->warningType == 5)
                    {
                        for (const auto &elem : info->stormSurges)
                        {
                            MarineMeteorologyEW::WarningElement we;
                            we.MRN = elem.MRN;
                            we.areaCode = elem.cityCode;
                            we.warningLevel = elem.warningLevel;
                            we.surgeHeight = elem.surgeHeight;
                            ew.elements.push_back(we);
                        }
                    }
                    else if (info->warningType == 6)
                    {
                        MarineMeteorologyEW::WarningElement we;
                        we.MRN = info->iceWarning.MRN;
                        we.areaCode = info->iceWarning.regionCode;
                        we.warningLevel = info->iceWarning.warningLevel;
                        we.surgeHeight = 0.0;
                        ew.elements.push_back(we);
                    }

                    ew.timestampPublished = info->publishTime;
                    ew.timestampStart = info->startTime;
                    ew.timestampEnd = info->endTime;
                    ew.warningDuration = info->warningDuration;
                    ew.infoSource = info->infoSource;
                    ew.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    std::unique_lock<std::mutex> lock(m_mutexMarineMeteorologyEW);
                    SaveMarineMeteorologyEW(ew);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::ASM_MARINE_METEOROLOGY_EW, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 32)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_32>(asmData);
                if (info)
                {
                    TideForecast forecast;
                    forecast.DAC = info->DAC;
                    forecast.FI = info->FI;
                    forecast.hourPublish = info->hourPublish;
                    forecast.infoSource = info->infoSource;
                    forecast.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    for (const auto &stationInfo : info->tideInfos)
                    {
                        TideForecast::TideStation station;
                        station.coordinate = stationInfo.coordinate;
                        station.tidalDatum = stationInfo.tidalDatum;
                        station.tideHigh = stationInfo.tideHigh;
                        station.timestampTideHigh = stationInfo.timestampTideHigh;
                        station.tideLow = stationInfo.tideLow;
                        station.timestampTideLow = stationInfo.timestampTideLow;
                        forecast.stations.emplace_back(station);
                    }

                    std::unique_lock<std::mutex> lock(m_mutexTideForecast);
                    SaveTideForecast(forecast);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::ASM_TIDE_FCST, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 33)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_33>(asmData);
                if (info)
                {
                    AtoNDynamics dynamics;
                    dynamics.DAC = info->DAC;
                    dynamics.FI = info->FI;
                    dynamics.MRN = info->MRN;
                    dynamics.fragment = info->fragment;
                    dynamics.status = info->status;
                    dynamics.precaution = info->precaution;
                    dynamics.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    for (const auto &elemInfo : info->elements)
                    {
                        AtoNDynamics::Element elem;
                        elem.MRN = elemInfo.MRN;
                        elem.type = elemInfo.type;
                        elem.coordinate = elemInfo.coordinate;
                        elem.rhythmNameCode = elemInfo.rhythmNameCode;
                        elem.rhythmParamCode = elemInfo.rhythmParamCode;
                        elem.bodyColor = elemInfo.bodyColor;
                        elem.lightColor = elemInfo.lightColor;
                        elem.lightPeriod = elemInfo.lightPeriod;
                        elem.range = elemInfo.range;
                        elem.morseCode = elemInfo.morseCode;
                        elem.prevCoordinate = elemInfo.prevCoordinate;
                        elem.isRoughPosition = elemInfo.isRoughPosition;
                        elem.failureType = elemInfo.failureType;
                        elem.timestampStart = elemInfo.timestampStart;
                        elem.timestampEnd = elemInfo.timestampEnd;
                        dynamics.elements.emplace_back(elem);
                    }

                    std::unique_lock<std::mutex> lock(m_mutexAtoNDynamics);
                    SaveAtoNDynamics(dynamics);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::ASM_ATON_DYNAMICS, 0);
                }
            }

            if (asmData->DAC == 412 && asmData->FI == 34)
            {
                auto info = std::dynamic_pointer_cast<ASM_DAC_412_FI_34>(asmData);
                if (info)
                {
                    AISAtoNDynamics dynamics;
                    dynamics.DAC = info->DAC;
                    dynamics.FI = info->FI;
                    dynamics.MRN = info->MRN;
                    dynamics.fragment = info->fragment;
                    dynamics.atonAttribute = info->atonAttribute;
                    dynamics.status = info->status;
                    dynamics.precaution = info->precaution;
                    dynamics.timestamp = UtilityInterface::GetCurrentTimeStamp();

                    for (const auto &elemInfo : info->elements)
                    {
                        AISAtoNDynamics::Element elem;
                        elem.mmsi = elemInfo.mmsi;
                        elem.MRN = elemInfo.MRN;
                        elem.type = elemInfo.type;
                        elem.coordinate = elemInfo.coordinate;
                        elem.range = elemInfo.range;
                        elem.prevCoordinate = elemInfo.prevCoordinate;
                        elem.isRoughPosition = elemInfo.isRoughPosition;
                        dynamics.elements.emplace_back(elem);
                    }

                    std::unique_lock<std::mutex> lock(m_mutexAISAtoNDynamics);
                    SaveAISAtoNDynamics(dynamics);
                    lock.unlock();

                    m_parent->notifyEvent(EventType::ASM_AIS_ATON_DYNAMICS, 0);
                }
            }
        }
    }

    void VDESManager::Impl::SendAISMessage6(const uint32_t mmsiDestination, const std::string &content)
    {
        auto BuildFunc = [&](uint16_t DAC, uint8_t FI)
        {
            AISBitsManager aisBitsManager;
            auto           isIFM0 = (DAC == 1 && FI == 0) ? true : false;

            aisBitsManager.Encode(DAC, 10);
            aisBitsManager.Encode(FI, 6);
            if (isIFM0)
            {
                aisBitsManager.Encode(1, 1);
                aisBitsManager.Encode(0, 11);
            }
            aisBitsManager.Encode(content, DAC, FI);
            if (isIFM0)
            {
                auto surplus = (100 + 6 * content.length()) % 8;
                auto bitNumReserve = (surplus == 0 ? 0 : (8 - surplus));
                aisBitsManager.Encode(0, bitNumReserve);
            }

            auto fillBitsNum = aisBitsManager.GetFillBitsNumberToEncode();
            auto encapsulatedData = aisBitsManager.GetEncodedVDMPayload();
            // Build BBM sentence
            auto length = encapsulatedData.length();
            auto count = length / 48;
            if (length % 48 != 0)
            {
                count++;
            }

            for (auto i = 0U; i < count; i++)
            {
                auto str = fmt::format("!AIABM,{},{},0,{:09d},0,6,{},{}",
                                       count, i + 1, mmsiDestination, 
                                       encapsulatedData.substr(i * 48, 48),
                                       ((i == (count - 1)) ? fillBitsNum : 0));
                UtilityInterface::AddChecksum(str);
                m_parent->sendEvent(CommunicationType::TCP, str.c_str(), str.length());
            }
        };

        auto isASCIIOnly = UtilityInterface::IsASCIICode(content);

        if (isASCIIOnly)
        {
            BuildFunc(1, 0);
            UtilityInterface::Sleep(100);
            BuildFunc(413, 1);
        }
        else
        {
            BuildFunc(413, 1);
        }
    }

    void VDESManager::Impl::SendAISMessage8(const std::string &content)
    {
        auto BuildFunc = [&](uint16_t DAC, uint8_t FI)
        {
            AISBitsManager aisBitsManager;
            bool isIFM0 = (DAC == 1 && FI == 0) ? true : false;

            aisBitsManager.Encode(DAC, 10);
            aisBitsManager.Encode(FI, 6);
            if (isIFM0)
            {
                aisBitsManager.Encode(0, 1);
                aisBitsManager.Encode(0, 11);
            }
            aisBitsManager.Encode(content, DAC, FI);
            if (isIFM0)
            {
                auto surplus = (68 + 6 * content.length()) % 8;
                auto bitNumReserve = (surplus == 0 ? 0 : (8 - surplus));
                aisBitsManager.Encode(0, bitNumReserve);
            }

            auto fillBitsNum = aisBitsManager.GetFillBitsNumberToEncode();
            auto encapsulatedData = aisBitsManager.GetEncodedVDMPayload();
            // Build BBM sentence
            auto length = encapsulatedData.length();
            auto count = length / 48;
            if (length % 48 != 0)
            {
                count++;
            }

            for (auto i = 0U; i < count; i++)
            {
                auto str = fmt::format("!AIBBM,{},{},0,0,8,{},{}",
                                       count, i + 1, 
                                       encapsulatedData.substr(i * 48, 48),
                                       ((i == (count - 1)) ? fillBitsNum : 0));
                UtilityInterface::AddChecksum(str);
                m_parent->sendEvent(CommunicationType::TCP, str.c_str(), str.length());
            }
        };

        auto isASCIIOnly = UtilityInterface::IsASCIICode(content);
        if (isASCIIOnly)
        {
            BuildFunc(1, 0);
            UtilityInterface::Sleep(100);
            BuildFunc(413, 1);
        }
        else
        {
            BuildFunc(413, 1);
        }
    }

    void VDESManager::Impl::SendAISMessage12(const uint32_t mmsiDestination, const std::string &content)
    {
        AISBitsManager aisBitsManager;

        aisBitsManager.Encode(content, 1, 0);

        auto fillBitsNum = aisBitsManager.GetFillBitsNumberToEncode();
        auto encapsulatedData = aisBitsManager.GetEncodedVDMPayload();
        // Build BBM sentence
        auto length = encapsulatedData.length();
        auto count = length / 48;
        if (length % 48 != 0)
        {
            count++;
        }

        for (auto i = 0U; i < count; i++)
        {
            auto str = fmt::format("!AIABM,{},{},0,{:09d},0,12,{},{}",
                                   count, i + 1, mmsiDestination, 
                                   encapsulatedData.substr(i * 48, 48),
                                   ((i == (count - 1)) ? fillBitsNum : 0));
            UtilityInterface::AddChecksum(str);
            m_parent->sendEvent(CommunicationType::TCP, str.c_str(), str.length());
        }
    }

    void VDESManager::Impl::SendAISMessage14(const std::string &content)                                       
    {
        AISBitsManager aisBitsManager;

        aisBitsManager.Encode(content, 1, 0);

        auto fillBitsNum = aisBitsManager.GetFillBitsNumberToEncode();
        auto encapsulatedData = aisBitsManager.GetEncodedVDMPayload();
        // Build BBM sentence
        auto length = encapsulatedData.length();
        auto count = length / 48;
        if (length % 48 != 0)
        {
            count++;
        }   

        for (auto i = 0U; i < count; i++)
        {
            auto sql = fmt::format("!AIBBM,{},{},0,0,14,{},{}",
                                   count, i + 1, 
                                   encapsulatedData.substr(i * 48, 48),
                                   ((i == (count - 1)) ? fillBitsNum : 0));
            UtilityInterface::AddChecksum(sql);
            m_parent->sendEvent(CommunicationType::TCP, sql.c_str(), sql.length());
        }
    }

    void VDESManager::Impl::HandleConfigureEvent(ConfigureManager::EventType type, 
                                                 const int retCode)
    {
        switch (type)
        {
        case ConfigureManager::EventType::OWN_VESSEL_CONFIGURE:
            if (retCode == 0 || retCode == 2)
            {
                // check message inbox whether there is ais message that sent by self.
                if (m_database)
                {
                    try
                    {
                        std::lock_guard<std::mutex> lock(m_mutexMessageInbox);

                        auto ownShipInfo = ConfigureManager::GetInstance().GetOwnVesselInfo();
                        auto sql = fmt::format("DELETE FROM MessageInbox WHERE Source = {}", ownShipInfo.mmsi);
                        m_database->exec(sql);
                    }
                    catch (const SQLite::Exception &execption)
                    {
                        DatabaseErrorProcess(execption, "HandleConfigureEvent");
                    }
                }
            }
            break;
        }
    }

    VDESManager &VDES::VDESManager::GetInstance(void)
    {
        static VDESManager manager;
        return manager;
    }

    void VDESManager::Initialize(void)
    {
        m_impl->InitializeDatabase();

        auto func = std::bind(&VDESManager::Impl::HandleConfigureEvent, m_impl.get(), 
            std::placeholders::_1, std::placeholders::_2);
        VDES::ConfigureManager::GetInstance().notifyEvent.append(func);
    }

    bool VDESManager::IsInitialized(void) const
    {
        auto storagePath = VDES::ConfigureManager::GetInstance().GetStoragePath();

        return m_impl->m_database && !storagePath.empty();
    }

    void VDESManager::Parse(const char *data, uint32_t size)
    {
        SPDLOG_DEBUG("Parse: {}", std::string(data, size));
        for (auto i = 0U; i < size; i++)
        {
            m_impl->m_buffer[m_impl->m_pos++] = data[i];
            if (data[i] == '\r' || data[i] == '\n')
            {
                m_impl->m_buffer[m_impl->m_pos - 1] = '\0';
                if (m_impl->m_pos >= 2)
                {
                    char *ptr = nullptr;

                    if ((ptr = strchr(m_impl->m_buffer.data(), '!')) ||
                        (ptr = strchr(m_impl->m_buffer.data(), '$')))
                    {
                        auto size = m_impl->m_pos - 1 - (ptr - m_impl->m_buffer.data());
                        SPDLOG_DEBUG("ParseOneLineData: {}", std::string(ptr, size));
                        m_impl->ParseOneLineData(std::string(ptr, size));
                    }
                }
                m_impl->m_pos = 0;
            }
            else if (m_impl->m_pos >= Impl::BUFFER_SIZE)
            {
                m_impl->m_pos = 0;
            }
        }
    }

    void VDESManager::SetInformationRead(const InformationType type, const uint32_t dataID, const bool isRead)
    {
        std::string tableName;

        auto HandleReadStatus = [&](const std::string &tableName) {
            if (m_impl->m_database)
            {
                try
                {
                    auto sql = fmt::format("UPDATE {} SET Read = {} WHERE ID = {}",
                                            tableName, isRead ? 1 : 0, dataID);
                    m_impl->m_database->exec(sql);
                }
                catch (const SQLite::Exception &execption)
                {
                    m_impl->DatabaseErrorProcess(execption, "SetInformationRead");
                }
            }
        };

        switch (type)
        {
        case InformationType::MESSAGE_RECEIVED:
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMessageInbox);
                HandleReadStatus("MessageInbox");
            }
            break;
        case InformationType::ATON:
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAtoN);
                HandleReadStatus("AtoNs");
            }
            break;
        case InformationType::MSI_OBSTACLE:
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIObstacle);
                HandleReadStatus("MSIObstacle");
            }
            break;
        case InformationType::MSI_MARITINE_OPERATION:
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeOperation);
                HandleReadStatus("MSIMaritimeOperation");
            }
            break;
        case InformationType::MSI_MILITARY_ACTIVITY:
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMilitaryActivity);
                HandleReadStatus("MSIMilitaryActivity");
            }
            break;
        case InformationType::MSI_MARITIME_DISTRESS:
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeDistress);
                HandleReadStatus("MSIMaritimeDistress");
            }
            break;
        case InformationType::HYDRO_METEOROLOGY:
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexHydrometeorology);
                HandleReadStatus("Hydrometeorology");
            }
            break;
        case InformationType::ABNORMAL_SHIP:
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAbnormalShip);
                HandleReadStatus("AbnormalShip");
            }   
            break;
        case InformationType::MARINE_METEOROLOGY_FCST:
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineMeteorologyFCST);
                HandleReadStatus("MarineMeteorologyFCST");
            }
            break;
        case InformationType::MARINE_ENVIRONMENT_FCST:
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineEnvironmentFCST);
                HandleReadStatus("MarineEnvironmentFCST");
            }
            break;
        default:
            break;
        }
    }

    VDESManager::VDESManager()
    {
        m_impl = std::unique_ptr<Impl>(new Impl(this));
    }

    VDESManager::~VDESManager()
    {
    }

    /*******************************************************************************
    * Message business
    *******************************************************************************/
    VDESManager::Messages VDESManager::GetMessages(const MailBox mailbox, const uint32_t index,
                                                   const size_t number) const
    {
        Messages container;
        
        auto ObtainFunc = [&](std::string &sqlCmd) {
            try
            {
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    Message message;
                    if (mailbox == MailBox::INBOX)
                    {
                        m_impl->LoadInboxFromQueryResult(message, query);
                    }
                    else if (mailbox == MailBox::OUTBOX)
                    {
                        m_impl->LoadOutboxFromQueryResult(message, query);
                    }
                    container.push_back(message);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "ObtainFunc");
            }
            
        };

        if (m_impl->m_database)
        {
            if (mailbox == MailBox::INBOX)
            {
                auto sql = fmt::format("SELECT * FROM MessageInbox ORDER BY [Timestamp Receive] "
                    "DESC LIMIT {} OFFSET {}",
                    (number == -1) ? "-1" : fmt::format("{}", number), index);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMessageInbox);
                
                ObtainFunc(sql);
            }
            else if (mailbox == MailBox::OUTBOX)
            {
                auto sql = fmt::format("SELECT * FROM MessageOutbox ORDER BY [Timestamp Send] "
                    "DESC LIMIT {} OFFSET {}",
                    (number == -1) ? "-1" : fmt::format("{}", number), index);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMessageOutbox);

                ObtainFunc(sql);
            }
        } 
        return container;
    }

    VDESManager::Messages VDESManager::GetMessages(const uint32_t mmsi)
    {
        Messages container;

        if (m_impl->m_database)
        {
            try
            {
                std::unique_lock<std::mutex> lockInbox(m_impl->m_mutexMessageInbox);

                auto sqlInbox = fmt::format("SELECT * FROM MessageInbox WHERE Source = {}", mmsi);
                SQLite::Statement queryInbox(*m_impl->m_database.get(), sqlInbox);
                while (queryInbox.executeStep())
                {
                    Message message;
                    m_impl->LoadInboxFromQueryResult(message, queryInbox);
                    container.emplace_back(message);
                }
                lockInbox.unlock();

                std::unique_lock<std::mutex> lockOutbox(m_impl->m_mutexMessageOutbox);

                auto sqlOutbox = fmt::format("SELECT * FROM MessageOutbox WHERE Destination = {}", mmsi);
                SQLite::Statement queryOutbox(*m_impl->m_database.get(), sqlOutbox);
                while (queryOutbox.executeStep())
                {
                    Message message;
                    m_impl->LoadOutboxFromQueryResult(message, queryOutbox);
                    container.emplace_back(message);
                }
                lockOutbox.unlock();
            }
            catch(const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMessages");
            }
        }
        return container;
    }

    uint32_t VDESManager::GetUnReadMessageCount(const uint32_t mmsi)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lockInbox(m_impl->m_mutexMessageInbox);

                auto sql = fmt::format("SELECT COUNT(ID) FROM MessageInbox WHERE Source = {} AND Read = 0", mmsi);
                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    return query.getColumn(0).getInt();
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetUnReadMessageCount");
            }
        }
        return 0;
    }

    void VDESManager::ResetUnReadMessageCount(const uint32_t mmsi)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lockInbox(m_impl->m_mutexMessageInbox);

                auto sql = fmt::format("UPDATE MessageInbox SET Read = 1 WHERE Source = {}", mmsi);
                m_impl->m_database->exec(sql);
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "ResetUnReadMessageCount");
            }
        }    
    }

    bool VDESManager::DeleteMessages(const uint32_t mmsi)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database.get());

                std::unique_lock<std::mutex> lockInbox(m_impl->m_mutexMessageInbox);

                auto sqlInbox = fmt::format("DELETE FROM MessageInbox WHERE Source = {}", mmsi);
                m_impl->m_database->exec(sqlInbox);
                lockInbox.unlock();

                std::unique_lock<std::mutex> lockOutbox(m_impl->m_mutexMessageOutbox);

                auto sqlOutbox = fmt::format("DELETE FROM MessageOutbox WHERE Destination = {}", mmsi);
                m_impl->m_database->exec(sqlOutbox);
                lockOutbox.unlock();
                
                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMessages");
            }
        }
        return false;
    }

    std::vector<uint32_t> VDESManager::GetContactAddresses(const uint32_t mmsi)
    {
        std::vector<uint32_t> container;

        if (m_impl->m_database)
        {
            try
            {
                std::unique_lock<std::mutex> lockInbox(m_impl->m_mutexMessageInbox);

                auto sqlInbox = fmt::format("SELECT DISTINCT Source FROM MessageInbox WHERE Destination = {} OR "
                                            "Destination = 0", mmsi);
                SQLite::Statement queryInbox(*m_impl->m_database.get(), sqlInbox);
                while (queryInbox.executeStep())
                {
                    container.push_back(queryInbox.getColumn(0).getInt());
                }
                lockInbox.unlock();

                std::unique_lock<std::mutex> lockOutbox(m_impl->m_mutexMessageOutbox);
                
                auto sqlOutbox = fmt::format("SELECT DISTINCT Destination FROM MessageOutbox WHERE Source = {}", mmsi);
                SQLite::Statement queryOutbox(*m_impl->m_database.get(), sqlOutbox);
                while (queryOutbox.executeStep())
                {
                    container.push_back(queryOutbox.getColumn(0).getInt());
                }
                lockOutbox.unlock();
                
                std::sort(container.begin(), container.end());
                container.erase(std::unique(container.begin(), container.end()), container.end());
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetContactAddresses");
            }
        }
        return container;
    }

    VDESManager::Messages VDESManager::GetLastInteractionMessages(const uint32_t mmsi)
    {
        Messages container, inbox, outbox;

        if (!m_impl->m_database)
        {
            return container;
        }
        try
        {
            std::unique_lock<std::mutex> lockInbox(m_impl->m_mutexMessageInbox);
            auto sqlInbox = fmt::format("SELECT * FROM MessageInbox WHERE (Destination = {} OR Destination = 0) "
                                        "AND [Timestamp Receive] IN (SELECT MAX([Timestamp Receive]) "
                                        "FROM MessageInbox WHERE (Destination = {} OR Destination = 0) "
                                        "GROUP BY Source)", mmsi, mmsi);
            SQLite::Statement queryInbox(*m_impl->m_database.get(), sqlInbox);
            while (queryInbox.executeStep())
            {
                Message message;
                m_impl->LoadInboxFromQueryResult(message, queryInbox);
                inbox.push_back(message);
            }
            lockInbox.unlock();

            std::sort(inbox.begin(), inbox.end(), [](const Message &a, const Message &b) {
                return a.mmsiSource < b.mmsiSource;
            });
            auto last = std::unique(inbox.begin(), inbox.end(), [](const Message &a, const Message &b) {
                return a.mmsiSource == b.mmsiSource && a.timestamp == b.timestamp;
            });
            inbox.erase(last, inbox.end());


            std::unique_lock<std::mutex> lockOutbox(m_impl->m_mutexMessageOutbox);
            auto sqlOutbox = fmt::format("SELECT * FROM MessageOutbox WHERE Source = {} "
                                        "AND [Timestamp Send] IN (SELECT MAX([Timestamp Send]) "
                                        "FROM MessageOutbox WHERE Source = {} "
                                        "GROUP BY Destination)", mmsi, mmsi);
            SQLite::Statement queryOutbox(*m_impl->m_database.get(), sqlOutbox);
            while (queryOutbox.executeStep())
            {
                Message message;
                m_impl->LoadOutboxFromQueryResult(message, queryOutbox);
                outbox.push_back(message);
            }
            lockOutbox.unlock();

            std::sort(outbox.begin(), outbox.end(), [](const Message &a, const Message &b) {
                return a.mmsiDestination < b.mmsiDestination;
            });
            
            last = std::unique(outbox.begin(), outbox.end(), [](const Message &a, const Message &b) {
                return a.mmsiDestination == b.mmsiDestination && a.timestamp == b.timestamp;
            });
            outbox.erase(last, outbox.end());

            auto iterInbox = inbox.begin();
            
            while (iterInbox != inbox.end())
            {
                auto iterOutbox = outbox.begin();
                while (iterOutbox != outbox.end())
                {
                    if (iterInbox != inbox.end())
                    {
                        if (iterInbox->mmsiSource == iterOutbox->mmsiDestination)
                        {
                            if (iterInbox->timestamp > iterOutbox->timestamp)
                            {
                                iterOutbox = outbox.erase(iterOutbox);
                            }
                            else
                            {
                                iterInbox = inbox.erase(iterInbox);
                            }
                            continue;
                        } 
                    }
                    iterOutbox++;
                }
                if (iterInbox != inbox.end())
                {
					iterInbox++;
                }
            }
            container.insert(container.end(), inbox.begin(), inbox.end());
            container.insert(container.end(), outbox.begin(), outbox.end());
            
        }
        catch (const SQLite::Exception &execption)
        {
            m_impl->DatabaseErrorProcess(execption, "GetLastInteractionMessages");
        }
        return container;
    }

    bool VDESManager::DeleteMessages(const MailBox mailbox, const uint32_t index, 
                                     const size_t number)
    {
        if (m_impl->m_database)
        {
            try
            {
                if (mailbox == MailBox::INBOX)
                {
                    std::lock_guard<std::mutex> lock(m_impl->m_mutexMessageInbox);
                }
                else if (mailbox == MailBox::OUTBOX)
                {
                    std::lock_guard<std::mutex> lock(m_impl->m_mutexMessageOutbox);
                }

                auto tableName = (mailbox == MailBox::INBOX) ? "MessageInbox" : "MessageOutbox";
                auto fieldName = (mailbox == MailBox::INBOX) ? "[Timestamp Receive]" : "[Timestamp Send]";
                auto sql = fmt::format("DELETE FROM {} WHERE ID IN "
                                    "(SELECT ID FROM {} ORDER BY {} DESC "
                                    "LIMIT {} OFFSET {})",
                                    tableName, tableName, fieldName, number, index);
                m_impl->m_database->exec(sql);
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMessages");
            }
        }
        return false;
    }

    bool VDESManager::DeleteMessages(const MailBox mailbox, const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::unique_lock<std::mutex> lock;

                if (mailbox == MailBox::INBOX)
                {
                    lock = std::unique_lock<std::mutex>(m_impl->m_mutexMessageInbox);
                }
                else if (mailbox == MailBox::OUTBOX)
                {
                    lock = std::unique_lock<std::mutex>(m_impl->m_mutexMessageOutbox);
                }

                SQLite::Transaction transaction(*m_impl->m_database.get());

                for (auto &dataID : dataIDs)
                {
                    auto sql = fmt::format("DELETE FROM {} WHERE ID = {}", 
                        mailbox == MailBox::INBOX ? "MessageInbox" : "MessageOutbox", 
                        dataID);
                    m_impl->m_database->exec(sql);
                }
                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMessages");
            }
        }
        return false;
    }

    bool VDESManager::SendInformation(const uint32_t mmsiSource, const uint32_t mmsiDest,
                                      const std::string &content, const uint64_t deadline, 
                                      const bool needReply)
    {
        
        Impl::MessageBusinessType messageType = Impl::MessageBusinessType::SIX_BIT_ASCII;
        uint32_t                  sequenceNumber = 0;

        for (auto &character : content)
        {
            if ((character >= 0x40 && character <= 0x5F) ||
                (character >= 0x20 && character <= 0x3F))
            {
                continue;
            }
            
            if (character >= 0x60 && character <= 0x7E)
            {
                messageType = Impl::MessageBusinessType::UTF8;
                break;
            }
            messageType = Impl::MessageBusinessType::FOURTEEN_BIT;
        }

        AISBitsManager aisBitsManager;

        // VPFI
        aisBitsManager.Encode(2, 16);
        switch (messageType)
        {
        case Impl::MessageBusinessType::SIX_BIT_ASCII:
            {
                // Meesage No.
                aisBitsManager.Encode(0, 16);
                // Response indicator
                aisBitsManager.Encode((needReply && mmsiDest != 0) ? 1 : 0, 1);
                // Spare1
                aisBitsManager.Encode(0, 2);
                // Text sequence number
                aisBitsManager.Encode(++m_impl->m_textSequenceNoSixBitASCII, 11);
                sequenceNumber = m_impl->m_textSequenceNoSixBitASCII;
                if (m_impl->m_textSequenceNoSixBitASCII >= 1999)
                {
                    m_impl->m_textSequenceNoSixBitASCII = 0;
                }
                // Text string
                aisBitsManager.Encode(content, 1, 0);
                // Spare2
                auto spare2 = 8 - ((512 + 32 + aisBitsManager.GetBitsNumberToDecode()) % 8);
                aisBitsManager.Encode(0, spare2);
                // Deadline
                aisBitsManager.Encode(static_cast<uint32_t>(deadline), 32);
                // Authentication
                aisBitsManager.Encode(0, 512);
            }
            break;
        case Impl::MessageBusinessType::UTF8:
            {
                // Meesage No.
                aisBitsManager.Encode(1, 16);
                // Response indicator
                aisBitsManager.Encode((needReply &&mmsiDest != 0) ? 1 : 0, 1);
                // Spare1
                aisBitsManager.Encode(0, 2);
                // Text sequence number
                aisBitsManager.Encode(++m_impl->m_textSequenceNoUTF8, 11);
                sequenceNumber = m_impl->m_textSequenceNoUTF8;
                if (m_impl->m_textSequenceNoUTF8 >= 1999)
                {
                    m_impl->m_textSequenceNoUTF8 = 0;
                }
                // Spare2
                aisBitsManager.Encode(0, 2);
                // Text string
                for (auto &character : content)
                {
                    aisBitsManager.Encode(character, 8);
                }
                // Deadline
                aisBitsManager.Encode(static_cast<uint32_t>(deadline), 32);
                // Authentication
                aisBitsManager.Encode(0, 512);
            }
            break;
        case Impl::MessageBusinessType::FOURTEEN_BIT:
            {
                // Meesage No.
                aisBitsManager.Encode(11, 16);
                // Response indicator
                aisBitsManager.Encode((needReply && mmsiDest != 0) ? 1 : 0, 1);
                // Spare1
                aisBitsManager.Encode(0, 2);
                // Text sequence number
                aisBitsManager.Encode(++m_impl->m_textSequenceNoFourteenBit, 11);
                sequenceNumber = m_impl->m_textSequenceNoFourteenBit;
                if (m_impl->m_textSequenceNoFourteenBit >= 1999)
                {
                    m_impl->m_textSequenceNoFourteenBit = 0;
                }
                // Spare2
                aisBitsManager.Encode(0, 2);
                // Text string
                auto contentGBK = UtilityInterface::UTF8ToGBK(content);
                aisBitsManager.Encode(contentGBK, 413, 1);
                auto fillBitsNum = 8 - ((512 + 32 + aisBitsManager.GetBitsNumberToDecode()) % 8);
                aisBitsManager.Encode(0, fillBitsNum);
                // Deadline
                aisBitsManager.Encode(static_cast<uint32_t>(deadline), 32);
                // Authentication
                aisBitsManager.Encode(0, 512);
            }
            break;
        default:
            break;
        }
        auto payload = aisBitsManager.GetEncodedVDMPayload();
        auto fillBitNum = aisBitsManager.GetFillBitsNumberToEncode();
        auto strTDB = fmt::format("$XNTDB,1,1,0,,{},0,0,17,{},{}",
            (mmsiDest == 0) ? "" : fmt::format("{:09d}", mmsiDest),
            payload, fillBitNum);
        UtilityInterface::AddChecksum(strTDB);
        
        Message message;
        message.mmsiSource = mmsiSource;
        message.mmsiDestination = mmsiDest;
        message.content = content;
        message.dataID = 0;
        message.needAck = needReply;
        message.replyStatus = ReplyStatus::UN_REPLIED;
        message.deadline = deadline;
        message.timestamp = UtilityInterface::GetCurrentTimeStamp();
        
        std::unique_lock<std::mutex> lockGuard(m_impl->m_mutexMessageOutbox);
        auto dataID = m_impl->SaveMessage(MailBox::OUTBOX, message, messageType, sequenceNumber);
        lockGuard.unlock();
        m_impl->m_dataIDLastSentMessage = dataID;
        m_impl->m_parent->sendEvent(CommunicationType::TCP, strTDB.c_str(), strTDB.length());
       
        if (!TimerManager::IsTimerValid(m_impl->m_timerSendMessage))
        {
            auto &timerManager = TimerManager::GetInstance();
           
            m_impl->m_timerSendMessage = timerManager.AddTimer(20000, false, [&] {
                // Change the sending status
                m_impl->UpdateSentBusinessStatus(SendStatus::TIMEOUT);
                m_impl->m_timerSendMessage = 0;
                m_impl->m_parent->notifyEvent(EventType::MESSAGE_SEND, -1);
            });
        }
        return true;
    }

    void VDESManager::ConfirmMessage(const uint32_t dataID, const bool isMessageValid)
    {
        if (!m_impl->m_database)
        {
            return;
        }

        try
        {
            std::lock_guard<std::mutex> lockGuard(m_impl->m_mutexMessageInbox);  

            auto sql = fmt::format("SELECT Source, Type, [Text Sequence Number] FROM MessageInbox "
                                   "WHERE ID = {}", dataID);
            SQLite::Statement query(*m_impl->m_database.get(), sql);
            if (query.executeStep())
            { 
                auto source = query.getColumn(0).getInt();
                auto type = static_cast<Impl::MessageBusinessType>(query.getColumn(1).getInt());
                auto sequenceNumber = query.getColumn(2).getInt();

                AISBitsManager aisBitsManager;
                // VPFI
                aisBitsManager.Encode(2, 16);
                // Message ID
                aisBitsManager.Encode(2, 16);
                // Message ID replied
                uint16_t messageIDReplied = 0;
                switch (type)
                {   
                case Impl::MessageBusinessType::SIX_BIT_ASCII:
                    messageIDReplied = 0;
                    m_impl->m_transmissionBusiness = Impl::TransmissionBusiness::SIX_BIT_ASCII;
                    break;
                case Impl::MessageBusinessType::UTF8:
                    messageIDReplied = 1;
                    m_impl->m_transmissionBusiness = Impl::TransmissionBusiness::UTF8;
                    break;
                case Impl::MessageBusinessType::FOURTEEN_BIT:
                    messageIDReplied = 11;
                    m_impl->m_transmissionBusiness = Impl::TransmissionBusiness::FOURTEEN_BIT;
                    break;
                default:
                    // In theory, it should not be executed.
                    return;
                }
                aisBitsManager.Encode(messageIDReplied, 16);
                // Spare1
                aisBitsManager.Encode(0, 2);
                // Text sequence number
                aisBitsManager.Encode(sequenceNumber, 11);
                // Message ID confirmed
                aisBitsManager.Encode(isMessageValid ? 1 : 0, 1);
                // Spare2
                aisBitsManager.Encode(0, 2);
                // Authentication
                aisBitsManager.Encode(0, 512);

                auto payload = aisBitsManager.GetEncodedVDMPayload();
                auto fillBitNum = aisBitsManager.GetFillBitsNumberToEncode();
                auto strTDB = fmt::format("$XNTDB,1,1,0,,{},0,0,17,{},{}", source, payload, fillBitNum);
                UtilityInterface::AddChecksum(strTDB);
                m_impl->m_parent->sendEvent(CommunicationType::TCP, strTDB.c_str(), strTDB.length());

                m_impl->m_replyStatus = (isMessageValid) ? ReplyStatus::VALID : ReplyStatus::INVALID;
                m_impl->m_businessIDToBeConfirmed = dataID;

                if (!TimerManager::IsTimerValid(m_impl->m_timerSendMessage))
                {
                    auto &timerManager = TimerManager::GetInstance();

                    m_impl->m_timerSendMessage = timerManager.AddTimer(20000, false, [&] {
                        m_impl->m_businessIDToBeConfirmed = 0;
                        m_impl->m_timerSendMessage = 0;
                        m_impl->m_transmissionBusiness = Impl::TransmissionBusiness::NONE;
                        m_impl->m_parent->notifyEvent(EventType::MESSAGE_SEND, -1);
                    });
                }
            }
        }
        catch (const SQLite::Exception &execption)
        {
            m_impl->DatabaseErrorProcess(execption, "ConfirmMessage");
        }
    }

    void VDESManager::SendAISMessage(const AISMessageType type, const uint32_t mmsiDestination,
                                     const std::string &content)
    {
        if (!m_impl->m_database)
        {
            return;
        }
        try
        {
            auto contentGBK = UtilityInterface::UTF8ToGBK(content);

            std::transform(contentGBK.begin(), contentGBK.end(), contentGBK.begin(), ::toupper);
        
            switch (type)
            {
            case AISMessageType::MESSAGE_6:
                m_impl->SendAISMessage6(mmsiDestination, contentGBK);
                break;
            case AISMessageType::MESSAGE_8:
                m_impl->SendAISMessage8(contentGBK);
                break;
            case AISMessageType::MESSAGE_12:
                m_impl->SendAISMessage12(mmsiDestination, contentGBK);
                break;
            case AISMessageType::MESSAGE_14:
                m_impl->SendAISMessage14(contentGBK);
                break;
            default:
                break;
            }

            Message message;
            auto    ownShipInfo = ConfigureManager::GetInstance().GetOwnVesselInfo();

            message.mmsiSource = ownShipInfo.mmsi;
            message.mmsiDestination = mmsiDestination;
            message.content = contentGBK;
            message.messageType = MessageType::AIS;

            std::unique_lock<std::mutex> lockGuard(m_impl->m_mutexMessageOutbox);
            message.dataID = m_impl->SaveMessage(MailBox::OUTBOX, message, Impl::MessageBusinessType::NONE, 0);
            lockGuard.unlock();

            m_impl->m_dataIDLastSentAISMessage = message.dataID;
            auto &timerManager = TimerManager::GetInstance();
            m_impl->m_timerSendAISMessage = timerManager.AddTimer(10000, false, [&] {
                auto sql = fmt::format("UPDATE MessageOutbox SET [Send Status] = {} WHERE ID = {}", 
                                        static_cast<int>(SendStatus::TIMEOUT), message.dataID);
                m_impl->m_database->exec(sql);
                m_impl->m_timerSendAISMessage = 0;
                m_impl->m_dataIDLastSentAISMessage = 0;
                m_impl->m_parent->notifyEvent(EventType::MESSAGE_SEND, -1);
            });
        }
        catch (const SQLite::Exception &execption)
        {
            m_impl->DatabaseErrorProcess(execption, "SendAISMessage");
        }
    }

    VDESManager::AtoNs VDESManager::GetAtoNs(const uint32_t index, const size_t number)
    {
        std::vector<AtoN> atons;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM AtoNs ORDER BY Timestamp DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAtoN);
    
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
    
                while (query.executeStep())
                {
                    AtoN aton;
                    m_impl->LoadAtoNFromQueryResult(aton, query);
                    atons.push_back(aton);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetAtoNs");
            }
        }
        return atons;
    }

    VDESManager::AtoNs VDESManager::GetAtoNs(const BoundingBox &bbox, const size_t number)
    {
        std::vector<AtoN> atons;

        if (m_impl->m_database)
        {
            auto sqlCmd = m_impl->BuildSQLCmd("AtoNs", "AtoNBBox", bbox);
            if (number != -1)
            {
                sqlCmd.append(fmt::format(" LIMIT {}", number));
            }

            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAtoN);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    AtoN aton;
                    m_impl->LoadAtoNFromQueryResult(aton, query);
                    atons.push_back(aton);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetAtoNWithRange");
            }
        }
        return atons;
    }

    VDESManager::AtoNPtr VDESManager::GetAtoN(const double latitude, const double longitude, 
        const double radius)
    {
        if (m_impl->m_database)
        {
            std::lock_guard<std::mutex> lock(m_impl->m_mutexAtoN);

            auto bbox = BoundingBox::Build(latitude, longitude, radius);
            auto sqlCmd = m_impl->BuildSQLCmd("AtoNs", "AtoNBBox", bbox);
            SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
            
            try
            {
                AtoN    atonNearest;
                double  distanceNearest = 0.0F;

                while (query.executeStep())
                {
                    AtoN aton;

                    m_impl->LoadAtoNFromQueryResult(aton, query);
                    auto distance = GeoCalculator::Distance(Coordinate(latitude, longitude), aton.coordinate);
                    if (distance <= radius)
                    {
                        if (atonNearest.mmsi == 0)
                        {
                            atonNearest = aton;
                            distanceNearest = distance;
                        }
                        else
                        {
                            if (distance <= distanceNearest)
                            {
                                distanceNearest = distance;
                                atonNearest = aton;
                            }
                        }
                    }
                }
                if (atonNearest.mmsi != 0)
                {
                    return std::make_shared<AtoN>(atonNearest);
                } 
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetAtoNWithRange");
            }
        }
        return nullptr;
    }

    VDESManager::AtoNPtr VDESManager::GetAtoN(const uint32_t mmsi)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAtoN);

                auto sql = fmt::format("SELECT * FROM AtoNs WHERE ID = {}", mmsi);
                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    AtoN aton;
                    m_impl->LoadAtoNFromQueryResult(aton, query);
                    return std::make_shared<AtoN>(aton);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetAtoN");
            }
        }
        return nullptr;
    }

    bool VDESManager::DeleteAtoNs(const std::vector<uint32_t> &mmsiSet)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAtoN);

                auto sql = fmt::format("DELETE FROM AtoNs WHERE ID IN ({})", fmt::join(mmsiSet, ", "));
                m_impl->m_database->exec(sql);
                sql = fmt::format("DELETE FROM AtoNBBox WHERE ID = {}", fmt::join(mmsiSet, ", "));
                m_impl->m_database->exec(sql);
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteAtoNs");
            }
        }
        return false;
    }

    VDESManager::Obstacles VDESManager::GetObstacles(const uint32_t index, const size_t number)
    {
        Obstacles obstacles;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM MSIObstacle ORDER BY [Timestamp Receive] DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIObstacle);
    
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
    
                while (query.executeStep())
                {
                    MSIObstacle obstacle;
                    m_impl->LoadMSIObstacleFromQueryResult(obstacle, query);
                    obstacles.push_back(obstacle);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetAtoNs");
            }
        }
        return obstacles;
    }

    VDESManager::Obstacles VDESManager::GetObstacles(const BoundingBox &bbox, const size_t number)
    {
        Obstacles obstacles;
        
        if (m_impl->m_database)
        {
            auto sqlCmd = m_impl->BuildSQLCmd("MSIObstacle", "MSIObstacleBBox", bbox);
            if (number != -1)
            {
                sqlCmd.append(fmt::format(" LIMIT {}", number));
            }

            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIObstacle);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MSIObstacle obstacle;
                    m_impl->LoadMSIObstacleFromQueryResult(obstacle, query);
                    obstacles.push_back(obstacle);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetObstaclesInBBox");
            }
        }
        return obstacles;
    }

    VDESManager::ObstaclePtr VDESManager::GetObstacle(const double latitude, const double longitude, 
        double radius)
    {
        if (m_impl->m_database)
        {
            std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIObstacle);

            auto bbox = BoundingBox::Build(latitude, longitude, radius);
            auto sqlCmd = m_impl->BuildSQLCmd("MSIObstacle", "MSIObstacleBBox", bbox);
            SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

            try
            {
                MSIObstacle obstacleNearest;
                double      distanceNearest = -1.0F;

                while (query.executeStep())
                {
                    MSIObstacle obstacle;

                    m_impl->LoadMSIObstacleFromQueryResult(obstacle, query);
                    auto distance = GeoCalculator::Distance(Coordinate(latitude, longitude), 
                                                            obstacle.coordinate);
                    if (distance <= radius)
                    {
                        if (distanceNearest < 0)
                        {
                            obstacleNearest = obstacle;
                            distanceNearest = distance;
                        }
                        else
                        {
                            if (distance <= distanceNearest)
                            {
                                distanceNearest = distance;
                                obstacleNearest = obstacle;
                            }
                        }
                    }
                }
                if (distanceNearest >= 0)
                {
                    return std::make_shared<MSIObstacle>(obstacleNearest);
                }

            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetObstacleWithRange");
            }
        }
        return nullptr;
    }

    VDESManager::ObstaclePtr VDESManager::GetObstacle(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIObstacle);

                auto sql = fmt::format("SELECT * FROM MSIObstacle WHERE ID = {}", dataID);
                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    MSIObstacle obstacle;
                    m_impl->LoadMSIObstacleFromQueryResult(obstacle, query);
                    return std::make_shared<MSIObstacle>(obstacle);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetObstacle");
            } 
        }
        return nullptr;
    }

    bool VDESManager::DeleteObstacle(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIObstacle);

                auto sql = fmt::format("DELETE FROM MSIObstacle WHERE ID = {}", dataID);
                m_impl->m_database->exec(sql);
                auto sqlBBox = fmt::format("DELETE FROM MSIObstacleBBox WHERE ID = {}", dataID);
                m_impl->m_database->exec(sqlBBox);
                transaction.commit();
                return true;
            }
            catch(const SQLite::Exception &exception)
            {
                m_impl->DatabaseErrorProcess(exception, "DeleteObstacle");
            }
        }  
        return false;
    }

    bool VDESManager::DeleteObstacles(const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIObstacle);

                auto sql = fmt::format("DELETE FROM MSIObstacle WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sql);
                auto sqlBBox = fmt::format("DELETE FROM MSIObstacleBBox WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlBBox);
                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteObstacles");
            }
        }
        return false;
    }

    VDESManager::MaritimeOperations VDESManager::GetMaritimeOperations(const uint32_t index, 
        const size_t number)
    {
        MaritimeOperations operations;
        
        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM MSIMaritimeOperation ORDER BY [Timestamp Receive] "
                             "DESC LIMIT {0} OFFSET {1}",
                             (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeOperation);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MSIMaritimeOperation operation;

                    m_impl->LoadMSIMaritineOperationFromQueryResult(operation, query);
                    operations.push_back(operation);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeOperations");
            }
        }
        return operations;
    }
    
    VDESManager::MaritimeOperations VDESManager::GetMaritimeOperations(const BoundingBox &bbox, 
        const size_t number)
    {
        MaritimeOperations operations;

        if (m_impl->m_database)
        {
            auto sqlCmd = m_impl->BuildSQLCmd("MSIMaritimeOperation", "MSIMaritimeOperationBBox", bbox);
            if (number != -1)
            {
                sqlCmd.append(fmt::format(" LIMIT {}", number));
            }

            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeOperation);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MSIMaritimeOperation operation;

                    m_impl->LoadMSIMaritineOperationFromQueryResult(operation, query);
                    operations.push_back(operation);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeOperationsInBBox");
            }
        }
        return operations;
    }

    VDESManager::MaritimeOperationPtr VDESManager::GetMaritimeOperation(const double latitude, 
        const double longitude, const double radius)
    {
        if (m_impl->m_database)
        {
            std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeOperation);

            auto bbox = BoundingBox::Build(latitude, longitude, radius);
            auto sqlCmd = m_impl->BuildSQLCmd("MSIMaritimeOperation", "MSIMaritimeOperationBBox", bbox);
            SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

            try
            {
                MSIMaritimeOperation operationNearest;
                MSIMaritimeOperation operation;
                bool                 found = false;
                double               distanceNearest = -1.0F;

                auto UpdateNearest = [&](MSIMaritimeOperation &operation, const double distance) {
                    if (distance <= radius)
                    {
                        if (distanceNearest < 0)
                        {
                            distanceNearest = distance;
                            operationNearest = operation;
                        }
                        else
                        {
                            if (distance <= distanceNearest)
                            {
                                distanceNearest = distance;
                                operationNearest = operation;
                            }
                        }
                    }
                    
                };

                while (query.executeStep())
                {
                    m_impl->LoadMSIMaritineOperationFromQueryResult(operation, query);

                    if (operation.coordinateSW.IsValid())
                    {
                        auto &NE = operation.coordinateNE;
                        auto &SW = operation.coordinateSW;
                        std::vector<Coordinate> polygon{
                            Coordinate(NE.GetLatitude(), SW.GetLongitude()),
                            NE,
                            Coordinate(SW.GetLatitude(), NE.GetLongitude()),
                            SW,
                            Coordinate(NE.GetLatitude(), SW.GetLongitude())
                        };
                        auto distance = GeoCalculator::GetDistanceToPolygon(Coordinate(latitude, longitude), polygon);
                        UpdateNearest(operation, distance);
                    }
                    else
                    {
                        auto distance = GeoCalculator::Distance(Coordinate(latitude, longitude), operation.coordinateNE);
                        UpdateNearest(operation, distance);
                    }
                    found = true;
                }
                if (distanceNearest >= 0)
                {
                    return std::make_shared<MSIMaritimeOperation>(operationNearest);
                }
                else if (found)
                {
                    return std::make_shared<MSIMaritimeOperation>(operation);
                }

            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeOperationWithRange");
            }
        }
        return nullptr;
    }

    VDESManager::MaritimeOperationPtr VDESManager::GetMaritimeOperation(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeOperation);

                auto sql = fmt::format("SELECT * FROM MSIMaritimeOperation WHERE ID = {}", dataID);
                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    MSIMaritimeOperation operation;
                    m_impl->LoadMSIMaritineOperationFromQueryResult(operation, query);
                    return std::make_shared<MSIMaritimeOperation>(operation);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeOperation");
            }
        }
        return nullptr;
    }

    bool VDESManager::DeleteMaritimeOperation(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeOperation);

                auto sql = fmt::format("DELETE FROM MSIMaritimeOperation WHERE ID = {}", dataID);
                m_impl->m_database->exec(sql);

                std::vector<int64_t> dataIDs{ dataID , -(int64_t)dataID};
                auto sqlBBox = fmt::format("DELETE FROM MSIMaritimeOperationBBox WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlBBox);
                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMaritimeOperation");
            }   
        }
        return false;
    }

    bool VDESManager::DeleteMaritimeOperations(const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeOperation);

                auto sql = fmt::format("DELETE FROM MSIMaritimeOperation WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sql);

                std::vector<int64_t> dataIDsToDelete;
                dataIDsToDelete.reserve(dataIDs.size() * 2); 

                std::transform(dataIDs.begin(), dataIDs.end(), std::back_inserter(dataIDsToDelete),
                [](uint32_t id) { return static_cast<int64_t>(id); }); 

                std::transform(dataIDs.begin(), dataIDs.end(), std::back_inserter(dataIDsToDelete),
                [](uint32_t id) { return -static_cast<int64_t>(id); });

                auto sqlBBox = fmt::format("DELETE FROM MSIMaritimeOperationBBox WHERE ID IN ({})", fmt::join(dataIDsToDelete, ", "));
                m_impl->m_database->exec(sqlBBox);
                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMaritimeOperations");
            }
        }
        return false;
    }

    VDESManager::MaritimeTowings VDESManager::GetMaritimeTowings(const uint32_t index, 
        const size_t number)
    {
        MaritimeTowings towings;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM MSIMaritimeTowing ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeTowing);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MSIMaritimeTowing towing;

                    m_impl->LoadMSIMaritimeTowingFromQueryResult(towing, query);
                    towings.push_back(towing);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeTowings");
            }
        }
        return towings;
    }

    VDESManager::MaritimeTowings VDESManager::GetMaritimeTowings(const BoundingBox &bbox, 
        const size_t number)
    {
        MaritimeTowings towings;

        if (m_impl->m_database)
        {
            auto sqlCmd = m_impl->BuildSQLCmd("MSIMaritimeTowing", "MSIMaritimeTowingBBox", bbox);
            if (number != -1)
            {
                sqlCmd.append(fmt::format(" LIMIT {}", number));
            }

            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeTowing);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MSIMaritimeTowing towing;

                    m_impl->LoadMSIMaritimeTowingFromQueryResult(towing, query);
                    towings.push_back(towing);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeTowingsInBBox");
            }
        }
        return towings;
    }

    VDESManager::MaritimeTowingPtr VDESManager::GetMaritimeTowing(const double latitude, 
        const double longitude, const double radius)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeTowing);

                auto bbox = BoundingBox::Build(latitude, longitude, radius);
                auto sqlCmd = m_impl->BuildSQLCmd("MSIMaritimeTowing", "MSIMaritimeTowingBBox", bbox);
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                MSIMaritimeTowing  towingNearest;
                double             distanceNearest = -1.0F;

                auto UpdateNearest = [&](MSIMaritimeTowing &towing, const double distance) {
                    if (distance <= radius)
                    {
                        if (distanceNearest < 0)
                        {
                            towingNearest = towing;
                            distanceNearest = distance;
                        }
                        else
                        {
                            if (distance <= distanceNearest)
                            {
                                towingNearest = towing;
                                distanceNearest = distance;
                            }
                        }
                    }
                };

                while (query.executeStep())
                {
                    MSIMaritimeTowing towing;
                    m_impl->LoadMSIMaritimeTowingFromQueryResult(towing, query);
                    auto distance = GeoCalculator::GetDistanceToLineSegment(Coordinate(latitude, longitude),
                        towing.coordinateStart, towing.coordinateEnd);
                    UpdateNearest(towing, distance);
                }

                if (distanceNearest >= 0)
                {
                    return std::make_shared<MSIMaritimeTowing>(towingNearest);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeTowingWithRange");
            }
        }
        return nullptr;
    }

    VDESManager::MaritimeTowingPtr VDESManager::GetMaritimeTowing(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeTowing);

                auto sql = fmt::format("SELECT * FROM MSIMaritimeTowing WHERE ID = {}", dataID);
                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    MSIMaritimeTowing towing;
                    m_impl->LoadMSIMaritimeTowingFromQueryResult(towing, query);
                    return std::make_shared<MSIMaritimeTowing>(towing);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeTowing");
            }
        }
        return nullptr;
    }

    bool VDESManager::DeleteMaritimeTowing(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeTowing);

                auto sql = fmt::format("DELETE FROM MSIMaritimeTowing WHERE ID = {}", dataID);
                m_impl->m_database->exec(sql);

                std::vector<int64_t> dataIDs{ dataID , -(int64_t)dataID};
                auto sqlBBox = fmt::format("DELETE FROM MSIMaritimeTowingBBox WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlBBox);
                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMaritimeTowing");
            }   
        }
        return false;
    }

    bool VDESManager::DeleteMaritimeTowings(const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeTowing);

                auto sql = fmt::format("DELETE FROM MSIMaritimeTowing WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sql);

                std::vector<int64_t> dataIDsToDelete;
                dataIDsToDelete.reserve(dataIDs.size() * 2); 

                std::transform(dataIDs.begin(), dataIDs.end(), std::back_inserter(dataIDsToDelete),
                [](uint32_t id) { return static_cast<int64_t>(id); }); 

                std::transform(dataIDs.begin(), dataIDs.end(), std::back_inserter(dataIDsToDelete),
                [](uint32_t id) { return -static_cast<int64_t>(id); });

                auto sqlBBox = fmt::format("DELETE FROM MSIMaritimeTowingBBox WHERE ID IN ({})", fmt::join(dataIDsToDelete, ", "));
                m_impl->m_database->exec(sqlBBox);
                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMaritimeTowings");
            }
        }
        return false;
    }

    VDESManager::MilitaryActivitys VDESManager::GetMilitaryActivitys(const uint32_t index, 
        const size_t number)
    {
        MilitaryActivitys activities;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM MSIMilitaryActivity ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMilitaryActivity);
    
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
    
                while (query.executeStep())
                {
                    MSIMilitaryActivity activity;
    
                    m_impl->LoadMSIMilitaryActivityFromQueryResult(activity, query);
                    activities.push_back(activity);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMilitaryActivitys");
            }
        }
        return activities;
    }

    VDESManager::MilitaryActivitys VDESManager::GetMilitaryActivitys(const BoundingBox &bbox, 
        const size_t number)
    {
        MilitaryActivitys activities;

        if (m_impl->m_database)
        {
            auto sqlCmd = m_impl->BuildSQLCmd("MSIMilitaryActivity", "MSIMilitaryActivityBBox", bbox);
            if (number != -1)
            {
                sqlCmd.append(fmt::format(" LIMIT {}", number));
            }

            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMilitaryActivity);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MSIMilitaryActivity activity;

                    m_impl->LoadMSIMilitaryActivityFromQueryResult(activity, query);
                    activities.push_back(activity);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMilitaryActivitysInBBox");
            }
        }
        return activities;
    }

    VDESManager::MilitaryActivityPtr VDESManager::GetMilitaryActivity(const double latitude, 
        const double longitude, const double radius)
    {
        if (m_impl->m_database)
        {
            std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMilitaryActivity);

            auto bbox = BoundingBox::Build(latitude, longitude, radius);
            auto sqlCmd = m_impl->BuildSQLCmd("MSIMilitaryActivity", "MSIMilitaryActivityBBox", bbox);
            SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

            try
            {
                MSIMilitaryActivity  activityNearest;
                MSIMilitaryActivity  activity;
                bool                 found = false;
                double               distanceNearest = -1.0F;

                auto UpdateNearest = [&](MSIMilitaryActivity &activity, const double distance) {
                    if (distance <= radius)
                    {
                        if (distanceNearest < 0)
                        {
                            distanceNearest = distance;
                            activityNearest = activity;
                        }
                        else
                        {
                            if (distance <= distanceNearest)
                            {
                                distanceNearest = distance;
                                activityNearest = activity;
                            }
                        }
                    }
                };
                while (query.executeStep())
                {
                    m_impl->LoadMSIMilitaryActivityFromQueryResult(activity, query);

                    double distance = -1.0;
                    if (activity.geometryType == 0 || activity.geometryType == 1)
                    {
                        distance = GeoCalculator::Distance(Coordinate(latitude, longitude), activity.coordinateNE);
                    }
                    else if (activity.geometryType == 2 && activity.polygon.size() >= 3)
                    {
                        std::vector<Coordinate> polygonTemp = activity.polygon;
                        polygonTemp.push_back(activity.polygon.front());
                        distance = GeoCalculator::GetDistanceToPolygon(Coordinate(latitude, longitude), polygonTemp);
                    }
                    else if (!activity.polygon.empty())
                    {
                        if (activity.polygon.size() == 1)
                        {
                            distance = GeoCalculator::Distance(Coordinate(latitude, longitude), activity.polygon[0]);
                        }
                        else
                        {
                            double minSegDist = -1.0;
                            for (size_t i = 0; i < activity.polygon.size() - 1; ++i)
                            {
                                double segDist = GeoCalculator::GetDistanceToLineSegment(
                                    Coordinate(latitude, longitude), 
                                    activity.polygon[i], 
                                    activity.polygon[i + 1]
                                );
                                if (minSegDist < 0 || segDist < minSegDist)
                                {
                                    minSegDist = segDist;
                                }
                            }
                            distance = minSegDist;
                        }
                    }

                    if (distance >= 0)
                    {
                        UpdateNearest(activity, distance);
                    }
                    found = true;
                }
                if (distanceNearest >= 0)
                {
                    return std::make_shared<MSIMilitaryActivity>(activityNearest);
                }
                else if (found)
                {
                    return std::make_shared<MSIMilitaryActivity>(activity);
                }

            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMilitaryActivityWithRange");
            }
        }
        return nullptr;
    }

    VDESManager::MilitaryActivityPtr VDESManager::GetMilitaryActivity(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMilitaryActivity);

                auto sql = fmt::format("SELECT * FROM MSIMilitaryActivity WHERE ID = {}", dataID);
                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    MSIMilitaryActivity activity;
                    m_impl->LoadMSIMilitaryActivityFromQueryResult(activity, query);
                    return std::make_shared<MSIMilitaryActivity>(activity);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMilitaryActivity");
            }
        }
        return nullptr;
    }

    bool VDESManager::DeleteMilitaryActivity(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMilitaryActivity);

                auto sql = fmt::format("DELETE FROM MSIMilitaryActivity WHERE ID = {}", dataID);
                m_impl->m_database->exec(sql);

                std::vector<int64_t> dataIDs{ dataID, -static_cast<int64_t>(dataID)};
                auto sqlBBox = fmt::format("DELETE FROM MSIMilitaryActivityBBox WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlBBox);
                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMilitaryActivity");
            }
        }
        return false;
    }

    bool VDESManager::DeleteMilitaryActivitys(const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMilitaryActivity);

                auto sql = fmt::format("DELETE FROM MSIMilitaryActivity WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sql);

                std::vector<int64_t> dataIDsToDelete;
                dataIDsToDelete.reserve(dataIDs.size() * 2);

                std::transform(dataIDs.begin(), dataIDs.end(), std::back_inserter(dataIDsToDelete),
                    [](uint32_t id) { return static_cast<int64_t>(id); });

                std::transform(dataIDs.begin(), dataIDs.end(), std::back_inserter(dataIDsToDelete),
                    [](uint32_t id) { return -static_cast<int64_t>(id); });

                auto sqlBBox = fmt::format("DELETE FROM MSIMilitaryActivityBBox WHERE ID IN ({})", fmt::join(dataIDsToDelete, ", "));
                m_impl->m_database->exec(sqlBBox);
                transaction.commit();

                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMilitaryActivitys");
            }
        }
        return false;
    }

    VDESManager::MaritimeDistresses VDESManager::GetMaritimeDistresses(const uint32_t index, const size_t number)
    {
        MaritimeDistresses container;
        
        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM MSIMaritimeDistress ORDER BY [Timestamp Receive] "
                             "DESC LIMIT {0} OFFSET {1}",
                             (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeDistress);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MSIMaritimeDistress distress;

                    m_impl->LoadMSIMaritimeDistressFromQueryResult(distress, query);
                    container.push_back(distress);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeDistress");
            }
        }
        return container;
    }
    
    VDESManager::MaritimeDistresses VDESManager::GetMaritimeDistresses(const BoundingBox &bbox,
        const size_t number)
    {
        MaritimeDistresses container;

        if (m_impl->m_database)
        {
            auto sqlCmd = m_impl->BuildSQLCmd("MSIMaritimeDistress", "MSIMaritimeDistressBBox", bbox);
            if (number != -1)
            {
                sqlCmd.append(fmt::format(" LIMIT {}", number));
            }

            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeDistress);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MSIMaritimeDistress distress;

                    m_impl->LoadMSIMaritimeDistressFromQueryResult(distress, query);
                    container.push_back(distress);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeDistressInBBox");
            }
        }
        return container;
    }

    VDESManager::MaritimeDistressPtr VDESManager::GetMaritimeDistress(const double latitude, 
        const double longitude, const double radius)
    {
        if (m_impl->m_database)
        {
            std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeDistress);

            auto bbox = BoundingBox::Build(latitude, longitude, radius);
            auto sqlCmd = m_impl->BuildSQLCmd("MSIMaritimeDistress", "MSIMaritimeDistressBBox", bbox);
            SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

            try
            {
                MSIMaritimeDistress  distressNearest;
                double               distanceNearest = -1.0F;

                auto UpdateNearest = [&](MSIMaritimeDistress &distress, const double distance) {
                    if (distance <= radius)
                    {
                        if (distanceNearest < 0)
                        {
                            distanceNearest = distance;
                            distressNearest = distress;
                        }
                        else
                        {
                            if (distance <= distanceNearest)
                            {
                                distanceNearest = distance;
                                distressNearest = distress;
                            }
                        }
                    }
                    };
                while (query.executeStep())
                {
                    MSIMaritimeDistress distress;

                    m_impl->LoadMSIMaritimeDistressFromQueryResult(distress, query);

                    UpdateNearest(distress, GeoCalculator::Distance(distress.coordinate, Coordinate(latitude, longitude)));
                }
                if (distanceNearest >= 0)
                {
                    return std::make_shared<MSIMaritimeDistress>(distressNearest);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeDistressWithRange");
            }
        }
        return nullptr;
    }

    VDESManager::MaritimeDistressPtr VDESManager::GetMaritimeDistress(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeDistress);

                auto sql = fmt::format("SELECT * FROM MSIMaritimeDistress WHERE ID = {}", dataID);
                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    MSIMaritimeDistress distress;
                    m_impl->LoadMSIMaritimeDistressFromQueryResult(distress, query);
                    return std::make_shared<MSIMaritimeDistress>(distress);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMaritimeDistress");
            }
        }
        return nullptr;
    }

    bool VDESManager::DeleteMaritimeDistress(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeDistress);

                auto sql = fmt::format("DELETE FROM MSIMaritimeDistress WHERE ID = {}", dataID);
                m_impl->m_database->exec(sql);

                auto sqlBBox = fmt::format("DELETE FROM MSIMaritimeDistressBBox WHERE ID = {}", dataID);
                m_impl->m_database->exec(sqlBBox);

                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMaritimeDistress");
            }
        }
        return false;
    }

    bool VDESManager::DeleteMaritimeDistresses(const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIMaritimeDistress);

                auto sql = fmt::format("DELETE FROM MSIMaritimeDistress WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sql);

                auto sqlBBox = fmt::format("DELETE FROM MSIMaritimeDistressBBox WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlBBox);

                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMaritimeDistresses");
            }
        }
        return false;
    }

    VDESManager::DesignatedAreas VDESManager::GetDesignatedAreas(const uint32_t index, const size_t number)
    {
        DesignatedAreas container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM MSIDesignatedArea ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIDesignatedArea);
    
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
    
                while (query.executeStep())
                {
                    MSIDesignatedArea area;
    
                    m_impl->LoadMSIDesignatedAreaFromQueryResult(area, query);
                    container.push_back(area);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetDesignatedAreas");
            }
        }
        return container;
    }

    VDESManager::DesignatedAreas VDESManager::GetDesignatedAreas(const BoundingBox &bbox, const size_t number)
    {
        DesignatedAreas container;

        if (m_impl->m_database)
        {
            auto sqlCmd = m_impl->BuildSQLCmd("MSIDesignatedArea", "MSIDesignatedAreaBBox", bbox);
            if (number != -1)
            {
                sqlCmd.append(fmt::format(" LIMIT {}", number));
            }

            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIDesignatedArea);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MSIDesignatedArea area;

                    m_impl->LoadMSIDesignatedAreaFromQueryResult(area, query);
                    container.push_back(area);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetDesignatedAreasInBBox");
            }
        }
        return container;
    }

    VDESManager::DesignatedAreaPtr VDESManager::GetDesignatedArea(const double latitude, 
        const double longitude, const double radius)
    {
        if (m_impl->m_database)
        {
            std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIDesignatedArea);

            auto bbox = BoundingBox::Build(latitude, longitude, radius);
            auto sqlCmd = m_impl->BuildSQLCmd("MSIDesignatedArea", "MSIDesignatedAreaBBox", bbox);
            SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

            try
            {
                MSIDesignatedArea  areaNearest;
                MSIDesignatedArea  area;
                double             distanceNearest = -1.0F;

                auto UpdateNearest = [&](MSIDesignatedArea &area, const double distance) {
                    if (distance <= radius)
                    {
                        if (distanceNearest < 0)
                        {
                            distanceNearest = distance;
                            areaNearest = area;
                        }
                        else
                        {
                            if (distance <= distanceNearest)
                            {
                                distanceNearest = distance;
                                areaNearest = area;
                            }
                        }
                    }
                };
                while (query.executeStep())
                {
                    m_impl->LoadMSIDesignatedAreaFromQueryResult(area, query);

                    double distance = -1.0;
                    if (area.geometryType == 0 || area.geometryType == 1)
                    {
                        distance = GeoCalculator::Distance(Coordinate(latitude, longitude), area.coordinateNE);
                    }
                    else if (area.geometryType == 2 && area.polygon.size() >= 3)
                    {
                        std::vector<Coordinate> polygonTemp = area.polygon;
                        polygonTemp.push_back(area.polygon.front());
                        distance = GeoCalculator::GetDistanceToPolygon(Coordinate(latitude, longitude), polygonTemp);
                    }
                    else if (!area.polygon.empty())
                    {
                        if (area.polygon.size() == 1)
                        {
                            distance = GeoCalculator::Distance(Coordinate(latitude, longitude), area.polygon[0]);
                        }
                        else
                        {
                            double minSegDist = -1.0;
                            for (size_t i = 0; i < area.polygon.size() - 1; ++i)
                            {
                                double segDist = GeoCalculator::GetDistanceToLineSegment(
                                    Coordinate(latitude, longitude), 
                                    area.polygon[i], 
                                    area.polygon[i + 1]
                                );
                                if (minSegDist < 0 || segDist < minSegDist)
                                {
                                    minSegDist = segDist;
                                }
                            }
                            distance = minSegDist;
                        }
                    }
                    UpdateNearest(area, distance);
                }
                if (distanceNearest >= 0)
                {
                    return std::make_shared<MSIDesignatedArea>(areaNearest);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetDesignatedAreaWithRange");
            }
        }
        return nullptr;
    }

    VDESManager::DesignatedAreaPtr VDESManager::GetDesignatedArea(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIDesignatedArea);

                auto sql = fmt::format("SELECT * FROM MSIDesignatedArea WHERE ID = {}", dataID);
                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    MSIDesignatedArea area;
                    m_impl->LoadMSIDesignatedAreaFromQueryResult(area, query);
                    return std::make_shared<MSIDesignatedArea>(area);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetDesignatedArea");
            }
        }
        return nullptr;
    }

    bool VDESManager::DeleteDesignatedArea(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMSIDesignatedArea);

                auto sql = fmt::format("DELETE FROM MSIDesignatedArea WHERE ID = {}", dataID);
                auto sqlBBox = fmt::format("DELETE FROM MSIDesignatedAreaBBox WHERE id = {0} OR id = -{0}", dataID);

                m_impl->m_database->exec(sql);
                m_impl->m_database->exec(sqlBBox);

                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteDesignatedArea");
            }
        }
        return false;
    }

    VDESManager::MSIInfos VDESManager::GetMSIOverviews(const uint32_t index, const size_t number)
    {
        // FIXME
        MSIInfos container;

        auto obstacles = GetObstacles(0, -1);
        for (auto &obstacle : obstacles)
        {
            MSIInfo info;

            info.dataID = obstacle.dataID;
            info.subtype = obstacle.type;
            info.type = 0;
            info.timestampStart = (obstacle.timestampStart != 0) ? obstacle.timestampStart : obstacle.timestamp;
            info.timestampEnd = obstacle.timestampEnd;
            container.emplace_back(info);
        }

        auto operations = GetMaritimeOperations(0, -1);
        for (auto &operation : operations)
        {
            MSIInfo info;

            info.dataID = operation.dataID;
            info.subtype = operation.type;
            info.type = 1;
            info.timestampStart = (operation.timestampStart != 0) ? operation.timestampStart : operation.timestamp;
            info.timestampEnd = operation.timestampEnd;
            container.emplace_back(info);
        }

        auto activities = GetMilitaryActivitys(0, -1);
        for (auto &activity : activities)
        {
            MSIInfo info;

            info.dataID = activity.dataID;
            info.subtype = activity.theme;
            info.type = 2;
            info.timestampStart = (activity.timestampStart != 0) ? activity.timestampStart : activity.timestamp;
            info.timestampEnd = activity.timestampEnd;
            container.emplace_back(info);
        }

        auto distresses = GetMaritimeDistresses(0, -1);
        for (auto &distress : distresses)
        {
            MSIInfo info;
            info.dataID = distress.dataID;
            info.subtype = distress.type;
            info.type = 3;
            info.timestampStart = distress.timestamp;
            info.timestampEnd = 0;
            container.emplace_back(info);
        }

        auto designatedAreas = GetDesignatedAreas(0, -1);
        for (auto &area : designatedAreas)
        {
            MSIInfo info;
            info.dataID = area.dataID;
            info.subtype = area.areaType;
            info.type = 4;
            info.timestampStart = (area.timestampStart != 0) ? area.timestampStart : area.timestamp;
            info.timestampEnd = area.timestampEnd;
            container.emplace_back(info);
        }

        std::sort(container.begin(), container.end(), [](const MSIInfo &a, const MSIInfo &b) {
            return a.timestampStart > b.timestampStart;
        });

        return std::vector<MSIInfo>(container.begin(), container.begin() + std::min(number, container.size()));
    }

    VDESManager::Hydrometeorologies VDESManager::GetHydrometeorology(const uint32_t index, const size_t number)
    {
        Hydrometeorologies container;
     
        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM Hydrometeorology ORDER BY [Timestamp Forecast] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexHydrometeorology);
    
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
    
                while (query.executeStep())
                {
                    Hydrometeorology hydro;
    
                    m_impl->LoadHydrometeorologyFromQueryResult(hydro, query);
                    container.push_back(hydro);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetHydrometeorology");
            }
        }
        return container;
    }

    VDESManager::Hydrometeorologies VDESManager::GetHydrometeorology(const uint32_t dataID, const uint64_t timestampStart, 
        const uint64_t timestampEnd)
    {
        Hydrometeorologies container;
      
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexHydrometeorology);

                auto sqlCmd = fmt::format("SELECT * FROM Hydrometeorology WHERE [Timestamp Forecast] >= {} AND "
                                          "[Timestamp Forecast] <= {}",
                                          timestampStart, timestampEnd);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    Hydrometeorology hydro;

                    m_impl->LoadHydrometeorologyFromQueryResult(hydro, query);
                    container.push_back(hydro);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetHydrometeorology");
            }
        }
        return container;
    }

    VDESManager::Hydrometeorologies VDESManager::GetHydrometeorology(const BoundingBox &bbox, 
        const size_t number)
    {
        Hydrometeorologies container;

        if (m_impl->m_database)
        {
            try
            {
                auto sqlCmd = m_impl->BuildSQLCmd("Hydrometeorology", "HydrometeorologyBBox", bbox);
                sqlCmd += " GROUP BY Latitude, Longitude";
                if (number != -1)
                {
                    sqlCmd.append(fmt::format(" LIMIT {}", number));
                }

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
                while (query.executeStep())
                {
                    Hydrometeorology hydro;
                    m_impl->LoadHydrometeorologyFromQueryResult(hydro, query);
                    container.emplace_back(hydro);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetHydrometeorology");
            }
        }
        return container;
    }

    VDESManager::Hydrometeorologies VDESManager::GetHydrometeorology(const double latitude, const double longitude,
        const double radius)
    {
        Hydrometeorologies container;

        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexHydrometeorology);

                auto bbox = BoundingBox::Build(latitude, longitude, radius);
                auto sqlCmd = m_impl->BuildSQLCmd("Hydrometeorology", "HydrometeorologyBBox", bbox);
                sqlCmd += " ORDER BY [Timestamp Forecast] DESC";
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    Hydrometeorology hydro;

                    m_impl->LoadHydrometeorologyFromQueryResult(hydro, query);
                    container.emplace_back(hydro); 
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetHydrometeorologyWithRange");
            }
        }
        return container;
    }

    bool VDESManager::DeleteHydrometeorology(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);   

                std::lock_guard<std::mutex> lock(m_impl->m_mutexHydrometeorology);

                auto sql = fmt::format("DELETE FROM Hydrometeorology WHERE ID = {}", dataID);
                m_impl->m_database->exec(sql);

                auto sqlBBox = fmt::format("DELETE FROM HydrometeorologyBBox WHERE ID = {}", dataID);
                m_impl->m_database->exec(sqlBBox);

                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteHydrometeorology");
            }
        }
        return false;
    }

    bool VDESManager::DeleteHydrometeorologies(const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexHydrometeorology);

                auto sql = fmt::format("DELETE FROM Hydrometeorology WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sql);

                auto sqlBBox = fmt::format("DELETE FROM HydrometeorologyBBox WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlBBox);

                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteHydrometeorologies");
            }
        }
        return false;
    }

    VDESManager::AbnormalShips VDESManager::GetAbnormalShips(const uint32_t index, const size_t number)
    {
        AbnormalShips container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM AbnormalShip ORDER BY [Timestamp Receive] "
                             "DESC LIMIT {0} OFFSET {1}",
                             (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAbnormalShip);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    AbnormalShip ship;
                    m_impl->LoadAbnormalShipFromQueryResult(ship, query);
                    container.emplace_back(ship);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetAbnormalShips");
            }
        }
        return container;
    }

    VDESManager::AbnormalShips VDESManager::GetAbnormalShipsByMMSI(const uint32_t mmsi)
    {
        AbnormalShips container;

        auto sqlCmd = fmt::format("SELECT * FROM AbnormalShip WHERE MMSI = {} ORDER BY [Timestamp Receive]", mmsi);
        
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAbnormalShip);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    AbnormalShip ship;
                    m_impl->LoadAbnormalShipFromQueryResult(ship, query);
                    container.emplace_back(ship);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetAbnormalShips");
            }
        }
        return container;
    }

    bool VDESManager::IsAbnormalShip(const uint32_t mmsi)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAbnormalShip);

                auto sql = fmt::format("SELECT ID FROM AbnormalShip WHERE MMSI = {}", mmsi);

                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    return true;
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "IsAbnormalShip");
            } 
        }
        return false;
    }

    VDESManager::AbnormalShipPtr VDESManager::GetAbnormalShip(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAbnormalShip);

                auto sql = fmt::format("SELECT * FROM AbnormalShip WHERE ID = {}", dataID);

                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    AbnormalShip ship;
                    m_impl->LoadAbnormalShipFromQueryResult(ship, query);
                    return std::make_shared<AbnormalShip>(ship);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetAbnormalShip");
            }
        }
        return nullptr;
    }

    bool VDESManager::DeleteAbnormalShip(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexAbnormalShip);

                auto sql = fmt::format("DELETE FROM AbnormalShip WHERE ID = {}", dataID);
                m_impl->m_database->exec(sql);

                auto sqlBBox = fmt::format("DELETE FROM CoordinateInfo WHERE ID = {}", dataID);
                m_impl->m_database->exec(sqlBBox);

                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteAbnormalShip");
            }
        }
        return false;
    }

    bool VDESManager::DeleteAbnormalShips(const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                SQLite::Transaction transaction(*m_impl->m_database);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexAbnormalShip);

                auto sql = fmt::format("DELETE FROM AbnormalShip WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sql);

                auto sqlBBox = fmt::format("DELETE FROM CoordinateInfo WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlBBox);

                transaction.commit();
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteAbnormalShips");
            }
        }
        return false;
    }

    void VDESManager::SendRequestToVTS(const uint32_t mmsiVTS, const uint8_t requestType)
    {
        AISBitsManager aisBitsManager;

        // VPFI
        aisBitsManager.Encode(2, 16);
        // Message No.
        aisBitsManager.Encode(7, 16);
        // Text message number
        aisBitsManager.Encode(++m_impl->m_textSequenceNoVTS, 11);
        if (m_impl->m_textSequenceNoVTS >= 1999)
        {
            m_impl->m_textSequenceNoVTS = 0;
        }
        // Message type
        aisBitsManager.Encode(0, 2);
        // Request type
        aisBitsManager.Encode(requestType, 4);
        // Spare
        aisBitsManager.Encode(0, 7);
        // Authentication
        aisBitsManager.Encode(0, 512);

        auto payload = aisBitsManager.GetEncodedVDMPayload();
        auto fillBitNum = aisBitsManager.GetFillBitsNumberToEncode();
        auto strTDB = fmt::format("$XNTDB,1,1,0,,{},0,0,17,{},{}", mmsiVTS, payload, fillBitNum);
        UtilityInterface::AddChecksum(strTDB);
        sendEvent(CommunicationType::TCP, strTDB.c_str(), strTDB.size());

        VTSRequest request;
        request.dataID = 0;
        request.requestType = requestType;
        request.timestamp = UtilityInterface::GetCurrentTimeStamp();
        request.replyResult = -1;
        request.timestampReply = 0;
        request.executingStatus = -1; 

        std::unique_lock<std::mutex> lock(m_impl->m_mutexVTSRequest);
        m_impl->SaveVTSRequest(request);
        auto dataID = static_cast<uint32_t>(m_impl->m_database->getLastInsertRowid());
        m_impl->m_dataIDLastSentVTSRequest = dataID;
        lock.unlock();

        if (!TimerManager::IsTimerValid(m_impl->m_timerSendMessage))
        {
            auto &timerManager = TimerManager::GetInstance();

            m_impl->m_timerSendMessage = timerManager.AddTimer(20000, false, [&] {
                // Change the sending status
                m_impl->m_timerSendMessage = 0;
                m_impl->m_parent->notifyEvent(EventType::MESSAGE_SEND, -1);
            });
        }
    }

    void VDESManager::SendConfirmationToVTS(const uint32_t requestDataID, const uint32_t mmsiVTS, 
        const bool canExecute, const std::string &chineseRemark, const std::string &englishRemark)
    {
        AISBitsManager aisBitsManager;
        // VPFI
        aisBitsManager.Encode(2, 16);
        // Message No.
        aisBitsManager.Encode(7, 16);
        // Text message number
        aisBitsManager.Encode(m_impl->m_textSequenceNoVTS, 11);
        // Message type
        aisBitsManager.Encode(2, 2);
        // Confirmation 
        aisBitsManager.Encode(canExecute ? 1 : 0, 1);
        // Chinese text
        aisBitsManager.Encode(static_cast<uint32_t>(chineseRemark.length()), 11);
        for (auto &character : chineseRemark)
        {
            aisBitsManager.Encode(character, 8);
        }
        // English text
        aisBitsManager.Encode(static_cast<uint32_t>(englishRemark.length()), 10);
        for (auto &character : englishRemark)
        {
            aisBitsManager.Encode(character, 8);
        }
        // Spare
        aisBitsManager.Encode(0, 5);
        // Authentication
        aisBitsManager.Encode(0, 512);

        auto payload = aisBitsManager.GetEncodedVDMPayload();
        auto fillBitNum = aisBitsManager.GetFillBitsNumberToEncode();
        auto strTDB = fmt::format("$XNTDB,1,1,0,,{},0,0,17,{},{}", mmsiVTS, payload, fillBitNum);
        UtilityInterface::AddChecksum(strTDB);
        sendEvent(CommunicationType::TCP, strTDB.c_str(), strTDB.size());

        m_impl->m_transmissionBusiness = Impl::TransmissionBusiness::VTS_CONFIRM;
        m_impl->m_executingStatus = canExecute ? Impl::ExecutingStatus::CAN_EXECUTE : Impl::ExecutingStatus::CANNOT_EXECUTE;
        
        if (!TimerManager::IsTimerValid(m_impl->m_timerSendMessage))
        {
            auto &timerManager = TimerManager::GetInstance();

            m_impl->m_timerSendMessage = timerManager.AddTimer(20000, false, [&] {
                m_impl->m_timerSendMessage = 0;
                m_impl->m_transmissionBusiness = Impl::TransmissionBusiness::NONE;
                m_impl->m_executingStatus = Impl::ExecutingStatus::NONE;
                m_impl->m_parent->notifyEvent(EventType::MESSAGE_SEND, -1);
            });
        }
    }

    VDESManager::VTSRequests VDESManager::GetVTSRequests(const uint32_t index, const size_t number)
    {
        VTSRequests container;

        if (m_impl->m_database)
        {
            try
            {
                auto sqlCmd = fmt::format("SELECT * FROM VTSRequest ORDER BY [Timestamp Request] "
                                        "DESC LIMIT {0} OFFSET {1}",
                                        (number == -1) ? "-1" : fmt::format("{}", number), index);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexVTSRequest);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    VTSRequest request;

                    m_impl->LoadVTSRequestFromQueryResult(request, query);
                    container.emplace_back(request);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetVTSRequests");
            }
        }
        return container;
    }

    VDESManager::VTSRequestPtr VDESManager::GetVTSRequest(const uint32_t dataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                auto sql = fmt::format("SELECT * FROM VTSRequest WHERE ID = {}", dataID);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexVTSRequest);

                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    VTSRequest request;
                    m_impl->LoadVTSRequestFromQueryResult(request, query);
                    return std::make_shared<VTSRequest>(request);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetVTSRequest");
            }
        }
        return nullptr; 
    }

    VDESManager::VTSReplyPtr VDESManager::GetVTSReply(const uint32_t requestDataID)
    {
        if (m_impl->m_database)
        {
            try
            {
                auto sql = fmt::format("SELECT * FROM VTSReply WHERE [VTS Request ID] = {}", requestDataID);

                std::lock_guard<std::mutex> lock(m_impl->m_mutexVTSReply);

                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    VTSReply reply;
                    m_impl->LoadVTSReplyFromQueryResult(reply, query);
                    return std::make_shared<VTSReply>(reply);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetVTSReply");
            }
        }
        return nullptr;
    }

    void VDESManager::DeleteVTSRequests(const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock(m_impl->m_mutexVTSRequest, m_impl->m_mutexVTSReply);

                std::unique_lock<std::mutex> lock1(m_impl->m_mutexVTSRequest, std::adopt_lock);
                std::unique_lock<std::mutex> lock2(m_impl->m_mutexVTSReply, std::adopt_lock);
                
                SQLite::Transaction transaction(*m_impl->m_database.get());

                auto sqlDelRequest = fmt::format("DELETE FROM VTSRequest WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlDelRequest);
                auto sqlDelReply = fmt::format("DELETE FROM VTSReply WHERE [VTS Request ID] IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlDelReply);
                transaction.commit();
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteVTSRequests");
            }
        }
    }

    VDESManager::MarineMeteorologyFCSTs VDESManager::GetMarineMeteorologyFCSTs(const uint32_t index, const size_t number)
    {
        MarineMeteorologyFCSTs container;
        
        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM MarineMeteorologyFCST ORDER BY [Timestamp Forecast] "
                "DESC LIMIT {0} OFFSET {1}",
                (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineMeteorologyFCST);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                MarineMeteorologyFCST info;

                m_impl->LoadMarineMeteorologyFCSTFromQueryResult(info, query);
                container.emplace_back(info);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMarineMeteorologyFCSTs");
            }
        }
        return container;
    }

    VDESManager::MarineMeteorologyFCSTs VDESManager::GetMarineMeteorologyFCSTs(const Coordinate &coordinate,
        const double radius)
    {
        MarineMeteorologyFCSTs container;

        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineMeteorologyFCST);

                auto bbox = BoundingBox::Build(coordinate.GetLatitude(), coordinate.GetLongitude(), radius);
                auto sqlCmd = m_impl->BuildSQLCmd("MarineMeteorologyFCST", "MeteorologyFCSTBBox", bbox);
                sqlCmd += " ORDER BY [Timestamp Forecast] DESC";
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MarineMeteorologyFCST fcstInfo;

                    m_impl->LoadMarineMeteorologyFCSTFromQueryResult(fcstInfo, query);
                    container.emplace_back(fcstInfo);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMarineMeteorologyFCSTsWithRange");
            }
        }
        return container;
    }

    VDESManager::MarineMeteorologyFCSTs VDESManager::GetMarineMeteorologyFCSTs(const BoundingBox &boundingBox, 
        const size_t number)
    {
        MarineMeteorologyFCSTs container;

        if (m_impl->m_database)
        {
            try
            {
                auto sqlCmd = m_impl->BuildSQLCmd("MarineMeteorologyFCST", "MeteorologyFCSTBBox", boundingBox);
                sqlCmd += " GROUP BY Latitude, Longitude";
                if (number != -1)
                {
                    sqlCmd.append(fmt::format(" LIMIT {}", number));
                }

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineMeteorologyFCST);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
                while (query.executeStep())
                {
                    MarineMeteorologyFCST info;
                    m_impl->LoadMarineMeteorologyFCSTFromQueryResult(info, query);
                    container.emplace_back(info);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMarineMeteorologyFCSTsWithBBox");
            }
        }
        return container;
    }

    bool VDESManager::DeleteMarineMeteorologyFCSTs(const uint32_t index, const size_t number)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineMeteorologyFCST);

                m_impl->m_database->exec("DROP VIEW IF EXISTS DataIDView");

                auto sqlCmd = fmt::format("CREATE VIEW DataIDView AS SELECT ID FROM MarineMeteorologyFCST ORDER BY "
                                          "[Timestamp Forecast] DESC LIMIT {} OFFSET {}", number, index);
                m_impl->m_database->exec(sqlCmd);
                m_impl->m_database->exec("DELETE FROM MarineMeteorologyFCST WHERE ID IN DataIDView");
                m_impl->m_database->exec("DELETE FROM MeteorologyFCSTBBox WHERE ID IN DataIDView");
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMarineMeteorologyFCSTs");
            }
        }
        return false;
    }

    bool VDESManager::DeleteMarineMeteorologyFCSTs(const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineMeteorologyFCST);

                auto sqlCmd = fmt::format("DELETE FROM MarineMeteorologyFCST WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlCmd);
                sqlCmd = fmt::format("DELETE FROM MeteorologyFCSTBBox WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlCmd);
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMarineMeteorologyFCSTs");
            }
        }
        return false;
    }
    
    VDESManager::MarineEnvironmentFCSTs VDESManager::GetMarineEnvironmentFCSTs(const uint32_t index, 
        const size_t number)
    {
        MarineEnvironmentFCSTs container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM MarineEnvironmentFCST ORDER BY [Timestamp Forecast] "
                                  "DESC LIMIT {0} OFFSET {1}",
                                  (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineEnvironmentFCST);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MarineEnvironmentFCST info;

                    m_impl->LoadMarineEnvironmentFCSTFromQueryResult(info, query);
                    container.emplace_back(info);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMarineEnvironmentFCSTs");
            }
        }
        return container;
    }

    VDESManager::MarineMeteorologyEWs VDESManager::GetMarineMeteorologyEWs(const uint32_t index, const size_t number)
    {
        MarineMeteorologyEWs container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM MarineMeteorologyEW ORDER BY [Timestamp Receive] "
                             "DESC LIMIT {0} OFFSET {1}",
                             (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineMeteorologyEW);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MarineMeteorologyEW ew;
                    m_impl->LoadMarineMeteorologyEWFromQueryResult(ew, query);
                    container.emplace_back(ew);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMarineMeteorologyEWs");
            }
        }
        return container;
    }

    VDESManager::TideForecasts VDESManager::GetTideForecasts(const uint32_t index, const size_t number)
    {
        TideForecasts container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM TideForecast ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexTideForecast);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    TideForecast forecast;
                    m_impl->LoadTideForecastFromQueryResult(forecast, query);
                    container.emplace_back(forecast);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetTideForecasts");
            }
        }
        return container;
    }

    VDESManager::AtoNDynamicsList VDESManager::GetAtoNDynamics(const uint32_t index, const size_t number)
    {
        AtoNDynamicsList container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM AtoNDynamics ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAtoNDynamics);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    AtoNDynamics dynamics;
                    m_impl->LoadAtoNDynamicsFromQueryResult(dynamics, query);
                    container.emplace_back(dynamics);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetAtoNDynamics");
            }
        }
        return container;
    }

    VDESManager::AISAtoNDynamicsList VDESManager::GetAISAtoNDynamics(const uint32_t index, const size_t number)
    {
        AISAtoNDynamicsList container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM AISAtoNDynamics ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexAISAtoNDynamics);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    AISAtoNDynamics dynamics;
                    m_impl->LoadAISAtoNDynamicsFromQueryResult(dynamics, query);
                    container.emplace_back(dynamics);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetAISAtoNDynamics");
            }
        }
        return container;
    }
    
    VDESManager::MarineEnvironmentFCSTs VDESManager::GetMarineEnvironmentFCSTs(const Coordinate &coordinate,
        const double radius)
    {
        MarineEnvironmentFCSTs container;

        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineEnvironmentFCST);

                auto bbox = BoundingBox::Build(coordinate.GetLatitude(), coordinate.GetLongitude(), radius);
                auto sqlCmd = m_impl->BuildSQLCmd("MarineEnvironmentFCST", "EnvironmentFCSTBBox", bbox);
                sqlCmd += " ORDER BY [Timestamp Forecast] DESC";
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MarineEnvironmentFCST fcstInfo;

                    m_impl->LoadMarineEnvironmentFCSTFromQueryResult(fcstInfo, query);
                    container.emplace_back(fcstInfo);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMarineEnvironmentFCSTsWithRange");
            }
        }
        return container;
    }

    VDESManager::MarineEnvironmentFCSTs VDESManager::GetMarineEnvironmentFCSTs(const BoundingBox &bbox, 
        const size_t number)
    {
        MarineEnvironmentFCSTs container;

        if (m_impl->m_database)
        {
            try
            {
                auto sqlCmd = m_impl->BuildSQLCmd("MarineEnvironmentFCST", "EnvironmentFCSTBBox", bbox);
                sqlCmd += " GROUP BY Latitude, Longitude";
                if (number != -1)
                {
                    sqlCmd.append(fmt::format(" LIMIT {}", number));
                }

                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineEnvironmentFCST);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
                while (query.executeStep())
                {
                    MarineEnvironmentFCST fcstInfo;

                    m_impl->LoadMarineEnvironmentFCSTFromQueryResult(fcstInfo, query);
                    container.emplace_back(fcstInfo);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMarineEnvironmentFCSTsWithBBox");
            }
        }
        return container;
    }
    
    bool VDESManager::DeleteMarineEnvironmentFCSTs(const uint32_t index, const size_t number)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineEnvironmentFCST);

                m_impl->m_database->exec("DROP VIEW IF EXISTS EnvironmentDataIDView");

                auto sqlCmd = fmt::format("CREATE VIEW EnvironmentDataIDView AS SELECT ID FROM MarineEnvironmentFCST "
                                          "ORDER BY [Timestamp Forecast] DESC LIMIT {} OFFSET {}", 
                                          number, index);
                m_impl->m_database->exec(sqlCmd);
                m_impl->m_database->exec("DELETE FROM MarineEnvironmentFCST WHERE ID IN EnvironmentDataIDView");
                m_impl->m_database->exec("DELETE FROM EnvironmentFCSTBBox WHERE ID IN EnvironmentDataIDView");
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMarineEnvironmentFCSTs");
            }
        }
        return false;
    }

    bool VDESManager::DeleteMarineEnvironmentFCSTs(const std::vector<uint32_t> &dataIDs)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineEnvironmentFCST);

                auto sqlCmd = fmt::format("DELETE FROM MarineEnvironmentFCST WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlCmd);
                sqlCmd = fmt::format("DELETE FROM EnvironmentFCSTBBox WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlCmd);
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteMarineEnvironmentFCSTs");
            }
        }
        return false;
    }

    VDESManager::MarineMeteorologyFCSTAreas VDESManager::GetMarineMeteorologyFCSTAreas(const uint32_t index, const size_t number)
    {
        MarineMeteorologyFCSTAreas container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM MarineMeteorologyFCSTArea ORDER BY [Timestamp Forecast] "
                "DESC LIMIT {0} OFFSET {1}",
                (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineMeteorologyFCSTArea);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MarineMeteorologyFCSTArea info;
                    m_impl->LoadMarineMeteorologyFCSTAreaFromQueryResult(info, query);
                    container.emplace_back(info);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMarineMeteorologyFCSTAreas");
            }
        }
        return container;
    }

    VDESManager::MarineEnvironmentFCSTAreas VDESManager::GetMarineEnvironmentFCSTAreas(const uint32_t index, const size_t number)
    {
        MarineEnvironmentFCSTAreas container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM MarineEnvironmentFCSTArea ORDER BY [Timestamp Forecast] "
                "DESC LIMIT {0} OFFSET {1}",
                (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineEnvironmentFCSTArea);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MarineEnvironmentFCSTArea info;
                    m_impl->LoadMarineEnvironmentFCSTAreaFromQueryResult(info, query);
                    container.emplace_back(info);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMarineEnvironmentFCSTAreas");
            }
        }
        return container;
    }

    VDESManager::MarineEnvironmentFCSTAlongshores VDESManager::GetMarineEnvironmentFCSTAlongshores(const uint32_t index, const size_t number)
    {
        MarineEnvironmentFCSTAlongshores container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM MarineEnvironmentFCSTAlongshore ORDER BY [Timestamp Forecast] "
                "DESC LIMIT {0} OFFSET {1}",
                (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexMarineEnvironmentFCSTAlongshore);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    MarineEnvironmentFCSTAlongshore info;
                    m_impl->LoadMarineEnvironmentFCSTAlongshoreFromQueryResult(info, query);
                    container.emplace_back(info);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetMarineEnvironmentFCSTAlongshores");
            }
        }
        return container;
    }

    VDESManager::Bridges VDESManager::GetBridges(const uint32_t index, const size_t number)
    {
        Bridges container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM Bridge ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexBridge);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    Bridge bridge;

                    m_impl->LoadBridgeFromQueryResult(bridge, query);
                    container.push_back(bridge);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetBridges");
            }
        }
        return container;
    }

    VDESManager::NetSounders VDESManager::GetNetSounders(const uint32_t index, const size_t number)
    {
        NetSounders container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM NetSounder ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexNetSounder);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    NetSounder netSounder;

                    m_impl->LoadNetSounderFromQueryResult(netSounder, query);
                    container.push_back(netSounder);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetNetSounders");
            }
        }
        return container;
    }

    VDESManager::ChannelCenterlines VDESManager::GetChannelCenterlines(const uint32_t index, const size_t number)
    {
        ChannelCenterlines container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM ChannelCenterline ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexChannelCenterline);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    ChannelCenterline centerline;

                    m_impl->LoadChannelCenterlineFromQueryResult(centerline, query);
                    container.push_back(centerline);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetChannelCenterlines");
            }
        }
        return container;
    }

    VDESManager::ChannelBoundaries VDESManager::GetChannelBoundaries(const uint32_t index, const size_t number)
    {
        ChannelBoundaries container;

        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT rowid, * FROM ChannelBoundary ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexChannelBoundary);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    ChannelBoundary boundary;

                    m_impl->LoadChannelBoundaryFromQueryResult(boundary, query);
                    container.push_back(boundary);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetChannelBoundaries");
            }
        }
        return container;
    }

    VDESManager::FrontendPrompts VDESManager::GetFrontendPrompts(const uint32_t index, const size_t number)
    {
        FrontendPrompts container;

        if (m_impl->m_database)
        {
            auto currentTime = UtilityInterface::GetCurrentTimeStamp();
            auto sqlCmd = fmt::format("SELECT rowid, * FROM FrontendPrompt WHERE [Timestamp Expiry] = 0 OR [Timestamp Expiry] > {0} "
                                      "ORDER BY [Timestamp Receive] DESC LIMIT {1} OFFSET {2}",
                                      currentTime,
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexFrontendPrompt);

                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);

                while (query.executeStep())
                {
                    FrontendPrompt prompt;

                    m_impl->LoadFrontendPromptFromQueryResult(prompt, query);
                    container.push_back(prompt);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetFrontendPrompts");
            }
        }
        return container;
    }

    bool VDESManager::DeleteFrontendPrompts(const std::vector<uint32_t> &dataIDs)
    {
        if (dataIDs.empty())
        {
            return true;
        }

        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexFrontendPrompt);

                std::string sqlCmd = "DELETE FROM FrontendPrompt WHERE ID IN (";
                for (size_t i = 0; i < dataIDs.size(); ++i)
                {
                    sqlCmd += fmt::format("{}", dataIDs[i]);
                    if (i != dataIDs.size() - 1)
                    {
                        sqlCmd += ",";
                    }
                }
                sqlCmd += ")";

                m_impl->m_database->exec(sqlCmd);
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteFrontendPrompts");
            }
        }
        return false;
    }

    void VDESManager::Impl::SaveRouteRecommendationResponse(const RouteRecommendationResponse &response)
    {
        if (m_database)
        {
            try
            {
                // Auto cleanup records older than 7 days
                auto limitTime = UtilityInterface::GetCurrentTimeStamp() - 7 * 24 * 3600;
                m_database->exec(fmt::format("DELETE FROM RouteRecommendationResponse WHERE [Timestamp Receive] < {}", limitTime));

                auto sql = "REPLACE INTO RouteRecommendationResponse (ID, [Effective Time], Coordinates, [Timestamp Receive], Read) "
                           "VALUES (@ID, @EffectiveTime, @Coordinates, @TimestampRcv, @Read)";
                auto stmt = m_database->buildStatement(sql);
                if (response.dataID != 0)
                {
                    stmt.bind("@ID", response.dataID);
                }
                stmt.bind("@EffectiveTime", response.effectiveTime);
                stmt.bind("@TimestampRcv", response.timestamp);
                stmt.bind("@Read", response.read ? 1 : 0);

                auto pointsCount = response.coordinates.size();
                auto size = pointsCount * sizeof(CoordinateStruct);
                auto blob = std::unique_ptr<CoordinateStruct[]>(new CoordinateStruct[pointsCount]);
                for (size_t i = 0; i < pointsCount; ++i)
                {
                    blob[i].latitude  = response.coordinates[i].GetLatitude();
                    blob[i].longitude = response.coordinates[i].GetLongitude();
                }
                stmt.bind("@Coordinates", blob.get(), static_cast<int>(size));
                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveRouteRecommendationResponse");
            }
        }
    }

    void VDESManager::Impl::InitializeRouteRecommendationResponseTable(void)
    {
        try
        {
            if (!m_database->tableExists("RouteRecommendationResponse"))
            {
                auto sql = "CREATE TABLE RouteRecommendationResponse ("
                           "ID                      INTEGER   PRIMARY KEY AUTOINCREMENT,"
                           "[Effective Time]        DOUBLE    NOT NULL DEFAULT 31.5,"
                           "Coordinates             BLOB,"
                           "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                           "Read                    BOOLEAN   NOT NULL DEFAULT 0)";
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeRouteRecommendationResponseTable");
        }
    }

    void VDESManager::Impl::LoadRouteRecommendationResponseFromQueryResult(RouteRecommendationResponse &response, const SQLite::Statement &query)
    {
        response.dataID = query.getColumn("ID").getUInt();
        response.effectiveTime = query.getColumn("Effective Time").getDouble();
        response.timestamp = query.getColumn("Timestamp Receive").getInt64();
        response.read = query.getColumn("Read").getUInt() != 0;
        response.DAC = 412;
        response.FI = 46;

        SQLite::Column column = query.getColumn("Coordinates");
        auto size = column.getBytes();
        if (size > 0)
        {
            auto ptr = (const CoordinateStruct *)column.getBlob();
            auto count = size / sizeof(CoordinateStruct);
            response.coordinates.clear();
            for (auto i = 0U; i < count; i++)
            {
                response.coordinates.push_back(Coordinate(ptr[i].latitude, ptr[i].longitude));
            }
        }
    }

    void VDESManager::Impl::SaveHydrometeorologyResponse(const HydrometeorologyResponse &response)
    {
        if (m_database)
        {
            try
            {
                // Auto cleanup records older than 7 days
                auto limitTime = UtilityInterface::GetCurrentTimeStamp() - 7 * 24 * 3600;
                m_database->exec(fmt::format("DELETE FROM HydrometeorologyResponse WHERE [Timestamp Receive] < {}", limitTime));

                auto sql = "REPLACE INTO HydrometeorologyResponse (ID, MRN, [Forecast Time], "
                           "[Has Wind Speed], [Has Wind Direction], [Has Visibility], [Has Wave Height], "
                           "[Has Wave Direction], [Has Swell Height], Points, [Timestamp Receive], Read) "
                           "VALUES (@ID, @MRN, @ForecastTime, @HasWindSpeed, @HasWindDirection, @HasVisibility, "
                           "@HasWaveHeight, @HasWaveDirection, @HasSwellHeight, @Points, @TimestampRcv, @Read)";
                auto stmt = m_database->buildStatement(sql);
                if (response.dataID != 0)
                {
                    stmt.bind("@ID", response.dataID);
                }
                stmt.bind("@MRN", response.MRN);
                stmt.bind("@ForecastTime", response.forecastTime);
                stmt.bind("@HasWindSpeed", response.hasWindSpeed ? 1 : 0);
                stmt.bind("@HasWindDirection", response.hasWindDirection ? 1 : 0);
                stmt.bind("@HasVisibility", response.hasVisibility ? 1 : 0);
                stmt.bind("@HasWaveHeight", response.hasWaveHeight ? 1 : 0);
                stmt.bind("@HasWaveDirection", response.hasWaveDirection ? 1 : 0);
                stmt.bind("@HasSwellHeight", response.hasSwellHeight ? 1 : 0);
                stmt.bind("@TimestampRcv", response.timestamp);
                stmt.bind("@Read", response.read ? 1 : 0);

                auto pointsCount = response.points.size();
                auto size = pointsCount * sizeof(HydrometeorologyResponse::PointForecast);
                stmt.bind("@Points", response.points.data(), static_cast<int>(size));
                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveHydrometeorologyResponse");
            }
        }
    }

    void VDESManager::Impl::InitializeHydrometeorologyResponseTable(void)
    {
        try
        {
            if (!m_database->tableExists("HydrometeorologyResponse"))
            {
                auto sql = "CREATE TABLE HydrometeorologyResponse ("
                           "ID                      INTEGER   PRIMARY KEY AUTOINCREMENT,"
                           "MRN                     INTEGER   NOT NULL DEFAULT 0,"
                           "[Forecast Time]         INTEGER   NOT NULL DEFAULT 0,"
                           "[Has Wind Speed]        BOOLEAN   NOT NULL DEFAULT 0,"
                           "[Has Wind Direction]    BOOLEAN   NOT NULL DEFAULT 0,"
                           "[Has Visibility]        BOOLEAN   NOT NULL DEFAULT 0,"
                           "[Has Wave Height]       BOOLEAN   NOT NULL DEFAULT 0,"
                           "[Has Wave Direction]    BOOLEAN   NOT NULL DEFAULT 0,"
                           "[Has Swell Height]      BOOLEAN   NOT NULL DEFAULT 0,"
                           "Points                  BLOB,"
                           "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                           "Read                    BOOLEAN   NOT NULL DEFAULT 0)";
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeHydrometeorologyResponseTable");
        }
    }

    void VDESManager::Impl::LoadHydrometeorologyResponseFromQueryResult(HydrometeorologyResponse &response, const SQLite::Statement &query)
    {
        response.dataID = query.getColumn("ID").getUInt();
        response.MRN = query.getColumn("MRN").getUInt();
        response.forecastTime = query.getColumn("Forecast Time").getInt64();
        response.hasWindSpeed = query.getColumn("Has Wind Speed").getUInt() != 0;
        response.hasWindDirection = query.getColumn("Has Wind Direction").getUInt() != 0;
        response.hasVisibility = query.getColumn("Has Visibility").getUInt() != 0;
        response.hasWaveHeight = query.getColumn("Has Wave Height").getUInt() != 0;
        response.hasWaveDirection = query.getColumn("Has Wave Direction").getUInt() != 0;
        response.hasSwellHeight = query.getColumn("Has Swell Height").getUInt() != 0;
        response.timestamp = query.getColumn("Timestamp Receive").getInt64();
        response.read = query.getColumn("Read").getUInt() != 0;
        response.DAC = 412;
        response.FI = 49;

        SQLite::Column column = query.getColumn("Points");
        auto size = column.getBytes();
        if (size > 0)
        {
            auto ptr = (const HydrometeorologyResponse::PointForecast *)column.getBlob();
            auto count = size / sizeof(HydrometeorologyResponse::PointForecast);
            response.points.clear();
            for (auto i = 0U; i < count; i++)
            {
                response.points.push_back(ptr[i]);
            }
        }
    }

    void VDESManager::Impl::SaveHydrometeorologyRequest(const HydrometeorologyRequest &request, uint32_t seqNo)
    {
        if (m_database)
        {
            try
            {
                auto limitTime = UtilityInterface::GetCurrentTimeStamp() - 7 * 24 * 3600;
                m_database->exec(fmt::format("DELETE FROM HydrometeorologyRequest WHERE [Timestamp Sent] < {}", limitTime));

                auto sql = "INSERT INTO HydrometeorologyRequest (MRN, [Requested Info], [Request Time], [Timestamp Sent], [Sequence Num], [Send Status], Coordinates) "
                           "VALUES (@MRN, @RequestedInfo, @RequestTime, @TimestampSent, @SeqNum, @SendStatus, @Coordinates)";
                auto stmt = m_database->buildStatement(sql);
                stmt.bind("@MRN", request.MRN);

                uint16_t reqInfo = 0;
                if (request.windSpeed)     reqInfo |= (1 << 0);
                if (request.windDirection) reqInfo |= (1 << 1);
                if (request.visibility)    reqInfo |= (1 << 2);
                if (request.waveHeight)     reqInfo |= (1 << 3);
                if (request.waveDirection) reqInfo |= (1 << 4);
                if (request.swellHeight)    reqInfo |= (1 << 5);

                stmt.bind("@RequestedInfo", reqInfo);
                stmt.bind("@RequestTime", request.requestTime);
                stmt.bind("@TimestampSent", UtilityInterface::GetCurrentTimeStamp());
                stmt.bind("@SeqNum", seqNo);
                stmt.bind("@SendStatus", -1);

                auto pointsCount = request.coordinates.size();
                auto size = pointsCount * sizeof(CoordinateStruct);
                auto blob = std::unique_ptr<CoordinateStruct[]>(new CoordinateStruct[pointsCount]);
                for (size_t i = 0; i < pointsCount; ++i)
                {
                    blob[i].latitude  = request.coordinates[i].GetLatitude();
                    blob[i].longitude = request.coordinates[i].GetLongitude();
                }
                stmt.bind("@Coordinates", blob.get(), static_cast<int>(size));
                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveHydrometeorologyRequest");
            }
        }
    }

    void VDESManager::Impl::InitializeHydrometeorologyRequestTable(void)
    {
        try
        {
            if (!m_database->tableExists("HydrometeorologyRequest"))
            {
                auto sql = "CREATE TABLE HydrometeorologyRequest ("
                           "ID                      INTEGER   PRIMARY KEY AUTOINCREMENT,"
                           "MRN                     INTEGER   NOT NULL DEFAULT 0,"
                           "[Requested Info]        INTEGER   NOT NULL DEFAULT 0,"
                           "[Request Time]          INTEGER   NOT NULL DEFAULT 0,"
                           "[Timestamp Sent]        INTEGER   NOT NULL DEFAULT 0,"
                           "[Sequence Num]          INTEGER   NOT NULL DEFAULT 0,"
                           "[Send Status]           INTEGER   NOT NULL DEFAULT -1,"
                           "Coordinates             BLOB)";
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeHydrometeorologyRequestTable");
        }
    }

    void VDESManager::Impl::InitializeRouteRecommendationRequestTable(void)
    {
        try
        {
            if (!m_database->tableExists("RouteRecommendationRequest"))
            {
                auto sql = "CREATE TABLE RouteRecommendationRequest ("
                           "ID                      INTEGER   PRIMARY KEY AUTOINCREMENT,"
                           "[Gross Tonnage]         INTEGER   NOT NULL DEFAULT 0,"
                           "[Max Static Draft]      DOUBLE    NOT NULL DEFAULT 0.0,"
                           "[Cargo Type]            INTEGER   NOT NULL DEFAULT 0,"
                           "Month                   INTEGER   NOT NULL DEFAULT 0,"
                           "Day                     INTEGER   NOT NULL DEFAULT 0,"
                           "Hour                    INTEGER   NOT NULL DEFAULT 0,"
                           "Minute                  INTEGER   NOT NULL DEFAULT 0,"
                           "[Start Longitude]       DOUBLE    NOT NULL DEFAULT 0.0,"
                           "[Start Latitude]        DOUBLE    NOT NULL DEFAULT 0.0,"
                           "[Dest Longitude]        DOUBLE    NOT NULL DEFAULT 0.0,"
                           "[Dest Latitude]         DOUBLE    NOT NULL DEFAULT 0.0,"
                           "[Timestamp Sent]        INTEGER   NOT NULL DEFAULT 0,"
                           "[Sequence Num]          INTEGER   NOT NULL DEFAULT 0,"
                           "[Send Status]           INTEGER   NOT NULL DEFAULT -1)";
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeRouteRecommendationRequestTable");
        }
    }

    void VDESManager::Impl::SaveRouteRecommendationRequest(const RouteRecommendationRequest &request, uint32_t seqNo)
    {
        if (m_database)
        {
            try
            {
                auto limitTime = UtilityInterface::GetCurrentTimeStamp() - 7 * 24 * 3600;
                m_database->exec(fmt::format("DELETE FROM RouteRecommendationRequest WHERE [Timestamp Sent] < {}", limitTime));

                auto sql = "INSERT INTO RouteRecommendationRequest ([Gross Tonnage], [Max Static Draft], [Cargo Type], Month, Day, Hour, Minute, "
                           "[Start Longitude], [Start Latitude], [Dest Longitude], [Dest Latitude], [Timestamp Sent], [Sequence Num], [Send Status]) "
                           "VALUES (@GrossTonnage, @MaxStaticDraft, @CargoType, @Month, @Day, @Hour, @Minute, "
                           "@StartLon, @StartLat, @DestLon, @DestLat, @TimestampSent, @SeqNum, @SendStatus)";
                auto stmt = m_database->buildStatement(sql);
                stmt.bind("@GrossTonnage", request.grossTonnage);
                stmt.bind("@MaxStaticDraft", request.maxStaticDraft);
                stmt.bind("@CargoType", request.cargoType);
                stmt.bind("@Month", request.month);
                stmt.bind("@Day", request.day);
                stmt.bind("@Hour", request.hour);
                stmt.bind("@Minute", request.minute);
                stmt.bind("@StartLon", request.startCoordinate.GetLongitude());
                stmt.bind("@StartLat", request.startCoordinate.GetLatitude());
                stmt.bind("@DestLon", request.destCoordinate.GetLongitude());
                stmt.bind("@DestLat", request.destCoordinate.GetLatitude());
                stmt.bind("@TimestampSent", UtilityInterface::GetCurrentTimeStamp());
                stmt.bind("@SeqNum", seqNo);
                stmt.bind("@SendStatus", -1);
                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveRouteRecommendationRequest");
            }
        }
    }

    void VDESManager::Impl::ParseAMK(const std::string &sentence)
    {
        std::vector<std::string> seps {",", "*"};
        auto strList = UtilityInterface::SplitString(sentence, seps);
        if (strList.size() >= 3)
        {
            uint32_t seqNo = strtoul(strList[1].c_str(), nullptr, 10);
            uint8_t ackType = static_cast<uint8_t>(strtoul(strList[2].c_str(), nullptr, 10));

            UpdateAABRequestStatus(seqNo, ackType);
        }
    }

    void VDESManager::Impl::UpdateAABRequestStatus(uint32_t seqNo, uint8_t ackType)
    {
        if (m_database)
        {
            try
            {
                auto sqlRoute = fmt::format(
                    "UPDATE RouteRecommendationRequest SET [Send Status] = {0} "
                    "WHERE ID = (SELECT ID FROM RouteRecommendationRequest WHERE [Sequence Num] = {1} ORDER BY [Timestamp Sent] DESC LIMIT 1)",
                    ackType, seqNo);
                m_database->exec(sqlRoute);

                auto sqlHydro = fmt::format(
                    "UPDATE HydrometeorologyRequest SET [Send Status] = {0} "
                    "WHERE ID = (SELECT ID FROM HydrometeorologyRequest WHERE [Sequence Num] = {1} ORDER BY [Timestamp Sent] DESC LIMIT 1)",
                    ackType, seqNo);
                m_database->exec(sqlHydro);

                int retCode = 2;
                if (ackType == 0) retCode = 1;
                else if (ackType == 2) retCode = 3;
                else if (ackType == 4) retCode = 4;
                else if (ackType == 5) retCode = 5;
                else if (ackType == 6) retCode = 6;
                
                m_parent->notifyEvent(EventType::MESSAGE_SEND, retCode);
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "UpdateAABRequestStatus");
            }
        }
    }

    void VDESManager::Impl::InitializeOwnExtendedVesselInfoTable(void)
    {
        try
        {
            if (!m_database->tableExists("OwnExtendedVesselInfo"))
            {
                auto sql = "CREATE TABLE OwnExtendedVesselInfo ("
                           "MMSI                    INTEGER   PRIMARY KEY,"
                           "[Extended Vessel Type]  INTEGER   NOT NULL DEFAULT 0,"
                           "[Autonomous Level]      INTEGER   NOT NULL DEFAULT 0,"
                           "[Port Code]             TEXT,"
                           "[Chinese Name]          TEXT)";
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeOwnExtendedVesselInfoTable");
        }
    }

    void VDESManager::Impl::InitializeOtherVesselExtendedInfoTable(void)
    {
        try
        {
            if (!m_database->tableExists("OtherVesselExtendedInfo"))
            {
                auto sql = "CREATE TABLE OtherVesselExtendedInfo ("
                           "ID                      INTEGER   PRIMARY KEY AUTOINCREMENT,"
                           "MMSI                    INTEGER   NOT NULL UNIQUE,"
                           "[Extended Vessel Type]  INTEGER   NOT NULL DEFAULT 0,"
                           "[Autonomous Level]      INTEGER   NOT NULL DEFAULT 0,"
                           "[Navi Status]           INTEGER   NOT NULL DEFAULT 15,"
                           "Draught                 DOUBLE    NOT NULL DEFAULT 0.0,"
                           "ETA                     INTEGER   NOT NULL DEFAULT 0,"
                           "[Port Code]             TEXT,"
                           "[Crew Num]              INTEGER   NOT NULL DEFAULT 0,"
                           "[Chinese Name]          TEXT,"
                           "[Timestamp Receive]     INTEGER   NOT NULL DEFAULT 0,"
                           "Read                    BOOLEAN   NOT NULL DEFAULT 0)";
                m_database->exec(sql);
            }
        }
        catch (const SQLite::Exception &execption)
        {
            DatabaseErrorProcess(execption, "InitializeOtherVesselExtendedInfoTable");
        }
    }

    void VDESManager::Impl::SaveOwnExtendedVesselInfo(const ExtendedVesselInfo &info)
    {
        if (m_database)
        {
            try
            {
                auto ownVessel = ConfigureManager::GetInstance().GetOwnVesselInfo();
                auto sql = "REPLACE INTO OwnExtendedVesselInfo (MMSI, [Extended Vessel Type], [Autonomous Level], [Port Code], [Chinese Name]) "
                           "VALUES (@MMSI, @Type, @Level, @Port, @Chinese)";
                auto stmt = m_database->buildStatement(sql);
                stmt.bind("@MMSI", ownVessel.mmsi);
                stmt.bind("@Type", info.extendedVesselType);
                stmt.bind("@Level", info.autonomousLevel);
                stmt.bind("@Port", info.portCode);
                stmt.bind("@Chinese", info.chineseName);
                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveOwnExtendedVesselInfo");
            }
        }
    }

    void VDESManager::Impl::SaveOtherVesselExtendedInfoPartA(const ASM_DAC_412_FI_50 &partA)
    {
        if (m_database)
        {
            try
            {
                auto sqlInsert = fmt::format("INSERT OR IGNORE INTO OtherVesselExtendedInfo (MMSI) VALUES ({})", partA.mmsi);
                m_database->exec(sqlInsert);

                auto sql = "UPDATE OtherVesselExtendedInfo SET "
                           "[Extended Vessel Type] = @Type, "
                           "[Autonomous Level] = @Level, "
                           "[Navi Status] = @Navi, "
                           "Draught = @Draught, "
                           "ETA = @ETA, "
                           "[Port Code] = @Port, "
                           "[Timestamp Receive] = @TimestampRcv, "
                           "Read = 0 "
                           "WHERE MMSI = @MMSI";
                auto stmt = m_database->buildStatement(sql);
                stmt.bind("@MMSI", partA.mmsi);
                stmt.bind("@Type", partA.extendedVesselType);
                stmt.bind("@Level", partA.autonomousLevel);
                stmt.bind("@Navi", partA.naviStatus);
                stmt.bind("@Draught", partA.draught);
                stmt.bind("@ETA", partA.eta);
                stmt.bind("@Port", partA.portCode);
                stmt.bind("@TimestampRcv", UtilityInterface::GetCurrentTimeStamp());
                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveOtherVesselExtendedInfoPartA");
            }
        }
    }

    void VDESManager::Impl::SaveOtherVesselExtendedInfoPartB(const ASM_DAC_412_FI_51 &partB)
    {
        if (m_database)
        {
            try
            {
                auto sqlInsert = fmt::format("INSERT OR IGNORE INTO OtherVesselExtendedInfo (MMSI) VALUES ({})", partB.mmsi);
                m_database->exec(sqlInsert);

                auto sql = "UPDATE OtherVesselExtendedInfo SET "
                           "[Crew Num] = @Crew, "
                           "[Chinese Name] = @Chinese, "
                           "[Timestamp Receive] = @TimestampRcv, "
                           "Read = 0 "
                           "WHERE MMSI = @MMSI";
                auto stmt = m_database->buildStatement(sql);
                stmt.bind("@MMSI", partB.mmsi);
                stmt.bind("@Crew", partB.crewNum);
                stmt.bind("@Chinese", partB.chineseName);
                stmt.bind("@TimestampRcv", UtilityInterface::GetCurrentTimeStamp());
                stmt.exec();
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SaveOtherVesselExtendedInfoPartB");
            }
        }
    }

    void VDESManager::Impl::LoadOtherVesselExtendedInfoFromQueryResult(OtherVesselExtendedInfo &info, const SQLite::Statement &query)
    {
        info.dataID = query.getColumn("ID").getUInt();
        info.mmsi = query.getColumn("MMSI").getUInt();
        info.extendedVesselType = query.getColumn("Extended Vessel Type").getUInt();
        info.autonomousLevel = query.getColumn("Autonomous Level").getUInt();
        info.naviStatus = query.getColumn("Navi Status").getUInt();
        info.draught = query.getColumn("Draught").getDouble();
        info.eta = query.getColumn("ETA").getInt64();
        info.portCode = query.getColumn("Port Code").getText();
        info.crewNum = query.getColumn("Crew Num").getUInt();
        info.chineseName = query.getColumn("Chinese Name").getText();
        info.timestamp = query.getColumn("Timestamp Receive").getInt64();
        info.read = query.getColumn("Read").getUInt() != 0;
        info.DAC = 412;
        info.FI = 50;
    }

    void VDESManager::Impl::SendExtendedVesselInfoPartA(void)
    {
        ExtendedVesselInfo info;
        if (m_database)
        {
            try
            {
                auto ownVessel = ConfigureManager::GetInstance().GetOwnVesselInfo();
                auto sql = fmt::format("SELECT * FROM OwnExtendedVesselInfo WHERE MMSI = {}", ownVessel.mmsi);
                SQLite::Statement query(*m_database.get(), sql);
                if (query.executeStep())
                {
                    info.extendedVesselType = query.getColumn("Extended Vessel Type").getUInt();
                    info.autonomousLevel = query.getColumn("Autonomous Level").getUInt();
                    info.portCode = query.getColumn("Port Code").getText();
                    info.chineseName = query.getColumn("Chinese Name").getText();
                }
                else
                {
                    return;
                }
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SendExtendedVesselInfoPartA_Load");
                return;
            }
        }

        auto ownVessel = ConfigureManager::GetInstance().GetOwnVesselInfo();

        AISBitsManager aisBitsManager;
        aisBitsManager.Encode(412, 10);
        aisBitsManager.Encode(50, 6);
        aisBitsManager.Encode(info.extendedVesselType, 8);
        aisBitsManager.Encode(info.autonomousLevel, 3);
        aisBitsManager.Encode(ownVessel.naviStatus, 4);
        
        uint8_t rawDraught = static_cast<uint8_t>(ownVessel.draught * 10);
        aisBitsManager.Encode(rawDraught, 8);

        if (ownVessel.eta.month > 0)
        {
            aisBitsManager.Encode(ownVessel.eta.month & 0xF, 4);
            aisBitsManager.Encode(ownVessel.eta.day & 0x1F, 5);
            aisBitsManager.Encode(ownVessel.eta.hour & 0x1F, 5);
            aisBitsManager.Encode(ownVessel.eta.minute & 0x3F, 6);
        }
        else
        {
            aisBitsManager.Encode(0, 20);
        }

        std::string portStr = info.portCode;
        while (portStr.size() < 5)
        {
            portStr.push_back('@');
        }
        for (int i = 0; i < 5; ++i)
        {
            char ch = portStr[i];
            uint8_t val = 0;
            if (ch >= 64 && ch <= 95) val = ch - 64;
            else if (ch >= 32 && ch <= 63) val = ch;
            aisBitsManager.Encode(val, 6);
        }

        auto payload = aisBitsManager.GetEncodedVDMPayload();
        auto fillBits = aisBitsManager.GetFillBitsNumberToEncode();

        std::string nmea = fmt::format("!AIVDO,1,1,,B,{0},{1}", payload, fillBits);
        UtilityInterface::AddChecksum(nmea);
        nmea += "\r\n";

        m_parent->sendEvent(CommunicationType::TCP, nmea.c_str(), nmea.length());
    }

    void VDESManager::Impl::SendExtendedVesselInfoPartB(void)
    {
        ExtendedVesselInfo info;
        if (m_database)
        {
            try
            {
                auto ownVessel = ConfigureManager::GetInstance().GetOwnVesselInfo();
                auto sql = fmt::format("SELECT * FROM OwnExtendedVesselInfo WHERE MMSI = {}", ownVessel.mmsi);
                SQLite::Statement query(*m_database.get(), sql);
                if (query.executeStep())
                {
                    info.extendedVesselType = query.getColumn("Extended Vessel Type").getUInt();
                    info.autonomousLevel = query.getColumn("Autonomous Level").getUInt();
                    info.portCode = query.getColumn("Port Code").getText();
                    info.chineseName = query.getColumn("Chinese Name").getText();
                }
                else
                {
                    return;
                }
            }
            catch (const SQLite::Exception &execption)
            {
                DatabaseErrorProcess(execption, "SendExtendedVesselInfoPartB_Load");
                return;
            }
        }

        auto ownVessel = ConfigureManager::GetInstance().GetOwnVesselInfo();

        AISBitsManager aisBitsManager;
        aisBitsManager.Encode(412, 10);
        aisBitsManager.Encode(51, 6);
        aisBitsManager.Encode(ownVessel.crewNum & 0x1FFF, 13);

        std::string gbkStr = UtilityInterface::UTF8ToGBK(info.chineseName);
        for (size_t i = 0; i < gbkStr.size(); )
        {
            uint16_t val = 0;
            uint8_t b1 = static_cast<uint8_t>(gbkStr[i]);
            if (b1 >= 0xA1 && b1 <= 0xFE && i + 1 < gbkStr.size())
            {
                uint8_t b2 = static_cast<uint8_t>(gbkStr[i + 1]);
                val = (b1 - 0xA1) * 94 + (b2 - 0xA1);
                i += 2;
            }
            else
            {
                val = (0xA1 - 0xA1) * 94 + (b1 - 0xA1);
                i += 1;
            }
            aisBitsManager.Encode(val, 14);
        }

        auto payload = aisBitsManager.GetEncodedVDMPayload();
        auto fillBits = aisBitsManager.GetFillBitsNumberToEncode();

        std::string nmea = fmt::format("!AIVDO,1,1,,B,{0},{1}", payload, fillBits);
        UtilityInterface::AddChecksum(nmea);
        nmea += "\r\n";

        m_parent->sendEvent(CommunicationType::TCP, nmea.c_str(), nmea.length());
    }

    void VDESManager::Impl::BroadcastExtendedVesselInfo(void)
    {
        SendExtendedVesselInfoPartA();
        SendExtendedVesselInfoPartB();
    }

    VDESManager::RouteRecommendationResponses VDESManager::GetRouteRecommendationResponses(const uint32_t index, const size_t number)
    {
        RouteRecommendationResponses container;
        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM RouteRecommendationResponse ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexRouteRecommendationResponse);
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
                while (query.executeStep())
                {
                    RouteRecommendationResponse response;
                    m_impl->LoadRouteRecommendationResponseFromQueryResult(response, query);
                    container.emplace_back(response);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetRouteRecommendationResponses");
            }
        }
        return container;
    }

    bool VDESManager::DeleteRouteRecommendationResponses(const uint32_t index, const size_t number)
    {
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexRouteRecommendationResponse);
                m_impl->m_database->exec("DROP VIEW IF EXISTS RouteRecRespIDView");
                auto sqlCmd = fmt::format("CREATE VIEW RouteRecRespIDView AS SELECT ID FROM RouteRecommendationResponse ORDER BY [Timestamp Receive] DESC LIMIT {0} OFFSET {1}",
                                          (number == -1) ? "-1" : fmt::format("{}", number), index);
                m_impl->m_database->exec(sqlCmd);
                m_impl->m_database->exec("DELETE FROM RouteRecommendationResponse WHERE ID IN (SELECT ID FROM RouteRecRespIDView)");
                m_impl->m_database->exec("DROP VIEW IF EXISTS RouteRecRespIDView");
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteRouteRecommendationResponses_Limit");
            }
        }
        return false;
    }

    bool VDESManager::DeleteRouteRecommendationResponses(const std::vector<uint32_t> &dataIDs)
    {
        if (dataIDs.empty()) return true;
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexRouteRecommendationResponse);
                auto sqlCmd = fmt::format("DELETE FROM RouteRecommendationResponse WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlCmd);
                return true;
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteRouteRecommendationResponses_IDs");
            }
        }
        return false;
    }

    VDESManager::HydrometeorologyResponses VDESManager::GetHydrometeorologyResponses(const uint32_t index, const size_t number)
    {
        HydrometeorologyResponses container;
        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM HydrometeorologyResponse ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexHydrometeorologyResponse);
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
                while (query.executeStep())
                {
                    HydrometeorologyResponse response;
                    m_impl->LoadHydrometeorologyResponseFromQueryResult(response, query);
                    container.emplace_back(response);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetHydrometeorologyResponses");
            }
        }
        return container;
    }

    void VDESManager::DeleteHydrometeorologyResponses(const std::vector<uint32_t> &dataIDs)
    {
        if (dataIDs.empty()) return;
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexHydrometeorologyResponse);
                auto sqlCmd = fmt::format("DELETE FROM HydrometeorologyResponse WHERE ID IN ({})", fmt::join(dataIDs, ", "));
                m_impl->m_database->exec(sqlCmd);
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "DeleteHydrometeorologyResponses");
            }
        }
    }    bool VDESManager::SendRouteRecommendationRequest(const RouteRecommendationRequest &request)
    {
        if (!request.startCoordinate.IsValid() || !request.destCoordinate.IsValid())
        {
            return false;
        }

        AISBitsManager aisBitsManager;
        // DAC (10 bits) = 412
        aisBitsManager.Encode(412, 10);
        // FI (6 bits) = 45
        aisBitsManager.Encode(45, 6);

        // grossTonnage (30 bits)
        aisBitsManager.Encode(request.grossTonnage, 30);

        // maxStaticDraft (13 bits, unit 0.1m)
        uint16_t rawDraft = static_cast<uint16_t>(request.maxStaticDraft * 10);
        aisBitsManager.Encode(rawDraft, 13);

        // cargoType (8 bits)
        aisBitsManager.Encode(request.cargoType, 8);

        // Departure time: month (4), day (5), hour (5), minute (6)
        aisBitsManager.Encode(request.month & 0xF, 4);
        aisBitsManager.Encode(request.day & 0x1F, 5);
        aisBitsManager.Encode(request.hour & 0x1F, 5);
        aisBitsManager.Encode(request.minute & 0x3F, 6);

        // Start coordinate: lon (28), lat (27)
        int32_t startLon = static_cast<int32_t>(request.startCoordinate.GetLongitude() * 600000.0);
        int32_t startLat = static_cast<int32_t>(request.startCoordinate.GetLatitude() * 600000.0);
        aisBitsManager.Encode(UtilityInterface::ConvertIntegerToComplementCode(startLon, 28), 28);
        aisBitsManager.Encode(UtilityInterface::ConvertIntegerToComplementCode(startLat, 27), 27);

        // Dest coordinate: lon (28), lat (27)
        int32_t destLon = static_cast<int32_t>(request.destCoordinate.GetLongitude() * 600000.0);
        int32_t destLat = static_cast<int32_t>(request.destCoordinate.GetLatitude() * 600000.0);
        aisBitsManager.Encode(UtilityInterface::ConvertIntegerToComplementCode(destLon, 28), 28);
        aisBitsManager.Encode(UtilityInterface::ConvertIntegerToComplementCode(destLat, 27), 27);

        m_impl->m_sequenceNoAAB = (m_impl->m_sequenceNoAAB + 1) % 10;
        uint32_t seqNo = m_impl->m_sequenceNoAAB;

        auto payload = aisBitsManager.GetEncodedVDMPayload();
        auto fillBits = aisBitsManager.GetFillBitsNumberToEncode();

        std::string nmea = fmt::format("!AIAAB,1,1,{0},0,,004129999,,{1},{2},{3}", seqNo, 0, payload, fillBits);
        UtilityInterface::AddChecksum(nmea);
        nmea += "\r\n";

        m_impl->SaveRouteRecommendationRequest(request, seqNo);

        sendEvent(CommunicationType::TCP, nmea.c_str(), nmea.length());

        return true;
    }

    bool VDESManager::SendHydrometeorologyRequest(const HydrometeorologyRequest &request)
    {
        if (request.coordinates.size() > 10)
        {
            return false;
        }

        for (const auto &coord : request.coordinates)
        {
            if (!coord.IsValid())
            {
                return false;
            }
        }

        AISBitsManager aisBitsManager;
        aisBitsManager.Encode(412, 10);
        aisBitsManager.Encode(48, 6);
        aisBitsManager.Encode(request.MRN & 0x1FFFF, 17);

        uint16_t reqInfo = 0;
        if (request.windSpeed)     reqInfo |= (1 << 0);
        if (request.windDirection) reqInfo |= (1 << 1);
        if (request.visibility)    reqInfo |= (1 << 2);
        if (request.waveHeight)     reqInfo |= (1 << 3);
        if (request.waveDirection) reqInfo |= (1 << 4);
        if (request.swellHeight)    reqInfo |= (1 << 5);
        aisBitsManager.Encode(reqInfo & 0x3FFF, 14);

        tm timeUTC = {0};
        auto timestamp = request.requestTime;
        if (timestamp > 0)
        {
            timeUTC = *gmtime((time_t *)&timestamp);
            aisBitsManager.Encode((timeUTC.tm_mon + 1) & 0xF, 4);
            aisBitsManager.Encode(timeUTC.tm_mday & 0x1F, 5);
            aisBitsManager.Encode(timeUTC.tm_hour & 0x1F, 5);
            aisBitsManager.Encode(timeUTC.tm_min & 0x3F, 6);
        }
        else
        {
            aisBitsManager.Encode(0, 20);
        }

        uint8_t pointNum = static_cast<uint8_t>(request.coordinates.size());
        aisBitsManager.Encode(pointNum & 0xF, 4);

        for (const auto &coord : request.coordinates)
        {
            int32_t lon = static_cast<int32_t>(coord.GetLongitude() * 600000.0);
            int32_t lat = static_cast<int32_t>(coord.GetLatitude() * 600000.0);
            aisBitsManager.Encode(UtilityInterface::ConvertIntegerToComplementCode(lon, 28), 28);
            aisBitsManager.Encode(UtilityInterface::ConvertIntegerToComplementCode(lat, 27), 27);
        }

        m_impl->m_sequenceNoAAB = (m_impl->m_sequenceNoAAB + 1) % 10;
        uint32_t seqNo = m_impl->m_sequenceNoAAB;

        auto payload = aisBitsManager.GetEncodedVDMPayload();
        auto fillBits = aisBitsManager.GetFillBitsNumberToEncode();

        std::string nmea = fmt::format("!AIAAB,1,1,{0},0,,004129999,,{1},{2},{3}", seqNo, 0, payload, fillBits);
        UtilityInterface::AddChecksum(nmea);
        nmea += "\r\n";

        m_impl->SaveHydrometeorologyRequest(request, seqNo);

        sendEvent(CommunicationType::TCP, nmea.c_str(), nmea.length());

        return true;
    }

    VDESManager::RouteRecommendationRequests VDESManager::GetRouteRecommendationRequests(const uint32_t index, const size_t number)
    {
        RouteRecommendationRequests container;
        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM RouteRecommendationRequest ORDER BY [Timestamp Sent] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexRouteRecommendationResponse);
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
                while (query.executeStep())
                {
                    RouteRecommendationRequest request;
                    request.dataID = query.getColumn("ID").getUInt();
                    request.grossTonnage = query.getColumn("Gross Tonnage").getUInt();
                    request.maxStaticDraft = query.getColumn("Max Static Draft").getDouble();
                    request.cargoType = query.getColumn("Cargo Type").getUInt();
                    request.month = query.getColumn("Month").getUInt();
                    request.day = query.getColumn("Day").getUInt();
                    request.hour = query.getColumn("Hour").getUInt();
                    request.minute = query.getColumn("Minute").getUInt();
                    request.startCoordinate.SetLongitude(query.getColumn("Start Longitude").getDouble());
                    request.startCoordinate.SetLatitude(query.getColumn("Start Latitude").getDouble());
                    request.destCoordinate.SetLongitude(query.getColumn("Dest Longitude").getDouble());
                    request.destCoordinate.SetLatitude(query.getColumn("Dest Latitude").getDouble());
                    request.timestampSent = query.getColumn("Timestamp Sent").getInt64();
                    request.sequenceNum = query.getColumn("Sequence Num").getUInt();
                    request.sendStatus = query.getColumn("Send Status").getInt();
                    container.emplace_back(request);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetRouteRecommendationRequests");
            }
        }
        return container;
    }

    VDESManager::HydrometeorologyRequests VDESManager::GetHydrometeorologyRequests(const uint32_t index, const size_t number)
    {
        HydrometeorologyRequests container;
        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM HydrometeorologyRequest ORDER BY [Timestamp Sent] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexHydrometeorologyResponse);
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
                while (query.executeStep())
                {
                    HydrometeorologyRequest request;
                    request.dataID = query.getColumn("ID").getUInt();
                    request.MRN = query.getColumn("MRN").getUInt();
                    uint16_t requestedInfo = query.getColumn("Requested Info").getUInt();
                    request.requestTime = query.getColumn("Request Time").getInt64();
                    request.timestampSent = query.getColumn("Timestamp Sent").getInt64();
                    request.sequenceNum = query.getColumn("Sequence Num").getUInt();
                    request.sendStatus = query.getColumn("Send Status").getInt();

                    request.windSpeed = (requestedInfo & (1 << 0)) != 0;
                    request.windDirection = (requestedInfo & (1 << 1)) != 0;
                    request.visibility = (requestedInfo & (1 << 2)) != 0;
                    request.waveHeight = (requestedInfo & (1 << 3)) != 0;
                    request.waveDirection = (requestedInfo & (1 << 4)) != 0;
                    request.swellHeight = (requestedInfo & (1 << 5)) != 0;

                    SQLite::Column column = query.getColumn("Coordinates");
                    auto size = column.getBytes();
                    if (size > 0)
                    {
                        auto ptr = (const Impl::CoordinateStruct *)column.getBlob();
                        auto count = size / sizeof(Impl::CoordinateStruct);
                        request.coordinates.clear();
                        for (auto i = 0U; i < count; i++)
                        {
                            request.coordinates.push_back(Coordinate(ptr[i].latitude, ptr[i].longitude));
                        }
                    }
                    container.emplace_back(request);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetHydrometeorologyRequests");
            }
        }
        return container;
    }

    bool VDESManager::SetExtendedVesselInfo(const ExtendedVesselInfo &info)
    {
        std::lock_guard<std::mutex> lock(m_impl->m_mutexOwnExtendedVesselInfo);
        m_impl->SaveOwnExtendedVesselInfo(info);
        
        m_impl->BroadcastExtendedVesselInfo();

        auto &timerManager = TimerManager::GetInstance();
        if (TimerManager::IsTimerValid(m_impl->m_timerExtendedVesselBroadcast))
        {
            timerManager.RemoveTimer(m_impl->m_timerExtendedVesselBroadcast);
        }

        m_impl->m_timerExtendedVesselBroadcast = timerManager.AddTimer(180000, true, [this] {
            m_impl->BroadcastExtendedVesselInfo();
        });

        return true;
    }

    ExtendedVesselInfo VDESManager::GetExtendedVesselInfo(void) const
    {
        ExtendedVesselInfo info;
        if (m_impl->m_database)
        {
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexOwnExtendedVesselInfo);
                auto ownVessel = ConfigureManager::GetInstance().GetOwnVesselInfo();
                auto sql = fmt::format("SELECT * FROM OwnExtendedVesselInfo WHERE MMSI = {}", ownVessel.mmsi);
                SQLite::Statement query(*m_impl->m_database.get(), sql);
                if (query.executeStep())
                {
                    info.extendedVesselType = query.getColumn("Extended Vessel Type").getUInt();
                    info.autonomousLevel = query.getColumn("Autonomous Level").getUInt();
                    info.portCode = query.getColumn("Port Code").getText();
                    info.chineseName = query.getColumn("Chinese Name").getText();
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetExtendedVesselInfo");
            }
        }
        return info;
    }

    VDESManager::OtherVesselExtendedInfos VDESManager::GetOtherVesselExtendedInfos(const uint32_t index, const size_t number)
    {
        OtherVesselExtendedInfos container;
        if (m_impl->m_database)
        {
            auto sqlCmd = fmt::format("SELECT * FROM OtherVesselExtendedInfo ORDER BY [Timestamp Receive] "
                                      "DESC LIMIT {0} OFFSET {1}",
                                      (number == -1) ? "-1" : fmt::format("{}", number), index);
            try
            {
                std::lock_guard<std::mutex> lock(m_impl->m_mutexOtherVesselExtendedInfo);
                SQLite::Statement query(*m_impl->m_database.get(), sqlCmd);
                while (query.executeStep())
                {
                    OtherVesselExtendedInfo info;
                    m_impl->LoadOtherVesselExtendedInfoFromQueryResult(info, query);
                    container.emplace_back(info);
                }
            }
            catch (const SQLite::Exception &execption)
            {
                m_impl->DatabaseErrorProcess(execption, "GetOtherVesselExtendedInfos");
            }
        }
        return container;
    }
    void VDESManager::EmptyDatabase(void)
    {
        if (!m_impl->m_database)
        {
            return;
        }

        std::lock(m_impl->m_mutexMessageInbox, m_impl->m_mutexMessageOutbox, m_impl->m_mutexMSIObstacle,
            m_impl->m_mutexMSIMaritimeOperation, m_impl->m_mutexMSIMilitaryActivity,
            m_impl->m_mutexMSIMaritimeDistress, m_impl->m_mutexMSIDesignatedArea, m_impl->m_mutexBridge, m_impl->m_mutexChannelCenterline, m_impl->m_mutexChannelBoundary, m_impl->m_mutexNetSounder, m_impl->m_mutexFrontendPrompt, m_impl->m_mutexHydrometeorology,
            m_impl->m_mutexAbnormalShip, m_impl->m_mutexMarineMeteorologyFCST,
            m_impl->m_mutexMarineEnvironmentFCST, m_impl->m_mutexVTSRequest, m_impl->m_mutexVTSReply);

        std::unique_lock<std::mutex> lock1(m_impl->m_mutexMessageInbox, std::adopt_lock);
        std::unique_lock<std::mutex> lock2(m_impl->m_mutexMessageOutbox, std::adopt_lock);
        std::unique_lock<std::mutex> lock3(m_impl->m_mutexMSIObstacle, std::adopt_lock);
        std::unique_lock<std::mutex> lock4(m_impl->m_mutexMSIMaritimeOperation, std::adopt_lock);
        std::unique_lock<std::mutex> lock5(m_impl->m_mutexMSIMilitaryActivity, std::adopt_lock);
        std::unique_lock<std::mutex> lock6(m_impl->m_mutexMSIMaritimeDistress, std::adopt_lock);
        std::unique_lock<std::mutex> lock6_a(m_impl->m_mutexMSIDesignatedArea, std::adopt_lock);
        std::unique_lock<std::mutex> lock6_b(m_impl->m_mutexBridge, std::adopt_lock);
        std::unique_lock<std::mutex> lock6_c(m_impl->m_mutexChannelCenterline, std::adopt_lock);
        std::unique_lock<std::mutex> lock6_d(m_impl->m_mutexChannelBoundary, std::adopt_lock);
        std::unique_lock<std::mutex> lock6_e(m_impl->m_mutexNetSounder, std::adopt_lock);
        std::unique_lock<std::mutex> lock6_f(m_impl->m_mutexFrontendPrompt, std::adopt_lock);
        std::unique_lock<std::mutex> lock7(m_impl->m_mutexHydrometeorology, std::adopt_lock);
        std::unique_lock<std::mutex> lock8(m_impl->m_mutexAbnormalShip, std::adopt_lock);
        std::unique_lock<std::mutex> lock9(m_impl->m_mutexMarineMeteorologyFCST, std::adopt_lock);
        std::unique_lock<std::mutex> lock10(m_impl->m_mutexMarineEnvironmentFCST, std::adopt_lock);
        std::unique_lock<std::mutex> lock11(m_impl->m_mutexVTSRequest, std::adopt_lock);
        std::unique_lock<std::mutex> lock12(m_impl->m_mutexVTSReply, std::adopt_lock);

        // FIXME: we have to wait for some time to ensure that the database is closed.
        m_impl->m_database->close();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        // Delete the database file directly is the most efficient.
        auto storagePath = VDES::ConfigureManager::GetInstance().GetStoragePath();
        auto filePath = storagePath + "/VDES.db";
      
        auto ret = std::remove(filePath.c_str());
        if (!ret)
        {
            m_impl->InitializeDatabase();
            notifyEvent(EventType::DATABASE_EMPTY, 0);
        }

        m_impl->m_meteorologyAreas.clear();
        m_impl->m_environmentAreas.clear();
        m_impl->m_environmentAlongshores.clear();
        m_impl->m_bridges.clear();
    }
}
