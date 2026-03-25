#include "Core/UUID.h"
#include "JREngine/Core/Hash.h"
#include <iostream>

namespace JRE
{
	const UUID UUID::InvalidUUID(0);

	UUID UUID::FromPath(std::string_view virtualPath)
	{
		uint64_t h = FNV1a64(virtualPath);
		return UUID(h == 0 ? 1 : h);   // reserve 0 for InvalidUUID
	}

	UUID UUID::Generate()
	{
		static uint64_t counter = 0;
		return UUID(0xFFFF000000000000ULL | ++counter);
	}

	uint64_t GenerateGUID()
	{
		return static_cast<uint64_t>(UUID::Generate());
	}

	UUID::UUID() :
		m_UUID{ GenerateGUID() }
	{
		//std::cout << "UUID created: " << m_UUID << "\n";
	}

	UUID::UUID(uint64_t uuid) :
		m_UUID{ uuid }
	{
	}
	UUID::UUID(const UUID& other) :
		m_UUID{ other.m_UUID }
	{
		//std::cout << "Copy Constructor called\n";
	}
}