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
	};
}
