#pragma once

namespace BubbleBobble
{
	// Registers BubbleBobble's asset loaders with the engine's AssetLoaderRegistry.
	// Call once at startup, before any game asset is loaded.
	//
	// This is done explicitly rather than via per-file static initializers in each importer.
	// Static initializers only run if the linker keeps their object file, and a static library
	// drops every object file the final binary never references — so the moment this game code
	// is extracted into a BubbleBobbleLib static lib, self-registration would silently vanish.
	// Mirrors the engine-side RegisterBuiltinAssetLoaders() in JREngine.cpp.
	void RegisterGameAssetLoaders();
}
