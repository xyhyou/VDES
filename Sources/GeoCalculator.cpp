#include "GeoCalculator.h"

#include <cmath>

#define DEGREE_TO_RADIAN(degree)	((degree) * PI / 180)
#define RADIAN_TO_DEGREE(rad)		((rad * 180)/PI)
#define R_EARTH						3443.91738661

static const double PI = (double)3.1415926535897932384626433832795;
static const double DEGREE = (PI / 180.0);
static const double RADIAN = (180.0 / PI);

/*
* Note : This algorithm originates from OpenCPN.(Rhumb Line)
*/
static void DistanceBearingMercator(double lat1, double lon1, double lat0, double lon0,
								    double *brg, double *dist)
{
	// Calculate bearing and distance between two points
	double latm = (lat0 + lat1) / 2 * DEGREE;  // median of latitude
	double delta_lat = (lat1 - lat0);
	double delta_lon = (lon1 - lon0);
	double ex_lat0, ex_lat1;
	double bearing, distance;
	// make sure we calc the shortest route, even if this across the date line.
	if (delta_lon < -180)
	{
		delta_lon += 360;
	}
	if (delta_lon > 180)
	{
		delta_lon -= 360;
	}

	if (delta_lon == 0)
	{
		bearing = .0;  // delta lon = 0 so course is due N or S
	}
	else if (fabs(delta_lat) != .0)
	{
		bearing = atan(delta_lon * cos(latm) / delta_lat);
	}
	else
	{
		bearing = PI / 2;  // delta lat = 0 so course must be E or W
	}

	distance = sqrt(pow(delta_lat, 2) + pow(delta_lon * cos(latm), 2));

	// > 1 degree we use exaggerated latitude to be more exact
	if (distance > 0.01745)
	{
		if (delta_lat != 0.)
		{
			ex_lat0 = 10800 / PI * log(tan(PI / 4 + lat0 * DEGREE / 2));
			ex_lat1 = 10800 / PI * log(tan(PI / 4 + lat1 * DEGREE / 2));
			bearing = atan(delta_lon * 60 / (ex_lat1 - ex_lat0));
			distance = fabs(delta_lat / cos(bearing));
		}
		else
		{
			bearing = PI / 2;
		}
	}

	bearing = fabs(bearing);
	if (lat1 > lat0)
	{
		if (delta_lon < 0)
		{
			bearing = 2 * PI - bearing;
		}
	}  // NW
	else
	{
		if (delta_lon > 0)
		{
			bearing = PI - bearing;  // SE
		}
		else
		{
			bearing = PI + bearing;
		}
	}  // SW

	if (brg)
	{
		*brg = bearing * RADIAN;  // in degrees
	}
	if (dist)
	{
		*dist = distance * 60;   // in NM
	}
}

namespace VDES
{
	double GeoCalculator::Distance(const Coordinate &coordinate1, const Coordinate &coordinate2,
		const bool isGreatCircle)
	{
		double distance = 0;
		DistanceBearingMercator(coordinate1.GetLatitude(), coordinate1.GetLongitude(), 
			coordinate2.GetLatitude(), coordinate2.GetLongitude(), nullptr, &distance);
		return distance;
	}

	Coordinate GeoCalculator::GetRelativePosition(const Coordinate &base, 
		const double bearing, const double distance, const bool isGreatCircle)
	{
		double latitudeTarget = 91.0F;
		double longitudeTarget = 181.0F;

		if (isGreatCircle)
		{
			// TODO...
		}
		else
		{
			double ang = DEGREE_TO_RADIAN(bearing);
			double dLatLine = distance * cos(ang);
			double dLngLine = distance * sin(ang);

			double dLat = dLatLine / R_EARTH;
			double rLng = R_EARTH * cos(DEGREE_TO_RADIAN(base.GetLatitude()));
			double dLng = dLngLine / rLng;

			latitudeTarget = (float)(base.GetLatitude() + RADIAN_TO_DEGREE(dLat));
			latitudeTarget = fmod(latitudeTarget, 360);

			if (fabs(latitudeTarget) > 90)
			{
				if (latitudeTarget > 0)
				{
					latitudeTarget = 180 - latitudeTarget;
				}
				else
				{
					latitudeTarget = 180 + latitudeTarget;
				}
			}

			longitudeTarget = (float)(base.GetLongitude() + RADIAN_TO_DEGREE(dLng));
			longitudeTarget = fmod(longitudeTarget, 360);
			if (fabs(longitudeTarget) > 180)
			{
				if (longitudeTarget > 0)
				{
					longitudeTarget -= 360;
				}
				else
				{
					longitudeTarget += 360;
				}
			}
		}
		return Coordinate(latitudeTarget, longitudeTarget);
	}

