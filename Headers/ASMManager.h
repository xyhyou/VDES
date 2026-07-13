#ifndef ASM_MANAGER_H_
#define ASM_MANAGER_H_

#include "ASMDTD.h"
#include "eventpp/callbacklist.h"

#include <memory>
#include <string>
#include <cstdint>

namespace VDES
{
    class ASMManager
    {
    public:
        ASMManager();

        ~ASMManager();

        void Parse(const std::string &sentence);

        void ParsePayload(const std::string &payload, const uint8_t fillBitNum);

        void ParsePayload(const std::string &payload, const uint8_t fillBitNum, uint32_t srcMMSI, uint32_t destMMSI);

        using ASMPtr = std::shared_ptr<ASM_DAC_FI>;

        using ASMNotify = eventpp::CallbackList<void(ASMPtr)>;

        ASMNotify asmNotify;
    private:
        class Impl;

        std::unique_ptr<Impl> m_impl;
    };
}

#endif