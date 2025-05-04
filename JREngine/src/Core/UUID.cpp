#include "Core/UUID.h"
#include <iostream>

namespace JRE
{
	const UUID UUID::InvalidUUID(0);

	uint64_t GenerateGUID()
	{
		static uint64_t counter = 0;
		return ++counter;
	}

	UUID::UUID() :
		m_UUID{ GenerateGUID() }
	{
		std::cout << "Constructor called\n";
	}

	UUID::UUID(uint64_t uuid) :
		m_UUID{ uuid }
	{
	}
	UUID::UUID(const UUID& other) :
		m_UUID{ other.m_UUID }
	{
		std::cout << "Copy Constructor called\n";
	}
}