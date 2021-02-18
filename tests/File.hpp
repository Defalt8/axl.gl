#pragma once
#include <axl.util/String.hpp>
#include <axl.util/WString.hpp>

class File {
	public:
		static bool exists(const axl::util::String& filename);
		static axl::util::String getStringContent(const axl::util::String& filename);
		static axl::util::WString getWStringContent(const axl::util::String& filename);

};