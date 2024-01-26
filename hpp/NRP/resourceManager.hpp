#pragma once

#include <NRP/metaParser.hpp>

namespace NightRP {
	class Resource {
		const char* m_name;
		Meta::Meta m_meta;
	public:
		Resource(const char* name) noexcept;
	};
}