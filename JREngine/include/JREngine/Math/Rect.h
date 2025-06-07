#pragma once

namespace JRE
{
	struct Region
	{
		float x;
		float y;
		float width;
		float height;

		float Left()   const { return x; }
		float Right()  const { return x + width; }
		float Top()    const { return y; }
		float Bottom() const { return y + height; }

		static constexpr float epsilon = 0.0001f;

		bool Intersects(const Region& other) const
		{
			return !(Right() <= other.Left() + epsilon || Left() >= other.Right() - epsilon ||
				Bottom() <= other.Top() + epsilon || Top() >= other.Bottom() - epsilon);
		}

		bool OverlapInX(const Region& other) const
		{
			return !(Right() <= other.Left() + epsilon || Left() >= other.Right() - epsilon);
		}

		bool OverlapInY(const Region& other) const
		{
			return !(Bottom() <= other.Top() + epsilon || Top() >= other.Bottom() - epsilon);
		}

		bool FullOverlapInX(const Region& other) const
		{
			return Left() >= other.Left() - epsilon && Right() <= other.Right() + epsilon;
		}

		bool FullOverlapInY(const Region& other) const
		{
			return Top() >= other.Top() - epsilon && Bottom() <= other.Bottom() + epsilon;
		}
	};
}