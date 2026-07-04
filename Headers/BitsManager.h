#ifndef BITS_MANAGER_H_
#define BITS_MANAGER_H_

#include <memory>
#include <string>
#include <vector>

namespace VDES
{
    class BitsManager
    {
    public:
        BitsManager();

        virtual ~BitsManager();

        bool Encode(const uint32_t value, const uint32_t bitsNum);

        bool Encode(const uint32_t value, const uint32_t startBitPos, const uint32_t bitsNum);

        std::vector<uint32_t> &GetBitsEncoded(void) const;

        std::vector<uint8_t> GetBytesEncoded(const uint32_t startBitPos = 0, const int endBitPos = -1) const;

        void ResetEncoder(void);

        void RemoveTailBits(const uint32_t bitsNum);

        void RemoveBits(const uint32_t startBitPos, const uint32_t bitNum);

        uint32_t GetBitsNumberToDecode(void) const;

        int64_t DecodeToNumerical(const uint32_t startBitPos, const uint32_t bitNum) const;
    private:
        class Impl;

        std::unique_ptr<Impl> m_impl;
    };
}

#endif
