#include "Coordinate.h"

namespace VDES
{
    class Coordinate::Impl
    {
    public:
        Impl(const double latitude = 91.0F, const double longitude = 181.0F);

        ~Impl();
    public:
        double m_latitude;
        double m_longitude;
    };

    Coordinate::Impl::Impl(const double latitude, const double longitude)
    {
        m_latitude = latitude;
        m_longitude = longitude;
    }

    Coordinate::Impl::~Impl()
    {

    }

    Coordinate::Coordinate()
    {
        m_impl = std::unique_ptr<Impl>(new Impl());
    }

    Coordinate::Coordinate(const double latitude, const double longitude)
    {
        m_impl = std::unique_ptr<Impl>(new Impl(latitude, longitude));
    }

    Coordinate::Coordinate(const Coordinate &coordinate)
    {
        if (coordinate.m_impl)
        {
            m_impl = std::unique_ptr<Impl>(new Impl(coordinate.m_impl->m_latitude, 
                coordinate.m_impl->m_longitude));
        }
        else
        {
            m_impl = std::unique_ptr<Impl>(new Impl());
        }
    }

    Coordinate::Coordinate(Coordinate &&coordinate) noexcept
    {
        m_impl = std::move(coordinate.m_impl);
    }

    Coordinate::~Coordinate()
    {

    }

    Coordinate &Coordinate::operator=(const Coordinate &coordinate)
    {
        if (this == &coordinate)
        {
            return *this;
        }

        if (coordinate.m_impl)
        {
            if (!m_impl)
            {
                m_impl = std::unique_ptr<Impl>(new Impl());
            }
            m_impl->m_latitude = coordinate.m_impl->m_latitude;
            m_impl->m_longitude = coordinate.m_impl->m_longitude;
        }
        else
        {
            m_impl.reset();
        }
        return *this;
    }

    Coordinate &Coordinate::operator=(Coordinate &&coordinate) noexcept
    {
        if (this != &coordinate)
        {
            m_impl = std::move(coordinate.m_impl);
        }
        return *this;
    }

    bool Coordinate::operator==(const Coordinate &coordinate) const
    {
        if (!m_impl || !coordinate.m_impl)
        {
            return (!m_impl && !coordinate.m_impl);
        }
        if (m_impl->m_latitude == coordinate.m_impl->m_latitude &&
            m_impl->m_longitude == coordinate.m_impl->m_longitude)
        {
            return true;
        }
        return false;
    }

    bool Coordinate::IsValid(void) const
    {
        if (!m_impl)
        {
            return false;
        }
        if (m_impl->m_latitude < -90.0F || m_impl->m_latitude > 90.0F)
        {
            return false;
        }
        if (m_impl->m_longitude < -180.0F || m_impl->m_longitude > 180.0F)
        {
            return false;
        }
        return true;
    }

    void Coordinate::SetLatitude(const double latitude)
    {
        if (!m_impl)
        {
            m_impl = std::unique_ptr<Impl>(new Impl());
        }
        m_impl->m_latitude = latitude;
    }

    double Coordinate::GetLatitude(void) const
    {
        return m_impl ? m_impl->m_latitude : 91.0;
    }

    void Coordinate::SetLongitude(const double longitude)
    {
        if (!m_impl)
        {
            m_impl = std::unique_ptr<Impl>(new Impl());
        }
        m_impl->m_longitude = longitude;
    }

    double Coordinate::GetLongitude(void) const
    {
        return m_impl ? m_impl->m_longitude : 181.0;
    }
}