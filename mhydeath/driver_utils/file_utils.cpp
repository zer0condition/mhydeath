#include "file_utils.h"

bool file_utils::create_file_from_buffer(const std::string_view file_path, void* buffer, size_t size)
{
	std::ofstream stream(
		file_path.data(),
		std::ios_base::out | std::ios_base::binary
	);

	if (!stream.write((char*)buffer, size))
	{
		stream.close();
		return false;
	}

	stream.close();
	return true;
}