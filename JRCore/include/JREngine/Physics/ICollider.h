#pragma once
#include <memory>
#include <cstdint>
#include <type_traits>
#include "JREngine/Math/Rect.h"
#include "glm.hpp"

namespace JRE
{
	enum class ColliderShapeType
	{
		Box
	};

	struct ICollisionShape
	{
		virtual ~ICollisionShape() = default;
		virtual ColliderShapeType GetType() const = 0;
	};

	struct BoxShape final : public ICollisionShape
	{
		glm::vec2 offset{};
		float width{};
		float height{};

		static ColliderShapeType GetStaticType() { return ColliderShapeType::Box; };
		ColliderShapeType GetType() const override { return GetStaticType(); };

		static constexpr float epsilon = 0.0001f;

		Region GetRegion(const glm::vec2& base = {}) const
		{
			return Region{ base.x + offset.x, base.y + offset.y, width, height };
		}

		BoxShape Translated(const glm::vec2& delta) const
		{
			BoxShape result = *this;
			result.offset += delta;
			return result;
		}

		bool Intersects(const BoxShape& other, const glm::vec2& baseA = {}, const glm::vec2& baseB = {}) const
		{
			const Region a = GetRegion(baseA);
			const Region b = other.GetRegion(baseB);

			return !(a.Right() <= b.Left() + epsilon ||
				a.Left() >= b.Right() - epsilon ||
				a.Bottom() <= b.Top() + epsilon ||
				a.Top() >= b.Bottom() - epsilon);
		}

		bool OverlapInX(const BoxShape& other, const glm::vec2& baseA = {}, const glm::vec2& baseB = {}) const
		{
			const Region a = GetRegion(baseA);
			const Region b = other.GetRegion(baseB);

			return !(a.Right() <= b.Left() + epsilon ||
				a.Left() >= b.Right() - epsilon);
		}

		bool OverlapInY(const BoxShape& other, const glm::vec2& baseA = {}, const glm::vec2& baseB = {}) const
		{
			const Region a = GetRegion(baseA);
			const Region b = other.GetRegion(baseB);

			return !(a.Bottom() <= b.Top() + epsilon ||
				a.Top() >= b.Bottom() - epsilon);
		}

		bool FullOverlapInX(const BoxShape& other, const glm::vec2& baseA = {}, const glm::vec2& baseB = {}) const
		{
			const Region a = GetRegion(baseA);
			const Region b = other.GetRegion(baseB);

			return a.Left() >= b.Left() - epsilon &&
				a.Right() <= b.Right() + epsilon;
		}

		bool FullOverlapInY(const BoxShape& other, const glm::vec2& baseA = {}, const glm::vec2& baseB = {}) const
		{
			const Region a = GetRegion(baseA);
			const Region b = other.GetRegion(baseB);

			return a.Top() >= b.Top() - epsilon &&
				a.Bottom() <= b.Bottom() + epsilon;
		}

		float GetXDist(const BoxShape& other, const glm::vec2& baseA = {}, const glm::vec2& baseB = {}) const
		{
			const Region a = GetRegion(baseA);
			const Region b = other.GetRegion(baseB);

			if (a.Right() <= b.Left())
				return a.Right() - b.Left();
			else if (a.Left() >= b.Right())
				return a.Left() - b.Right();
			//overlap
			return 0.0f;
		}

		float GetYDist(const BoxShape& other, const glm::vec2& baseA = {}, const glm::vec2& baseB = {}) const
		{
			const Region a = GetRegion(baseA);
			const Region b = other.GetRegion(baseB);

			if (a.Bottom() <= b.Top())
				return a.Bottom() - b.Top();
			else if (a.Top() >= b.Bottom())
				return a.Top() - b.Bottom();
			//overlap
			return 0.0f;
		}
	};

	struct ColliderProperties
	{
		uint32_t layer = 0;
		uint32_t mask = 0;

		bool isStatic = false;
		bool isTrigger = false;
	};

	struct StaticCollider
	{
		std::unique_ptr<ICollisionShape> shape;
		ColliderProperties properties;
		uint32_t group;
	};

	class GameObject;
	class ICollider
	{
	public:
		virtual ~ICollider() = default;

		virtual GameObject& GetOwner() const = 0;
		virtual const ICollisionShape& GetShape() const = 0;
		virtual BoxShape GetBounds() const = 0;
		virtual const ColliderProperties& GetProperties() const = 0;
		virtual void OnCollisionWith(const ICollider&) {};

		template<typename T>
		requires std::is_base_of_v<ICollisionShape, T>
		const T& GetShapeByType()
		{
			const ICollisionShape& shape = GetShape();
			assert(T::GetStaticType() == shape.GetType() && "ICollider::GetShapeByType() type mismatch");
			return static_cast<const T&>(shape);
		}
	};
}
