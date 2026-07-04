#ifndef GEO_CALCULATOR_H_
#define GEO_CALCULATOR_H_

#include "Coordinate.h"

#include <vector>

namespace VDES
{
    class GeoCalculator
    {
    public:

        static double Distance(const Coordinate &coordinate1, const Coordinate &coordinate2, 
                               const bool isGreatCircle = false);

        static Coordinate GetRelativePosition(const Coordinate &base, const double bearing, 
                                              const double distance, const bool isGreatCircle = false);

        static double GetDistanceToLineSegment(const Coordinate &point, const Coordinate &startNode, 
            const Coordinate &endNode);

        static double GetDistanceToPolygon(const Coordinate &point, const std::vector<Coordinate> &polygon);
    };
}

#endif