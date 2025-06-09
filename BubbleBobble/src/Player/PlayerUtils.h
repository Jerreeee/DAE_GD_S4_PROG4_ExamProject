#pragma once

namespace BubbleBobble
{
	enum class PlayerAnimation
	{
		Idle,
		Run,
		Shoot,
		Death
	};

	struct PlayerAnimationName
	{
		static constexpr PlayerAnimation GetAnimation(std::string_view animName)
		{
			for (size_t i = 0; i < s_Names.size(); ++i)
			{
				if (s_Names[i] == animName)
					return static_cast<PlayerAnimation>(i);
			}

			assert(false && "Invalid animName");
			return PlayerAnimation::Idle; // fallback for non-constexpr builds
		}

		static constexpr std::string_view ToString(PlayerAnimation anim)
		{
			return s_Names[static_cast<size_t>(anim)];
		}

		inline static constexpr std::array<std::string_view, 4> s_Names{
			"Idle", "Run", "Shoot", "Death"
		};
	};
}
