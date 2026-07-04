#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include "Coordinate.h"
#include "BoundingBox.h"

#include <memory>
#include <vector>

namespace VDES
{
	class BoundingBox
	{
	public:
		BoundingBox();

		BoundingBox(const BoundingBox &boundingBox);

		virtual ~BoundingBox();

		struct Rect
		{
			double left;
			double right;
			double top;
			double bottom;
		};

		static BoundingBox Build(const Rect &rect);

		static BoundingBox Build(const std::vector<Rect> &rectSet);

		static BoundingBox Build(const double latitude, const double longitude, const double range);

		void Update(const double latitude, const double longitude);

		void Update(const Coordinate &coordinate);

		void Clear(void);

		std::vector<Rect> GetRects(void) const;

	private:
		class Impl;

		std::unique_ptr<Impl> m_impl;
	};
}

#endif