#ifndef VDES_H_
#define VDES_H_

#include "VDESDTD.h"
#include "eventpp/callbacklist.h"

#include <vector>
#include <memory>

namespace VDES
{
    class VDESManager
    {
    public:
        static VDESManager &GetInstance(void);

        void Parse(const char *data, uint32_t size);

        //std::vector<Message> GetMessages(void) const;

        //std::vector<AtoN> GetAtoNs(const BBox &bbox, const uint32_t maximum = 1000) const;

        //std::vector<AtoN> GetAtoNs(const uint32_t index, const uint32_t maximum) const;

        /*
        * @brief      Send message by the VDES termianl
        * @param[in]  mmsiSource  -  source addres
        * @param[in]  mmsiTarget  -  target address. if mmsiTarget == 0, send to all
        * @param[in]  sentence    -  message content
        * @param[out] none
        * @return     return true if invoke successfully
        */
        bool SendMessage(const uint32_t mmsiSource, const uint32_t mmsiTarget, const std::string &sentence);

        using SendEvent = eventpp::CallbackList<void(const std::string &sentece)>;

        SendEvent sendEvent;

        using MessageEvent = eventpp::CallbackList<void(const Message &message)>;

        MessageEvent messageEvent;

    private:
        VDESManager();

        ~VDESManager();

        VDESManager(const VDESManager &) = delete;

        VDESManager &operator=(const VDESManager &) = delete;

        friend class ConfigureManager;
    private:
        class Impl;
        using InstancePtr = std::unique_ptr<VDESManager>;
        using ImplPtr     = std::unique_ptr<Impl>;
        using ReceiveEvent = eventpp::CallbackList<void(const std::string &sentece)>;

        static InstancePtr instance;
        ImplPtr            m_impl;
        ReceiveEvent       m_receiveEvent;
    };
}

#endif
