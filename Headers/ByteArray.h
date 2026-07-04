#ifndef BYTE_ARRAY_H_
#define BYTE_ARRAY_H_

#include <memory>
#include <string>

namespace VDES
{
    class ByteArray
    {
    public:
        ByteArray();

        ByteArray(const char *rawData, size_t size);

        ByteArray(const std::string &str);

        ~ByteArray();

        uint8_t &operator[](std::size_t index);

        const uint8_t &operator[](std::size_t index) const;

        void Append(const char *rawData, size_t size);

        void Append(const char character, size_t count);

        void Append(const std::string &str);

        size_t Size(void) const;

        const uint8_t *Data(void) const;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}

#endif