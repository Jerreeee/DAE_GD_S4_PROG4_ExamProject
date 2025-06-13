#pragma once

namespace JRE
{
	struct PersistenceLayer
	{
		static constexpr uint32_t SceneLocal = 0;			//stays in current scene
		static constexpr uint32_t None = 1 << 0;			//remove from scene when switching scene
		static constexpr uint32_t AcrossScenes = 1 << 1;	//move accross scenes
	};
}
