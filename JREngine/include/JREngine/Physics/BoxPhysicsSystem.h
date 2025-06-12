#pragma once
#include <unordered_set>
#include <functional>
#include "JREngine/Physics/IPhysicsSystem.h"
#include "JREngine/Physics/ICollider.h"
#include "glm.hpp"

namespace JRE
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

	class BoxPhysicsSystem final : public IPhysicsSystem
	{
	public:
		virtual void Update() override;

		virtual uint32_t GetFreeStaticGroup() override;
		virtual void RegisterStaticCollider(StaticCollider collider) override;
		virtual void UnegisterStaticCollidersByGroup(uint32_t group, bool removeGroup = true) override;

		virtual void RegisterCollider(ICollider* const collider) override;
		virtual void UnregisterCollider(ICollider* const collider) override;

		struct CollisionSettings
		{
			glm::vec2 oldPos;
			const ICollider& collider;
			float dt;
			glm::vec2 vel;
			bool applyGravity{ true };

			using CollisionFilterFunc = std::function<CollisionDir(const StaticCollider& collider, const CollisionSettings& cs)>;
			CollisionFilterFunc filterFunc;
		};

		//Moves the give region with given velocity and deltaTime and check for collision
		//Fills in a CollisionInfo output struct
		bool MoveCollider(const CollisionSettings& cs, CollisionInfo& ci) const;

		virtual void SetGravity(float gravity) override { m_Gravity = gravity; };
		virtual void SetWorldScale(float scale) override;

		VelInfo GetVelInfo(glm::vec2 vel) const;
	private:
		std::unordered_set<uint32_t> m_StaticColliderGroups{};
		std::vector<StaticCollider> m_StaticColliders{};
		std::vector<ICollider*> m_DynamicColliders{};
		float m_Gravity{ 9.81f };
		float m_WorldScale{ 1.f };
		float m_VelScale{ 1.f };
	};
}
