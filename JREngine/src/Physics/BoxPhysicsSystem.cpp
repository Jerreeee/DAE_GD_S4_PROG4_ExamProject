#include "JREngine/Scene/GameObject.h"
#include "Physics/Box2DColliderComponent.h"
#include "Physics/BoxPhysicsSystem.h"

namespace JRE
{
	void BoxPhysicsSystem::Update()
	{
		//Dynamic vs Dynamic
		for (size_t i = 0; i < m_DynamicColliders.size(); ++i)
		{
			ICollider& a = *m_DynamicColliders[i];
			const auto& propsA = a.GetProperties();

			for (size_t j = i + 1; j < m_DynamicColliders.size(); ++j)
			{
				ICollider& b = *m_DynamicColliders[j];
				const auto& propsB = b.GetProperties();

				//Only check collision if layers and masks match
				if (!((propsA.layer & propsB.mask) && (propsB.layer & propsA.mask)))
					continue;

				const auto& shapeA = static_cast<const BoxShape&>(a.GetShape());
				const auto& shapeB = static_cast<const BoxShape&>(b.GetShape());

				const glm::vec2 posA = glm::vec2(a.GetOwner().GetWorldPosition());
				const glm::vec2 posB = glm::vec2(b.GetOwner().GetWorldPosition());

				BoxShape worldA = shapeA.Translated(posA);
				BoxShape worldB = shapeB.Translated(posB);

				if (worldA.Intersects(worldB))
				{
					a.OnCollisionWith(b);
					b.OnCollisionWith(a);
				}
			}
		}
	}
	uint32_t BoxPhysicsSystem::GetFreeStaticGroup()
	{
		uint32_t groupID;
		while (m_StaticColliderGroups.count(groupID))
			++groupID;

		m_StaticColliderGroups.insert(groupID);
		return groupID;
	}
	void BoxPhysicsSystem::RegisterStaticCollider(StaticCollider collider)
	{
		if (!collider.shape || collider.shape->GetType() != ColliderShapeType::Box)
			return;
		m_StaticColliders.emplace_back(std::move(collider));
	}
	void BoxPhysicsSystem::UnegisterStaticCollidersByGroup(uint32_t group, bool removeGroup)
	{
		std::erase_if(m_StaticColliders, [=](const StaticCollider& c)
			{
				return c.group == group;
			});

		if (removeGroup)
			m_StaticColliderGroups.erase(group);
	}
	void BoxPhysicsSystem::RegisterCollider(ICollider* const collider)
	{
		if (!collider ||
			collider->GetShape().GetType() != ColliderShapeType::Box ||
			collider->GetProperties().isStatic)
			return;

		m_DynamicColliders.emplace_back(collider);
	}
	void BoxPhysicsSystem::UnregisterCollider(ICollider* const collider)
	{
		if (!collider ||
			collider->GetProperties().isStatic)
			return;
		std::erase(m_DynamicColliders, collider);
	}
	bool BoxPhysicsSystem::MoveCollider(glm::vec2 oldPos, const ICollider& a, const MoveSettings& ms, const CollisionSettings& cs, CollisionInfo& ci) const
	{
		ci = CollisionInfo{}; //ensure no existing data

		const ICollisionShape& shapeA = a.GetShape();
		if (shapeA.GetType() != ColliderShapeType::Box)
			return false; //only BoxShape supported

		const BoxShape& boxA = static_cast<const BoxShape&>(shapeA);
		BoxShape worldBoxA = boxA.Translated(oldPos);
		const auto& propsA = a.GetProperties();
		const Region& regionA = worldBoxA.GetRegion();
		glm::vec2 worldLeftTopA = glm::vec2(regionA.x, regionA.y); //leftTop pos of the collider in worldSpace

		const float dt = ms.dt;
		glm::vec2 vel = ms.vel;

		if (ms.applyGravity)
			vel.y += m_Gravity * dt;

		VelInfo vi{ GetVelInfo(vel) };
		ci.velOut = vel;

		glm::vec2 endPos{
			worldLeftTopA.x + vel.x * m_VelScale * dt,
			worldLeftTopA.y + vel.y * m_VelScale * dt,
		};

		glm::vec2 offset{ endPos.x - oldPos.x, endPos.y - oldPos.y };
		glm::vec2 absOffset{ std::abs(offset.x), std::abs(offset.y) };

		BoxShape newBoxShapeX{ worldBoxA.Translated(glm::vec2(offset.x, 0.f)) };
		BoxShape newBoxShapeY{ worldBoxA.Translated(glm::vec2(0.f, offset.y)) };
		Region newRegionX{ newBoxShapeX.GetRegion() };
		Region newRegionY{ newBoxShapeY.GetRegion() };

		for (const StaticCollider& b : m_StaticColliders)
		{
			//Only check collision if layers and masks match
			const auto& propsB = b.properties;
			if (!((propsA.layer & propsB.mask) && (propsB.layer & propsA.mask)))
				continue;

			const ICollisionShape& shapeB = *b.shape;
			const BoxShape& boxB = static_cast<const BoxShape&>(shapeB);
			const Region& regionB = boxB.GetRegion();

			//Check X collision
			if (cs.CheckX() && vi.InX() && newBoxShapeX.Intersects(boxB))
			{
				if (cs.checkLeft && vi.left && newRegionX.Left() < regionB.Right()) //Colliding left
				{
					float lambda = (regionA.Left() - regionB.Right()) / absOffset.x;
					if (lambda < ci.left.lambda)
					{
						ci.collDir.left = true;
						ci.left.lambda = lambda;
						ci.left.collPos = regionB.Right();
						ci.left.entryDist = regionB.Right() - newRegionX.Left();
					}
				}
				else if (cs.checkRight && vi.right && regionB.Left() < newRegionX.Right()) //Colliding Right
				{
					float lambda = (regionB.Left() - regionA.Right()) / absOffset.x;
					if (lambda < ci.right.lambda)
					{
						ci.collDir.right = true;
						ci.right.lambda = lambda;
						ci.right.collPos = regionB.Left();
						ci.right.entryDist = newRegionX.Right() - regionB.Left();
					}
				}
			}

			//Check Y collision
			if (cs.CheckY() && vi.InY() && newBoxShapeY.Intersects(boxB))
			{
				if (cs.checkUp && vi.up && newRegionY.Top() < regionB.Bottom()) //Colliding up
				{
					float lambda = (regionA.Top() - regionB.Bottom()) / absOffset.y;
					if (lambda < ci.up.lambda)
					{
						ci.collDir.up = true;
						ci.up.lambda = lambda;
						ci.up.collPos = regionB.Bottom();
						ci.up.entryDist = regionB.Bottom() - newRegionY.Top();
					}
				}
				else if (cs.checkDown && vi.down && regionB.Top() < newRegionY.Bottom()) //Colliding down
				{
					float lambda = (regionB.Top() - regionA.Bottom()) / absOffset.y;
					if (lambda < ci.down.lambda)
					{
						ci.collDir.down = true;
						ci.down.lambda = lambda;
						ci.down.collPos = regionB.Top();
						ci.down.entryDist = newRegionY.Bottom() - regionB.Top();
					}
				}
			}
		}

		if (!ci.Collided())
			ci.newPos = endPos;
		else if (ci.collDir.X() && !ci.collDir.Y()) //Collided in X only
		{
			float x = ci.collDir.right ? ci.right.collPos - regionA.width : ci.left.collPos;
			ci.newPos = glm::vec2{ x, endPos.y };
			ci.velOut.x = 0.f;
		}
		else if (ci.collDir.Y() && !ci.collDir.X()) //Collided in Y only
		{
			float y = ci.collDir.up ? ci.up.collPos : ci.down.collPos - regionA.height;
			ci.newPos = glm::vec2{ endPos.x, y };
			ci.velOut.y = 0.f;
		}
		else if (vi.LeftDown() && ci.collDir.LeftBottom()) //LeftBottom concave collision
		{
			ci.newPos = glm::vec2{ ci.left.collPos, ci.down.collPos - regionA.height };
			ci.velOut = glm::vec2{};
		}
		else if (vi.LeftUp() && ci.collDir.LeftTop()) //LeftTop concave collision
		{
			ci.newPos = glm::vec2{ ci.left.collPos, ci.up.collPos };
			ci.velOut = glm::vec2{};
		}
		else if (vi.RightUp() && ci.collDir.RightTop()) //RightTop concave collision
		{
			ci.newPos = glm::vec2{ ci.right.collPos - regionA.width, ci.up.collPos };
			ci.velOut = glm::vec2{};
		}
		else if (vi.RightDown() && ci.collDir.RightBottom()) //RightBottom concave collision
		{
			ci.newPos = glm::vec2{ ci.right.collPos - regionA.width, ci.down.collPos - regionA.height };
			ci.velOut = glm::vec2{};
		}
		else //convex corner collision
		{
			//Calculate if the collision happened in X or Y first
			float lambdaX{ (vi.right) ? ci.right.lambda : ci.left.lambda };
			float lambdaY{ (vi.up) ? ci.up.lambda : ci.down.lambda };
			if (lambdaY <= lambdaX) //Y wins when equal to X
			{
				float y = ci.collDir.up ? ci.up.collPos : ci.down.collPos - regionA.height;
				ci.newPos = glm::vec2{ endPos.x, y };
				ci.collDir.left = false;
				ci.collDir.right = false;
				ci.velOut.y = 0.f;
			}
			else //lambdaX > lambdaY
			{
				float x = ci.collDir.right ? ci.right.collPos - regionA.width : ci.left.collPos;
				ci.newPos = glm::vec2{ x, endPos.y };
				ci.collDir.up = false;
				ci.collDir.down = false;
				ci.velOut.x = 0.f;
			}
		}

		//Currently ci.newPos is the worldLeftTop pos of the collider
		//Subtract the collider offset to get the worldLeftTop pos
		//of the object that has the collider
		ci.newPos -= boxA.offset;

		return ci.Collided();
	}
	void BoxPhysicsSystem::SetWorldScale(float scale)
	{
		m_WorldScale = scale;
		m_VelScale = 1 / scale;
	}
	VelInfo BoxPhysicsSystem::GetVelInfo(glm::vec2 vel) const
	{
		VelInfo vi{};
		vi.left = vel.x < 0.f;
		vi.right = !vi.left;
		vi.up = vel.y < 0.f;
		vi.down = !vi.up;
		return vi;
	}
}
