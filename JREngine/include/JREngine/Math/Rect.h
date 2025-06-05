#pragma once

namespace JRE
{
	struct Region
	{
		float x;
		float y;
		float width;
		float height;

		bool Intersects(const Region& other) const
		{
			return !(Right() <= other.Left() || Left() >= other.Right() ||
					 Bottom() <= other.Top() || Top() >= other.Bottom());
		}

		float Left()   const { return x; }
		float Right()  const { return x + width; }
		float Top()    const { return y; }
		float Bottom() const { return y + height; }

		float Width()  const { return width; }
		float Height() const { return height; }
	};
}