#pragma once
#include <cstdint>

namespace JRE
{
	using GUID = uint64_t;
	constexpr GUID InvalidGUID = 0;
	inline GUID GenerateGUID()
	{
		static GUID counter = InvalidGUID + 1;
		return counter++;
	}

	template<typename Resource_t>
	class ResourceHandle
	{
	public:

		ResourceHandle() = default;

		explicit ResourceHandle(GUID id)
			: m_GUID(id)
		{
		}

		GUID GetGUID() const
		{
			return m_GUID;
		}

		bool IsValid() const
		{
			return m_GUID != InvalidGUID;
		}
	private:
		GUID m_GUID{ InvalidGUID };
	};
}