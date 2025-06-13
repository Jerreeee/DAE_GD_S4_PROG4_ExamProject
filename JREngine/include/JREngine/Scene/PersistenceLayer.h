#pragma once

namespace JRE
{
	struct PersistenceLayer
	{
		static constexpr uint32_t SceneLocal = 1 << 0;		//stays in current scene
		static constexpr uint32_t SceneTemporary = 1 << 1;	//remove from scene when switching scene
	};
}
