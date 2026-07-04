#ifndef VDES_H_
#define VDES_H_

#include "VDESDTD.h"
#include "BoundingBox.h"
#include "eventpp/callbacklist.h"

#include <vector>
#include <memory>

namespace VDES
{
    class VDESManager
    {
    public:
        static VDESManager &GetInstance(void);

        void Initialize(void);

        /*
        * @brief      Check if the VDES manager is initialized
        * @param[in]  none
        * @param[out] none
        * @return     true if initialized, false otherwise
        */
        bool IsInitialized(void) const;

        void Parse(const char *data, uint32_t size);

        /*
        * @brief      Set the received relevant information to read or not.
        * @param[in]  type    -  information type
        * @param[in]  dataID  -  data ID
        * @param[in]  isRead  -  whether the message is read
        * @param[out] none
        * @return     none
        */
        void SetInformationRead(const InformationType type, const uint32_t  dataID, const bool isRead);

        /***********************************************************************
         * Message bussiness
         ***********************************************************************/
        using Messages = std::vector<Message>;

        enum class MailBox
        {
            INBOX,
            OUTBOX,
        };

        /*
        * @brief      Get messages
        * @param[in]  mailbox  -  INBOX or OUTBOX
        * @param[in]  index    -  start index
        * @param[in]  number   -  maximum number
        * @return     messages
        * @note       If the number is -1, get all messages after the starting position of index    
        */
        Messages GetMessages(const MailBox mailbox, const uint32_t index = 0,
                             const size_t number = -1) const;

        /*
        * @brief      Get messages
        * @param[in]  mmsi 
        * @param[out] none
        * @return     messages container
        * @note       Get all messages where the source address or destination 
        *             address is the specified mmsi
        */
        Messages GetMessages(const uint32_t mmsi);

        /*
        * @brief      Get unread message count
        * @param[in]  mmsi  -  the contract address
        * @param[out] none
        * @return     unread message number
        */
        uint32_t GetUnReadMessageCount(const uint32_t mmsi);

        /*
        * @brief      Reset unread message count
        * @param[in]  mmsi  -  the contract address
        * @param[out] none
        * @return     none
        */
        void ResetUnReadMessageCount(const uint32_t mmsi);

        /*
        * @brief      Delete messages
        * @param[in]  mmsi 
        * @param[out] none
        * @return     return true if invoke successfully
        * @note       Delete all messages where the source address or destination 
        *             address is the specified mmsi
        */
        bool DeleteMessages(const uint32_t mmsi);

        /*
        * @brief      Get contact addresses(mmsi)
        * @param[in]  mmsi  
        * @param[out] none
        * @return     the list of addresses
        * @note       Get all contact addresses(mmsi) that have exchanged messages 
        *             with the specified address(mmsi)
        */
        std::vector<uint32_t> GetContactAddresses(const uint32_t mmsi);

        /*
        * @brief      Get last interaction messages
        * @param[in]  mmsi 
        * @param[out] none
        * @return     messages container
        * @note       The parameter mmsi is the ownship's mmsi
        */
        Messages GetLastInteractionMessages(const uint32_t mmsi);

        /*
        * @brief      Delete messages
        * @param[in]  mailbox  -  INBOX or OUTBOX
        * @param[in]  index    -  start index
        * @param[in]  number   -  maximum number
        * @return     return true if invoke successfully.
        * @note       If the number is -1, then all messages after the starting position of index 
        *             will be deleted.
        */
        bool DeleteMessages(const MailBox mailbox, const uint32_t index, const size_t number = 1);

        /*
        * @brief      Delete messages
        * @param[in]  mailbox  -  INBOX or OUTBOX
        * @param[in]  dataIDs  -  message ID list
        * @return     return true if invoke successfully.
        */
        bool DeleteMessages(const MailBox mailbox, const std::vector<uint32_t> &dataIDs);

        /*
        * @brief      Send message by the VDES termianl
        * @param[in]  type        -  communication type 
        * @param[in]  mmsiSource  -  source addres
        * @param[in]  mmsiTarget  -  target address. if mmsiTarget == 0, send to all
        * @param[in]  content     -  message content
        * @param[in]  needReply   -  whether the message needs to be responded to.
        * @param[out] none
        * @return     return true if invoke successfully
        */
        bool SendInformation(const uint32_t mmsiSource, const uint32_t mmsiTarget, 
                             const std::string &content, const uint64_t deadline, 
                             const bool needReply = false);

