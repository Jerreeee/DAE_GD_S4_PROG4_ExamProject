#pragma once
#include <vector>
#include <limits>
#include <filesystem>
#include "JREngine/Asset/Asset.h"
#include "JREngine/Math/Rect.h"
#include "glm.hpp"

namespace JRE
{
	class Sprite;
}

namespace BubbleBobble
{
	struct CollisionDir
	{
		bool left;
		bool right;
		bool up;
		bool down;

		bool Any() const { return left || right || up || down; };
		bool X() const { return left || right; };
		bool Y() const { return up || down; };
		bool LeftBottom() const { return left && down; };
		bool LeftTop() const { return left && up; };
		bool RightTop() const { return right && up; };
		bool RightBottom() const { return right && down; };
		bool Corner() const { return LeftBottom() || LeftTop() || RightTop() || RightBottom(); };
	};

	struct CollisionDirInfo
	{
		float collPos;
		float entryDist{ std::numeric_limits<float>::max() };
		float lambda{ std::numeric_limits<float>::max() };
	};

	struct VelInfo
	{
		bool left;
		bool right;
		bool up;
		bool down;

		bool InX() const { return left || right; };
		bool InY() const { return up || down; };
		bool LeftDown() const { return left && down; };
		bool LeftUp() const { return left && up; };
		bool RightUp() const { return right && up; };
		bool RightDown() const { return right && down; };
	};

	struct CollisionInfo
	{
		CollisionDirInfo left;
		CollisionDirInfo right;
		CollisionDirInfo up;
		CollisionDirInfo down;
		CollisionDir collDir;
		glm::vec2 newPos;
		glm::vec2 velOut;

		bool Collided() { return collDir.Any(); };
	};

	class TileMap final : public JRE::Asset
	{
	public:
		struct ColliderInfo
		{
			JRE::Region region;
			bool isPlatform;
		};

		struct SpritePos
		{
			size_t spriteIdx;
			glm::vec2 pos;
		};

		TileMap() = default;

		void Render();

		void SetSprites(const std::vector<JRE::AssetRef<JRE::Sprite>>& sprites);
		void SetSpritePositions(const std::vector<TileMap::SpritePos>& spritePositions);
		void SetCollisionRects(const std::vector<ColliderInfo>& rects);

		//Moves the give region with given velocity and deltaTime through the TileMap
		//and fills in a CollisionInfo output struct with
		bool MovePosition(const JRE::Region& oldRegion, glm::vec2 vel, float dt, bool applyGravity, CollisionInfo& ci) const;

		const std::vector<ColliderInfo>& GetCollisionRects() const { return m_CollisionRects; };
		const std::vector<SpritePos>& GetDrawInfo() const;

		void SetGravity(float gravity) { m_Gravity = gravity; };
		void SetPixelsPerMeter(int pixPerM) { m_PixPerM = pixPerM; };

		static constexpr std::string_view GetStaticType() { return "BubbleBobble::TileMap"; };
		virtual std::string_view GetType() const override { return GetStaticType(); };
	private:
		VelInfo GetVelInfo(glm::vec2 vel) const;

		std::vector<SpritePos> m_SpritePositions{};
		std::vector<JRE::AssetRef<JRE::Sprite>> m_Sprites{};
		std::vector<ColliderInfo> m_CollisionRects{};
		int m_PixPerM{ 10 };
		float m_Gravity{ 200.f };
	};
}
