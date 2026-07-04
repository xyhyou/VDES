#include "ByteArray.h"

#include <vector>

namespace VDES
{
    class ByteArray::Impl
    {
    public:
        Impl();

        ~Impl();
    public:
        std::vector<uint8_t> m_datas;
    };

    ByteArray::Impl::Impl()
    {

    }

    ByteArray::Impl::~Impl()
    {

    }

    ByteArray::ByteArray()
    {
        m_impl = std::unique_ptr<Impl>(new Impl());
    }

    ByteArray::ByteArray(const char *rawData, size_t size)
    {
        m_impl = std::unique_ptr<Impl>(new Impl());

        m_impl->m_datas.assign(rawData, rawData + size);
    }

    ByteArray::ByteArray(const std::string &rawData)
    {
        m_impl = std::unique_ptr<Impl>(new Impl());

        m_impl->m_datas.assign(rawData.begin(), rawData.end());
    }

    uint8_t &ByteArray::operator[](std::size_t index)
    {
        return m_impl->m_datas[index];
    }

    const uint8_t &ByteArray::operator[](std::size_t index) const
    {
        return m_impl->m_datas[index];
    }

    void ByteArray::Append(const char *rawData, size_t size)
    {
        m_impl->m_datas.insert(m_impl->m_datas.end(), rawData, rawData + size);
    }

    void ByteArray::Append(const char character, size_t count)
    {
        for (auto i = 0U; i < count; i++)
        {
            m_impl->m_datas.push_back(character);
        }
    }

    void ByteArray::Append(const std::string &str)
    {
        m_impl->m_datas.insert(m_impl->m_datas.end(), str.begin(), str.end());
    }

    size_t ByteArray::Size(void) const
    {
        return m_impl->m_datas.size();
    }

    const uint8_t *ByteArray::Data(void) const
    {
        return m_impl->m_datas.data();
    }

    ByteArray::~ByteArray()
    {

    }
}




