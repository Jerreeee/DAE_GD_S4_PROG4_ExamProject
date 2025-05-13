#pragma once

namespace BubbleBobble::Player
{
	enum class Animation
	{
		Idle,
		Run,
		Shoot,
		Death
	};

	struct AnimationName
	{
		static constexpr Animation GetAnimation(std::string_view animName)
		{
			for (size_t i = 0; i < s_Names.size(); ++i)
			{
				if (s_Names[i] == animName)
					return static_cast<Animation>(i);
			}

			assert(false && "Invalid animName");
			return Animation::Idle; // fallback for non-constexpr builds
		}

		static constexpr std::string_view ToString(Animation anim)
		{
			return s_Names[static_cast<size_t>(anim)];
		}

		inline static constexpr std::array<std::string_view, 4> s_Names{
			"Idle", "Run", "Shoot", "Death"
		};
	};

	struct Input
	{
		int moveDir;
		bool pressedJump;
		bool pressedShoot;
	};
}
