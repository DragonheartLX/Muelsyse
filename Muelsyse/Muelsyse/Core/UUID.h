#pragma once
#include "Muelsyse/Core/Core.h"

namespace mul
{
	class MUL_API UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std 
{
	template <typename T> struct hash;

	template<>
	struct hash<mul::UUID>
	{
		std::size_t operator()(const mul::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}