#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <memory>

namespace VDES
{
    class Coordinate
    {
    public:
        Coordinate();

        Coordinate(const double latitude, const double longitude);

        Coordinate(const Coordinate &coordinate);

        Coordinate(Coordinate &&coordinate) noexcept;

        ~Coordinate();

        Coordinate &operator=(const Coordinate &coordinate);

        Coordinate &operator=(Coordinate &&coordinate) noexcept;

        bool operator==(const Coordinate &coordinate) const;

        bool IsValid(void) const;

        void SetLatitude(const double latitude);

        double GetLatitude(void) const;

        void SetLongitude(const double longitude);

        double GetLongitude(void) const;

    private:
        class Impl;

        std::unique_ptr<Impl> m_impl;
    };
}

#endif