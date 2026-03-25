#pragma once
#include <cstdint>
#include <cstddef>
#include <string_view>

namespace JRE
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& other);

		bool IsValid() const { return m_UUID != InvalidUUID; };

		operator uint64_t() const { return m_UUID; };

		static UUID FromPath(std::string_view virtualPath); // hash-based, stable
		static UUID Generate();                              // session-unique, high-bit sentinel

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