        /*
        * @brief      Confirm message
        * @param[in]  dataID          -  message ID
        * @param[in]  isMessageValid  -  whether the message is valid
        * @param[out] none
        * @return     none
        */
        void ConfirmMessage(const uint32_t dataID, const bool isMessageValid);

        enum class AISMessageType
        {
            MESSAGE_NONE = 0,
            MESSAGE_6    = 6,
            MESSAGE_8    = 8,
            MESSAGE_12   = 12,
            MESSAGE_14   = 14
        };

        /*
        * @brief      Send AIS message
        * @param[in]  type            -  message type
        * @param[in]  mmsiDestination -  destination mmsi
        * @param[in]  content         -  message content
        * @param[out] none
        * @return     none
        * @note       If need to send a broadcast message, mmsi = 0
        */
        void SendAISMessage(const AISMessageType type, const uint32_t mmsiDestination, const std::string &content);

        /*
        * @brief      Send Route Recommendation Request
        * @param[in]  request - the route recommendation request DTO
        * @return     true if validation passes and sentence is dispatched, false otherwise
        */
        bool SendRouteRecommendationRequest(const RouteRecommendationRequest &request);

        /*
        * @brief      Send Hydrometeorological Request
        * @param[in]  request - the hydrometeorological request DTO
        * @return     true if validation passes and sentence is dispatched, false otherwise
        */
        bool SendHydrometeorologyRequest(const HydrometeorologyRequest &request);

        /***********************************************************************
        * AtoN
        ***********************************************************************/
        using AtoNs = std::vector<AtoN>;

        using AtoNPtr = std::shared_ptr<AtoN>;

        /*
        * @brief      Get Aids-to-navigation station
        * @param[in]  index   -  the starting position obtained 
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the AtoN container.
        */
        AtoNs GetAtoNs(const uint32_t index = 0, const size_t number = -1);
        
        /*
        * @brief      Get Aids-to-navigation station
        * @param[in]  bbox    -  geographical bounding box of the visible area
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the AtoN container.
        * @note       if number = -1, get all AtoN in the boundingbox.
        */
        AtoNs GetAtoNs(const BoundingBox &bbox, const size_t number = -1);

        /*
        * @brief      Get Aids-to-navigation station
        * @param[in]  latitude   -  the latitude of the cursor
        * @param[in]  longitude  -  the longitude of the cursor
        * @param[in]  radius     -  the radius of picking
        * @param[out] none
        * @return     return the AtoN object.
        * @note       This interface is mainly used for picking AtoN on nautical charts 
        */
        AtoNPtr GetAtoN(const double latitude, const double longitude, const double radius);

        /*
        * @brief      Get Aids-to-navigation station
        * @param[in]  mmsi   -  the AtoN's mmsi.
        * @param[out] none
        * @return     return the AtoN object.
        */
        AtoNPtr GetAtoN(const uint32_t mmsi);

        /*
        * @brief      Delete Aids-to-navigation station
        * @param[in]  mmsiSet  -  the set of mmsi which will be deleted
        * @param[out] none
        * @return     return true if invoke successfully
        */
        bool DeleteAtoNs(const std::vector<uint32_t> &mmsiSet);

        /***********************************************************************
        * Maritime Safety Information - Overview
        ***********************************************************************/
        using MSIInfos = std::vector<MSIInfo>;

        /*
        * @brief      Get maritime safety information overview
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        */
        MSIInfos GetMSIOverviews(const uint32_t index = 0, const size_t number = -1);
        
        /***********************************************************************
        * Maritime Safety Information - Obstacle
        ***********************************************************************/
        using Obstacles = std::vector<MSIObstacle>;

        using ObstaclePtr = std::shared_ptr<MSIObstacle>;

