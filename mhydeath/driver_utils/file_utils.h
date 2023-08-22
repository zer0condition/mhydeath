#pragma once

#include <Windows.h>
#include <string>
#include <fstream>

namespace file_utils
{
	bool create_file_from_buffer(const std::string_view file_path, void* buffer, size_t size);
}