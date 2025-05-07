#pragma once
#include <cstdint>
#include <cstddef>

namespace JRE
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& other);

		operator uint64_t() const { return m_UUID; };

		static const UUID InvalidUUID;
	private:
		uint64_t m_UUID; //generate valid UUID by default by calling UUID()
	};
}

namespace std
{
	template <typename T>
	struct hash;

	template<>
	struct hash<JRE::UUID>
	{
		std::size_t operator()(const JRE::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}