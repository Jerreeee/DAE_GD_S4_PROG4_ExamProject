#include "Event.h"

namespace Engine
{
	Event::Event(EventID _id) : id{ _id } {};
	Event::~Event() = default;
}