	struct VECTOR
	{
		double x, y;
	};

	static double GetDistanceToLineSegmentInner(Coordinate &nearest, const Coordinate &point, 
		const Coordinate &startNode, const Coordinate &endNode)
	{
		VECTOR segment;
		segment.x = endNode.GetLongitude() - startNode.GetLongitude();
		segment.y = endNode.GetLatitude() - startNode.GetLatitude();

		VECTOR startNodeToPoint;
		startNodeToPoint.x = point.GetLongitude() - startNode.GetLongitude();
		startNodeToPoint.y = point.GetLatitude() - startNode.GetLatitude();

		double cross = segment.x * startNodeToPoint.x + segment.y * startNodeToPoint.y;
		if (cross < 0)
		{
			nearest = startNode;
			return GeoCalculator::Distance(point, startNode);
		}

		double temp = std::pow(segment.x, 2) + std::pow(segment.y, 2);
		if (cross >= temp)
		{
			nearest = endNode;
			return GeoCalculator::Distance(point, endNode);
		}

		double r = cross / temp;
		double px = startNode.GetLongitude() + segment.x * r;
		double py = startNode.GetLatitude() + segment.y * r;

		nearest = Coordinate(py, px);
		return GeoCalculator::Distance(point, nearest);
	}

	double GeoCalculator::GetDistanceToLineSegment(const Coordinate &point, const Coordinate &startNode, 
		const Coordinate &endNode)
	{
		Coordinate nearestNode;

		// Determine whether the line crosses the 180-degree meridian
		if (startNode.GetLongitude() * endNode.GetLongitude() < 0) 
		{
			if (fabs(startNode.GetLongitude()) > 90 && fabs(endNode.GetLongitude()) > 90)
			{
				Coordinate positive, negative;

				if (startNode.GetLongitude() > 0)
				{
					positive = startNode;
					negative = endNode;
				}
				else
				{
					positive = endNode;
					negative = startNode;
				}

				// Translate the line segment
				Coordinate startTemp, endTemp, pointTemp;
				startTemp.SetLatitude(positive.GetLatitude());
				startTemp.SetLongitude(180.0F - positive.GetLongitude());
				endTemp.SetLatitude(negative.GetLatitude());
				endTemp.SetLongitude((negative.GetLongitude() + 180.0F) + 2 * (180.0F - positive.GetLongitude()));
				
				// Translate the point picked.
				pointTemp.SetLatitude(point.GetLatitude());
				pointTemp.SetLongitude(point.GetLongitude());
				if (point.GetLongitude() > 0)
				{
					pointTemp.SetLongitude(startTemp.GetLongitude() + (pointTemp.GetLongitude() - positive.GetLongitude()));
				}
				else
				{
					pointTemp.SetLongitude(startTemp.GetLongitude() + (pointTemp.GetLongitude() + 180.0F + 180.0F - positive.GetLongitude()));
				}

				
				double distance = GetDistanceToLineSegmentInner(nearestNode, pointTemp, startTemp, endTemp);
				auto longitude = nearestNode.GetLongitude();
				nearestNode.SetLongitude(longitude - startTemp.GetLongitude() + startNode.GetLongitude());
				return distance;
			}
		}
		return GetDistanceToLineSegmentInner(nearestNode, point, startNode, endNode);
	}

	double GeoCalculator::GetDistanceToPolygon(const Coordinate &point, 
		const std::vector<Coordinate> &polygon)
	{
		auto	nodeNum = polygon.size();
		double  distance = -1.0F;

		for (auto i = 0U; i < nodeNum - 1; i++)
		{
			auto &startNode = polygon.at(i);
			auto &endNode = polygon.at(i + 1);
			auto value = GetDistanceToLineSegment(point, startNode, endNode);
			if (distance < 0)
			{
				distance = value;
			}
			else if (distance >= value)
			{
				distance = value;
			}
		}
		return distance;
	}
}


