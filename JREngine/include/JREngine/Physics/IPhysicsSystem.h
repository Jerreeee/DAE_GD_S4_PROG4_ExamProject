#pragma once
#include <vector>
#include "JREngine/Physics/ICollider.h"

namespace JRE
{
	class IPhysicsSystem
	{
	public:
		virtual ~IPhysicsSystem() = default;

		virtual void Update() = 0;

		virtual uint32_t GetFreeStaticGroup() = 0;
		virtual void RegisterStaticCollider(StaticCollider collider) = 0;
		virtual void UnegisterStaticCollidersByGroup(uint32_t group, bool removeGroup = true) = 0;

		virtual void RegisterCollider(ICollider* const collider) = 0;
		virtual void UnregisterCollider(ICollider* const collider) = 0;

		virtual void SetGravity(float gravity) = 0;
		virtual void SetWorldScale(float scale) = 0;
	};

	class NullPhysicsSystem final : public IPhysicsSystem
	{
	public:
		virtual void Update() override {};

		virtual uint32_t GetFreeStaticGroup() override { return 0; };
		virtual void RegisterStaticCollider(StaticCollider) override {};
		virtual void UnegisterStaticCollidersByGroup(uint32_t, bool) override {};

		virtual void RegisterCollider(ICollider* const) override {};
		virtual void UnregisterCollider(ICollider* const) override {};

		virtual void SetGravity(float) override {};
		virtual void SetWorldScale(float) override {};
	};
}