#include "BoundingBox.h"
#include "GeoCalculator.h"

#include <cmath>

namespace VDES
{
	class BoundingBox::Impl
	{
	public:
		Impl();

		~Impl();

		void UpdateRect(Rect &rect, const double latitude, const double longitude);

		void UpdateRect(const Rect &rect);

		void ClearRect(Rect &rect);

	public:
		std::vector<Rect> m_rectSet;
		Coordinate        m_coordinateLast;
	};

	BoundingBox::Impl::Impl()
	{
		
	}

	BoundingBox::Impl::~Impl()
	{

	}

	void BoundingBox::Impl::UpdateRect(Rect &rect, const double latitude, const double longitude)
	{
		// 初始化
		if (rect.left == 0 && rect.right == 0 && rect.top == 0 && rect.bottom == 0)
		{
			rect.left = rect.right = longitude;
			rect.top = rect.bottom = latitude;
		}
		else
		{
			if (rect.top < latitude)
			{
				rect.top = latitude;
			}
			if (rect.bottom > latitude)
			{
				rect.bottom = latitude;
			}

			if (rect.left > longitude)
			{
				rect.left = longitude;
			}
			if (rect.right < longitude)
			{
				rect.right = longitude;
			}
		}
	}

	void BoundingBox::Impl::UpdateRect(const Rect &rect)
	{
		Rect rectTemp;

		rectTemp.top = rect.top;
		rectTemp.bottom = rect.bottom;
		// 判断是否在180度经线附近
		if (rect.left < -180 && rect.right < 0)
		{
			// 0-180
			rectTemp.left = rect.left + 360.0F;
			rectTemp.right = 180.0F;
			m_rectSet.push_back(rectTemp);
			// -180 - 0
			rectTemp.left = -180.0F;
			rectTemp.right = rect.right;
			m_rectSet.push_back(rectTemp);
		}
		else if (rect.left > 0.0F && rect.right > 180.0F)
		{
			// 0 - 180
			rectTemp.left = rect.left;
			rectTemp.right = 180.0F;
			m_rectSet.push_back(rectTemp);
			// -180 - 0
			rectTemp.left = -180.0F;
			rectTemp.right = rect.right - 360.0F;
			m_rectSet.push_back(rectTemp);
		}
		else if (rect.left > rect.right)
		{
			// 大包围圈
			// rect.left - 180.0F
			rectTemp.left = rect.left;
			rectTemp.right = 180.0F;
			m_rectSet.push_back(rectTemp);
			// -180 - rect.right
			rectTemp.left = -180.0F;
			rectTemp.right = rect.right;
			m_rectSet.push_back(rectTemp);
		}
		else
		{
			m_rectSet.push_back(rect);
		}
	}

	void BoundingBox::Impl::ClearRect(Rect &rect)
	{
		rect.left = rect.right = rect.top = rect.bottom = 0.0F;
	}

	BoundingBox::BoundingBox()
	{
		m_impl = std::unique_ptr<Impl>(new Impl());
	}

	BoundingBox::BoundingBox(const BoundingBox &boundingBox)
	{
		m_impl = std::unique_ptr<Impl>(new Impl());
		m_impl->m_rectSet = boundingBox.m_impl->m_rectSet;
		m_impl->m_coordinateLast = boundingBox.m_impl->m_coordinateLast;
	}

	BoundingBox::~BoundingBox()
	{

	}