        /*
        * @brief      Get MSI Obstacles 
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the obstacle container.
        * @note       If the number is -1, get all obstacles after the starting position of index
        */
        Obstacles GetObstacles(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Get MSI Obstacles 
        * @param[in]  bbox    -  geographical bounding box of the visible area
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the obstacle container.
        * @note       if number = -1,  get all obstacles in the boundingbox.
        */
        Obstacles GetObstacles(const BoundingBox &bbox, const size_t number = -1);

        /*
        * @brief      Get MSI Obstacles 
        * @param[in]  latitude   -  the latitude of the cursor
        * @param[in]  longitude  -  the longitude of the cursor
        * @param[in]  radius     -  the radius of picking
        * @param[out] none
        * @return     return the obstacle object.
        * @note       This interface is mainly used for picking obstacle on nautical charts
        */
        ObstaclePtr GetObstacle(const double latitude, const double longitude, const double radius);

        /*
        * @brief      Get MSI Obstacles 
        * @param[in]  dataID
        * @param[out] none
        * @return     return the obstacle object.
        */
        ObstaclePtr GetObstacle(const uint32_t dataID);

        /*
        * @brief      Delete one MSI Obstacle
        * @param[in]  dataID
        * @param[out] none
        * @return     return true if invoke successfully
        */
        bool DeleteObstacle(const uint32_t dataID);

        /*
        * @brief      Delete MSI Obstacles
        * @param[in]  dataIDs
        * @param[out] none
        * @return     reutrn true if invoke successfully
        */
        bool DeleteObstacles(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * Maritime Safety Information - Maritime operation
        ***********************************************************************/
        using MaritimeOperations = std::vector<MSIMaritimeOperation>;

        using MaritimeOperationPtr = std::shared_ptr<MSIMaritimeOperation>;

        /*
        * @brief      Get maritime operation information
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       If the number is -1, get all operations after the starting position of index
        */
        MaritimeOperations GetMaritimeOperations(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Get maritime operation information
        * @param[in]  bbox    -  geographical bounding box of the visible area
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if number = -1,  get all operations in the boundingbox.
        */
        MaritimeOperations GetMaritimeOperations(const BoundingBox &bbox, const size_t number = -1);

        /*
        * @brief      Get maritime operation information
        * @param[in]  latitude   -  the latitude of the cursor
        * @param[in]  longitude  -  the longitude of the cursor
        * @param[in]  radius     -  the radius of picking
        * @param[out] none
        * @return     return the object.
        * @note       This interface is mainly used for picking operation on nautical charts
        */
        MaritimeOperationPtr GetMaritimeOperation(const double latitude, const double longitude, 
                                                  const double radius);

        /*
        * @brief      Get maritime operation information
        * @param[in]  dataID
        * @param[out] none
        * @return     return the object.
        */
        MaritimeOperationPtr GetMaritimeOperation(const uint32_t dataID);

        /*
        * @brief      Delete one maritime operation
        * @param[in]  dataID
        * @param[out] none
        * @return     return true if invoke successfully
        */
        bool DeleteMaritimeOperation(const uint32_t dataID);

        /*
        * @brief      Delete maritime operations
        * @param[in]  dataIDs
        * @param[out] none
        * @return     reutrn true if invoke successfully
        */
        bool DeleteMaritimeOperations(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * Maritime Safety Information - Maritime towing
        ***********************************************************************/
        using MaritimeTowings = std::vector<MSIMaritimeTowing>;

        using MaritimeTowingPtr = std::shared_ptr<MSIMaritimeTowing>;

        /*
        * @brief      Get maritime towing information
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @return     return the container.
        */
        MaritimeTowings GetMaritimeTowings(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Get maritime towing information
        * @param[in]  bbox    -  geographical bounding box of the visible area
        * @param[in]  number  -  the maximum number obtained
        * @return     return the container.
        */
        MaritimeTowings GetMaritimeTowings(const BoundingBox &bbox, const size_t number = -1);

        /*
        * @brief      Get maritime towing information
        * @param[in]  latitude   -  the latitude of the cursor
        * @param[in]  longitude  -  the longitude of the cursor
        * @param[in]  radius     -  the radius of picking
        * @return     return the object.
        */
        MaritimeTowingPtr GetMaritimeTowing(const double latitude, const double longitude,
                                            const double radius);

        /*
        * @brief      Get maritime towing information
        * @param[in]  dataID
        * @return     return the object.
        */
        MaritimeTowingPtr GetMaritimeTowing(const uint32_t dataID);

        /*
        * @brief      Delete one maritime towing
        * @param[in]  dataID
        * @return     return true if invoke successfully
        */
        bool DeleteMaritimeTowing(const uint32_t dataID);

        /*
        * @brief      Delete maritime towings
        * @param[in]  dataIDs
        * @return     reutrn true if invoke successfully
        */
        bool DeleteMaritimeTowings(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * Maritime Safety Information - Military activity
        ***********************************************************************/
        using MilitaryActivitys = std::vector<MSIMilitaryActivity>;

        using MilitaryActivityPtr = std::shared_ptr<MSIMilitaryActivity>;

        /*
        * @brief      Get military activity information
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       If the number is -1, get all operations after the starting position of index
        */
        MilitaryActivitys GetMilitaryActivitys(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Get military activity information
        * @param[in]  bbox    -  geographical bounding box of the visible area
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if number = -1,  get all operations in the boundingbox.
        */
        MilitaryActivitys GetMilitaryActivitys(const BoundingBox &bbox, const size_t number = -1);

        /*
        * @brief      Get military activity information
        * @param[in]  latitude   -  the latitude of the cursor
        * @param[in]  longitude  -  the longitude of the cursor
        * @param[in]  radius     -  the radius of picking
        * @param[out] none
        * @return     return the object.
        * @note       This interface is mainly used for picking operation on nautical charts
        */
        MilitaryActivityPtr GetMilitaryActivity(const double latitude, const double longitude,
                                                const double radius);

        /*
        * @brief      Get military activity information
        * @param[in]  dataID
        * @param[out] none
        * @return     return the object.
        */
        MilitaryActivityPtr GetMilitaryActivity(const uint32_t dataID);

        /*
        * @brief      Delete one military activity
        * @param[in]  dataID
        * @param[out] none
        * @return     return true if invoke successfully
        */
        bool DeleteMilitaryActivity(const uint32_t dataID);

        /*
        * @brief      Delete military activities
        * @param[in]  dataIDs
        * @param[out] none
        * @return     reutrn true if invoke successfully
        */
        bool DeleteMilitaryActivitys(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * Maritime Safety Information - Maritime distress
        ***********************************************************************/
        using MaritimeDistresses = std::vector<MSIMaritimeDistress>;

        using MaritimeDistressPtr = std::shared_ptr<MSIMaritimeDistress>;

        using DesignatedAreas = std::vector<MSIDesignatedArea>;

        using DesignatedAreaPtr = std::shared_ptr<MSIDesignatedArea>;

        /*
        * @brief      Get maritime distress information
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       If the number is -1, get all distresses after the starting position of index
        */
        MaritimeDistresses GetMaritimeDistresses(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Get maritime distress information
        * @param[in]  bbox    -  geographical bounding box of the visible area
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if number = -1,  get all distresses in the boundingbox.
        */
        MaritimeDistresses GetMaritimeDistresses(const BoundingBox &bbox, const size_t number = -1);

        /*
        * @brief      Get maritime distress information
        * @param[in]  latitude   -  the latitude of the cursor
        * @param[in]  longitude  -  the longitude of the cursor
        * @param[in]  radius     -  the radius of picking
        * @param[out] none
        * @return     return the object.
        * @note       This interface is mainly used for picking on nautical charts
        */
        MaritimeDistressPtr GetMaritimeDistress(const double latitude, const double longitude,
                                                const double radius);

        /*
        * @brief      Get maritime distress information
        * @param[in]  dataID
        * @param[out] none
        * @return     return the object.
        */
        MaritimeDistressPtr GetMaritimeDistress(const uint32_t dataID);

        /*
        * @brief      Delete one maritime distress
        * @param[in]  dataID
        * @param[out] none
        * @return     return true if invoke successfully
        */
        bool DeleteMaritimeDistress(const uint32_t dataID);

        /*
        * @brief      Delete maritime distresses
        * @param[in]  dataIDs
        * @param[out] none
        * @return     reutrn true if invoke successfully
        */
        bool DeleteMaritimeDistresses(const std::vector<uint32_t> &dataIDs);

        DesignatedAreas GetDesignatedAreas(const uint32_t index = 0, const size_t number = -1);
        DesignatedAreas GetDesignatedAreas(const BoundingBox &bbox, const size_t number = -1);
        DesignatedAreaPtr GetDesignatedArea(const double latitude, const double longitude, const double radius);
        DesignatedAreaPtr GetDesignatedArea(const uint32_t dataID);
        bool DeleteDesignatedArea(const uint32_t dataID);

        /***********************************************************************
        * Hydrological and meteorological Information
        ***********************************************************************/
        using Hydrometeorologies = std::vector<Hydrometeorology>;

        using HydrometeorologyPtr = std::shared_ptr<Hydrometeorology>;

        /*
        * @brief      Get Hydrological and meteorological
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if the number is -1, get all hydrometeorology after the starting position of index
        */
        Hydrometeorologies GetHydrometeorology(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Get Hydrological and meteorological
        * @param[in]  dataID         -  the ID of the forecast point data.
        * @param[in]  timestampStart -  the start timestamp
        * @param[in]  timestampEnd   -  the end timestamp
        * @param[out] none
        * @return     return the container.
        * @note       if dataID = 0, get all hydrometeorologies between timestampStart and timestampEnd.
        */
        Hydrometeorologies GetHydrometeorology(const uint32_t dataID, const uint64_t timestampStart, 
            const uint64_t timestampEnd);

        /*
        * @brief      Get Hydrological and meteorological
        * @param[in]  bbox    -  geographical bounding box of the visible area
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if number = -1,  get all hydrometeorology in the boundingbox.
        */
        Hydrometeorologies GetHydrometeorology(const BoundingBox &bbox, const size_t number = -1);

        /*
        * @brief      Pickup the Hydrological and meteorological
        * @param[in]  latitude   -  the latitude of the cursor
        * @param[in]  longitude  -  the longitude of the cursor
        * @param[in]  radius     -  the radius of picking
        * @param[out] none
        * @return     return the container.
        * @note       This interface is mainly used for picking on nautical charts.           
        */
        Hydrometeorologies GetHydrometeorology(const double latitude, const double longitude,
            const double radius);

        /*
        * @brief      Delete one Hydrological and meteorological
        * @param[in]  dataID
        * @param[out] none
        * @return     return true if invoke successfully
        */
        bool DeleteHydrometeorology(const uint32_t dataID);

        /*
        * @brief      Delete Hydrological and meteorological
        * @param[in]  dataIDs
        * @param[out] none
        * @return     reutrn true if invoke successfully
        */
        bool DeleteHydrometeorologies(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * Abnormal ship Information
        ***********************************************************************/
        using AbnormalShips = std::vector<AbnormalShip>;

        using AbnormalShipPtr = std::shared_ptr<AbnormalShip>;
        
        /*
        * @brief      Get abnormal ship information
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if the number is -1, get all abnormal ship after the starting position of index
        */
        AbnormalShips GetAbnormalShips(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Get abnormal ship information
        * @param[in]  mmsi  -  the mmsi of the ship
        * @param[out] none
        * @return     return the container.
        */
        AbnormalShips GetAbnormalShipsByMMSI(const uint32_t mmsi);

        /*
        * @brief      Check whether the ship is abnormal
        * @param[in]  mmsi
        * @param[out] none
        * @return     return true if the ship is abnormal, otherwise return false
        */
        bool IsAbnormalShip(const uint32_t mmsi);

        /*
        * @brief      Get abnormal ship information
        * @param[in]  dataID  -  the data ID of the ship
        * @param[out] none
        * @return     return the object.
        */
        AbnormalShipPtr GetAbnormalShip(const uint32_t dataID);

        /*
        * @brief      Delete abnormal ship information
        * @param[in]  dataID  -  the data ID of the ship
        * @param[out] none
        * @return     return true if delete successfully, otherwise return false.
        */
        bool DeleteAbnormalShip(const uint32_t dataID);

        /*
        * @brief      Delete abnormal ship information
        * @param[in]  dataIDs  -  the set of data ID which will be deleted
        * @param[out] none
        * @return     return true if delete successfully, otherwise return false.
        */
        bool DeleteAbnormalShips(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * VTS
        ***********************************************************************/
        /*
        * @brief      Send request to VTS
        * @param[in]  mmsiVTS      -  the mmsi of the VTS
        * @param[in]  requestType  -  the request type
        * @param[out] none
        * @return     none
        * @note       requestType value:
        *             1 - online
        *             2 - offline
        *             3 - moor
        *             4 - departure
        *             5 - enter the gate
        *             6 - exit the gate
        *             7 - weigh anchor
        *             8 - drop anchor
        */
        void SendRequestToVTS(const uint32_t mmsiVTS, const uint8_t requestType);

        /*
        * @brief      Send the confirmation result to VTS.
        * @param[in]  requestDataID -  the data ID of the request
        * @param[in]  mmsiVTS       -  the mmsi of the VTS
        * @param[in]  canExecute    -  determine whether ownship can execute the VTS's route.
        * @param[in]  chineseRemark -  the chinese remark of the remark for the VTS
        * @param[in]  englishRemark -  the english remark of the remark for the VTS
        * @param[out] none
        * @return     none
        */
        void SendConfirmationToVTS(const uint32_t requestDataID, const uint32_t mmsiVTS, 
                                   const bool canExecute, const std::string &chineseRemark,
                                   const std::string &englishRemark);

        using VTSRequestPtr = std::shared_ptr<VTSRequest>;
        using VTSRequests = std::vector<VTSRequest>;

        /*
        * @brief      Get the VTS requests
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if the number is -1, get all VTS requests after the starting position of index
        */
        VTSRequests GetVTSRequests(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Get the VTS request by the data ID
        * @param[in]  dataID  -  the data ID of the request
        * @param[out] none
        * @return     return the information if the request is found, otherwise return nullptr
        */
        VTSRequestPtr GetVTSRequest(const uint32_t dataID);
        
        
        using VTSReplyPtr = std::shared_ptr<VTSReply>;

        /*
        * @brief      Get the VTS reply by the data ID of the request
        * @param[in]  requestDataID  -  the data ID of the request
        * @param[out] none
        * @return     return the information if the reply is found, otherwise return nullptr
        */
        VTSReplyPtr GetVTSReply(const uint32_t requestDataID);

        using HydrometeorologyRequests = std::vector<HydrometeorologyRequest>;
        using RouteRecommendationRequests = std::vector<RouteRecommendationRequest>;

        /*
        * @brief      Get the Hydrometeorology requests history
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @return     return the container.
        */
        HydrometeorologyRequests GetHydrometeorologyRequests(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Get the Route Recommendation requests history
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @return     return the container.
        */
        RouteRecommendationRequests GetRouteRecommendationRequests(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Delete the VTS requests
        * @param[in]  dataIDs  -  the set of data ID which will be deleted
        * @param[out] none
        * @return     none
        */
        void DeleteVTSRequests(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * ASM - Marine Meteorological Forecast
        ***********************************************************************/
        using MarineMeteorologyFCSTs = std::vector<MarineMeteorologyFCST>;
       
        /*
        * @brief      Get the marine meteorological forecast information
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if the number is -1, get all forecast after the starting position of index
        */
        MarineMeteorologyFCSTs GetMarineMeteorologyFCSTs(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief	  Get the marine meteorological forecast informations of the specified location
        * @param[in]  coordinate  -  location queried
        * @param[in]  range	      -  the radius of the queried location
        * @param[out] none
        * @return	  return all forecast information for that location
        */
        MarineMeteorologyFCSTs GetMarineMeteorologyFCSTs(const Coordinate &coordinate, const double radius);
        
        /*
        * @brief      Get the marine meteorological forecast information
        * @param[in]  boundingBox -  the bounding box
        * @param[in]  number      -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if the number is -1, get all informations in the bounding box
        */
        MarineMeteorologyFCSTs GetMarineMeteorologyFCSTs(const BoundingBox &boundingBox, 
            const size_t number = -1);

        /*
        * @brief      Delete the marine meteorological forecast information
        * @param[in]  index   -  the starting position to be deleted
        * @param[in]  number  -  the maximum number information to be deleted
        * @param[out] none
        * @return     true if delete successfully, otherwise false
        * @note       if the number is -1, delete all forecast after the starting 
        *             position of index           
        */
        bool DeleteMarineMeteorologyFCSTs(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Delete the marine meteorological forecast information
        * @param[in]  dataIDs  -  collection of dataIDs to be deleted
        * @param[out] none
        * @return     true if delete successfully, otherwise false
        */
        bool DeleteMarineMeteorologyFCSTs(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * ASM - Marine Environment Forecast
        ***********************************************************************/
        using MarineEnvironmentFCSTs = std::vector<MarineEnvironmentFCST>;

        /*
        * @brief      Get the marine enviroment forecast information
        * @param[in]  index   -  the starting position obtained
        * @param[in]  number  -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if the number is -1, get all forecast after the starting position of index
        */
        MarineEnvironmentFCSTs GetMarineEnvironmentFCSTs(const uint32_t index = 0, const size_t number = -1);

        /**
        * @brief	  Get the marine enviroment forecast informations of the specified location
        * @param[in]  coordinate  -  location queried
        * @param[in]  range	      -  the radius of the queried location
        * @param[out] none
        * @return	  Return all forecast information for that location
        */
        MarineEnvironmentFCSTs GetMarineEnvironmentFCSTs(const Coordinate &coordinate, const double radius);

        /*
        * @brief      Get the marine enviroment forecast information
        * @param[in]  bbox   -  the bounding box
        * @param[in]  number -  the maximum number obtained
        * @param[out] none
        * @return     return the container.
        * @note       if the number is -1, get all informations in the bounding box.
        */
        MarineEnvironmentFCSTs GetMarineEnvironmentFCSTs(const BoundingBox &bbox, const size_t number = -1);

        /*
        * @brief      Delete the marine enviroment forecast information
        * @param[in]  index   -  the starting position to be deleted
        * @param[in]  number  -  the maximum number information to be deleted
        * @param[out] none
        * @return     true if delete successfully, otherwise false
        * @note       if the number is -1, delete all forecast after the starting position of index
        */
        bool DeleteMarineEnvironmentFCSTs(const uint32_t index = 0, const size_t number = -1);

        /*
        * @brief      Delete the marine enviroment forecast information
        * @param[in]  dataIDs  -  collection of dataIDs to be deleted
        * @param[out] none
        * @return     true if delete successfully, otherwise false
        */
        bool DeleteMarineEnvironmentFCSTs(const std::vector<uint32_t> &dataIDs);


        /***********************************************************************
        * ASM - Marine Meteorology Forecast (Sea area)
        ***********************************************************************/
        using MarineMeteorologyFCSTAreas = std::vector<MarineMeteorologyFCSTArea>;
        
        MarineMeteorologyFCSTAreas GetMarineMeteorologyFCSTAreas(const uint32_t index = 0, const size_t number = -1);

        /***********************************************************************
        * ASM - Marine Environment Forecast (Sea area)
        ***********************************************************************/
        using MarineEnvironmentFCSTAreas = std::vector<MarineEnvironmentFCSTArea>;

        MarineEnvironmentFCSTAreas GetMarineEnvironmentFCSTAreas(const uint32_t index = 0, const size_t number = -1);


        using MarineEnvironmentFCSTAlongshores = std::vector<MarineEnvironmentFCSTAlongshore>;

        MarineEnvironmentFCSTAlongshores GetMarineEnvironmentFCSTAlongshores(const uint32_t index = 0, const size_t number = -1);


        /***********************************************************************
        * ASM - Bridge
        ***********************************************************************/
        using Bridges = std::vector<Bridge>;
        
        Bridges GetBridges(const uint32_t index = 0, const size_t number = -1);


        /***********************************************************************
        * ASM - Net Sounder
        ***********************************************************************/
        using NetSounders = std::vector<NetSounder>;
        
        NetSounders GetNetSounders(const uint32_t index = 0, const size_t number = -1);


        /***********************************************************************
        * ASM - Channel Centerline
        ***********************************************************************/
        using ChannelCenterlines = std::vector<ChannelCenterline>;
        
        ChannelCenterlines GetChannelCenterlines(const uint32_t index = 0, const size_t number = -1);


        /***********************************************************************
        * ASM - Channel Boundary
        ***********************************************************************/
        using ChannelBoundaries = std::vector<ChannelBoundary>;
        
        ChannelBoundaries GetChannelBoundaries(const uint32_t index = 0, const size_t number = -1);

        /***********************************************************************
        * ASM - Frontend Prompt Text (前端提示文字)
        ***********************************************************************/
        using FrontendPrompts = std::vector<FrontendPrompt>;

        FrontendPrompts GetFrontendPrompts(const uint32_t index = 0, const size_t number = -1);

        bool DeleteFrontendPrompts(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * ASM - Marine Meteorology & Environmental Warning
        ***********************************************************************/
        using MarineMeteorologyEWs = std::vector<MarineMeteorologyEW>;

        MarineMeteorologyEWs GetMarineMeteorologyEWs(const uint32_t index = 0, const size_t number = -1);

        /***********************************************************************
        * ASM - Tide Forecast
        ***********************************************************************/
        using TideForecasts = std::vector<TideForecast>;

        TideForecasts GetTideForecasts(const uint32_t index = 0, const size_t number = -1);

        /***********************************************************************
        * ASM - AtoN Dynamics (non-AIS AtoN)
        ***********************************************************************/
        using AtoNDynamicsList = std::vector<AtoNDynamics>;

        AtoNDynamicsList GetAtoNDynamics(const uint32_t index = 0, const size_t number = -1);

        /***********************************************************************
        * ASM - AIS AtoN Dynamics (AIS AtoN)
        ***********************************************************************/
        using AISAtoNDynamicsList = std::vector<AISAtoNDynamics>;

        AISAtoNDynamicsList GetAISAtoNDynamics(const uint32_t index = 0, const size_t number = -1);

        /***********************************************************************
        * ASM - Route Recommendation Response
        ***********************************************************************/
        using RouteRecommendationResponses = std::vector<RouteRecommendationResponse>;
        using RouteRecommendationResponsePtr = std::shared_ptr<RouteRecommendationResponse>;

        RouteRecommendationResponses GetRouteRecommendationResponses(const uint32_t index = 0, const size_t number = -1);
        bool DeleteRouteRecommendationResponses(const uint32_t index = 0, const size_t number = -1);
        bool DeleteRouteRecommendationResponses(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * ASM - Hydrometeorological Response
        ***********************************************************************/
        using HydrometeorologyResponses = std::vector<HydrometeorologyResponse>;
        using HydrometeorologyResponsePtr = std::shared_ptr<HydrometeorologyResponse>;

        HydrometeorologyResponses GetHydrometeorologyResponses(const uint32_t index = 0, const size_t number = -1);
        void DeleteHydrometeorologyResponses(const std::vector<uint32_t> &dataIDs);

        /***********************************************************************
        * ASM - Extended static and voyage information
        ***********************************************************************/
        using OtherVesselExtendedInfos = std::vector<OtherVesselExtendedInfo>;
        using OtherVesselExtendedInfoPtr = std::shared_ptr<OtherVesselExtendedInfo>;

        bool SetExtendedVesselInfo(const ExtendedVesselInfo &info);
        ExtendedVesselInfo GetExtendedVesselInfo(void) const;

        OtherVesselExtendedInfos GetOtherVesselExtendedInfos(const uint32_t index = 0, const size_t number = -1);

        /**********************************************************************/

        /*
        * @brief      Empty the database
        * @param[in]  none
        * @param[out] none
        * @return     none
        */
        void EmptyDatabase(void);

        /***********************************************************************
        * Event
        ***********************************************************************/
        using SendEvent = eventpp::CallbackList<void(const CommunicationType type, 
            const char *data, size_t size)>;

        SendEvent sendEvent;

        enum class EventType
        {
            /*
            * Event ret code value:
            * 0 - Normal message
            * 1 - Addressed Application Message Acknowledgment
            */
            MESSAGE_RECEIVE,

            /*
            * Event ret code value:
            * -1 - No terminal response received.
            *  0 - Broadcast successful
            *  1 - The message was successfully sent and a reply was received from
            *      the target node (addressing was successful).
            *  2 - Failed to send.
            *  3 - The sending queue is full. Please try again later.
            *  4 - Radio link related time-out
            *  5 - Data length exceeds the limit
            *  6 - Refused service
            */
            MESSAGE_SEND,

            ATON,

            /*
            * Maritime Safety Information - Obstacle
            */
            MSI_OBSTACLE,

            /*
            * Maritime Safety Information - Maritime operation
            */
            MSI_MARITIME_OPERATION,

            /*
            * Maritime Safety Information - Military activity
            */
            MSI_MILITARY_ACTIVITY,

            /*
            * Maritime Safety Information - Maritime distress
            */
            MSI_MARITIME_DISTRESS,

            /*
            * Maritime Safety Information - Designated Area
            */
            MSI_DESIGNATED_AREA,

            /*
            * Hydrological and meteorological Information
            */
            HYDROLOGICAL_METEOROLOGICAL,

            ABNORMAL_SHIP,

            /*
            * Marine meteorological forecast 
            */
            ASM_MARINE_METEOROLOGY_FCST,

            /*
            * Marine enviroment forecast
            */
            ASM_MARINE_ENVIRONMENT_FCST,

            DATABASE_EMPTY,

            /************************ New *************************/

            ASM_MARINE_METEOROLOGY_FCST_AREA,

            ASM_MARINE_ENVIRONMENT_FCST_AREA,

            ASM_MARINE_ENVIRONMENT_FCST_ALONGSHORE,

            ASM_BRIDGE,

            ASM_NET_SOUNDER,

            ASM_MARINE_METEOROLOGY_EW,

            ASM_TIDE_FCST,

            ASM_ATON_DYNAMICS,

            ASM_AIS_ATON_DYNAMICS,

            ASM_MARITIME_TOWING,

            ASM_CHANNEL_CENTERLINE,

            ASM_CHANNEL_BOUNDARY,

            ASM_FRONTEND_PROMPT,

            ASM_ROUTE_RECOMMENDATION_RESPONSE,

            ASM_HYDROMETEOROLOGY_RESPONSE,

            ASM_EXTENDED_VESSEL_A,

            ASM_EXTENDED_VESSEL_B,
        };

        using NotifyEvent = eventpp::CallbackList<void(const EventType, const int retCode)>;

        NotifyEvent notifyEvent;

        using VTSReplyEvent = eventpp::CallbackList<void(const VTSReply reply)>;

        VTSReplyEvent vtsReplyEvent;

    private:
        VDESManager();

        ~VDESManager();

        VDESManager(const VDESManager &) = delete;

        VDESManager &operator=(const VDESManager &) = delete;

        friend class ConfigureManager;
    private:
        class Impl;
        using InstancePtr  = std::unique_ptr<VDESManager>;
        using ImplPtr      = std::unique_ptr<Impl>;
        using ReceiveEvent = eventpp::CallbackList<void(const std::string &sentece)>;

        static InstancePtr instance;
        ImplPtr            m_impl;
        ReceiveEvent       m_receiveEvent;
    };
}

#endif