	void BoundingBox::Update(const double latitude, const double longitude)
	{
		auto rectNum = m_impl->m_rectSet.size();

		if (0 == rectNum)
		{
			Rect rect;
			m_impl->ClearRect(rect);
			m_impl->UpdateRect(rect, latitude, longitude);
			m_impl->m_rectSet.push_back(rect);
		}
		else if (1 == rectNum)
		{
			auto &rectNow = m_impl->m_rectSet.at(0);
			auto longitudeLast = m_impl->m_coordinateLast.GetLongitude();

			if (longitude * longitudeLast < 0)
			{
				if (fabs(longitude) > 90.0F)
				{
					// Crossing the 180-degree meridian, the bounding box needs to be split
					Rect rect;
					

					m_impl->ClearRect(rect);
					m_impl->UpdateRect(rect, latitude, longitude);
					if (longitude > 0)
					{
						m_impl->UpdateRect(rect, rectNow.top, 180.0F);
						m_impl->UpdateRect(rect, rectNow.bottom, 180.0F);
						m_impl->m_rectSet.push_back(rect);
						m_impl->UpdateRect(rectNow, latitude, -180.0F);
					}
					else
					{
						m_impl->UpdateRect(rect, rectNow.top, -180.0F);
						m_impl->UpdateRect(rect, rectNow.bottom, -180.0F);
						m_impl->m_rectSet.push_back(rect);
						m_impl->UpdateRect(rectNow, latitude, 180.0F);
					}
					m_impl->m_coordinateLast = Coordinate(latitude, longitudeLast);
					return;
				}
			}
			m_impl->UpdateRect(rectNow, latitude, longitude);
		}
		else
		{
			Rect *positive, *negative;
			if (m_impl->m_rectSet[0].right == 180.0F)
			{
				positive = &m_impl->m_rectSet[0];
				negative = &m_impl->m_rectSet[1];
			}
			else
			{
				positive = &m_impl->m_rectSet[1];
				negative = &m_impl->m_rectSet[0];
			}

			auto longitudeLast = m_impl->m_coordinateLast.GetLongitude();
			if (longitude * longitudeLast < 0)
			{
				if (longitude > 0)
				{
					m_impl->UpdateRect(*positive, latitude, longitude);
					m_impl->UpdateRect(*negative, latitude, -180.0F);
				}
				else
				{
					m_impl->UpdateRect(*positive, latitude, 180.0F);
					m_impl->UpdateRect(*negative, latitude, longitude);
				}
			}
			else
			{
				if (longitude > 0)
				{
					m_impl->UpdateRect(*positive, latitude, longitude);
				}
				else
				{
					m_impl->UpdateRect(*negative, latitude, longitude);
				}
			}
		}
		m_impl->m_coordinateLast = Coordinate(latitude, longitude);
	}

	void BoundingBox::Update(const Coordinate &coordinate)
	{
		Update(coordinate.GetLatitude(), coordinate.GetLongitude());
	}

	BoundingBox BoundingBox::Build(const Rect &rect)
	{
		Rect		rectTemp;
		BoundingBox boundingBox;

		boundingBox.m_impl->ClearRect(rectTemp);
		boundingBox.m_impl->UpdateRect(rect);
		return boundingBox;
	}

	BoundingBox BoundingBox::Build(const std::vector<Rect> &rectSet)
	{
		BoundingBox boundingBox;

		for (auto i = 0U; i < rectSet.size(); i++)
		{
			boundingBox.m_impl->UpdateRect(rectSet[i]);
		}
		return boundingBox;
	}

	BoundingBox BoundingBox::Build(const double latitude, const double longitude, const double range)
	{
		BoundingBox::Rect  rect;
		Coordinate         coordinate, coordinateBase;

		coordinateBase.SetLatitude(latitude);
		coordinateBase.SetLongitude(longitude);

		coordinate = GeoCalculator::GetRelativePosition(coordinateBase, 0, (float)range);
		rect.top = coordinate.GetLatitude();
		coordinate = GeoCalculator::GetRelativePosition(coordinateBase, 90, (float)range);
		rect.right = coordinate.GetLongitude();
		coordinate = GeoCalculator::GetRelativePosition(coordinateBase, 180, (float)range);
		rect.bottom = coordinate.GetLatitude();
		coordinate = GeoCalculator::GetRelativePosition(coordinateBase, 270, (float)range);
		rect.left = coordinate.GetLongitude();

		return BoundingBox::Build(rect);
	}

	void BoundingBox::Clear(void)
	{
		m_impl->m_rectSet.clear();
	}

	std::vector<BoundingBox::Rect> BoundingBox::GetRects(void) const
	{
		return m_impl->m_rectSet;
	}
}


