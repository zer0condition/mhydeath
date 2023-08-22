#pragma once
#include <Windows.h>
#include <fstream>
#include <filesystem>

#include "../driver_utils/raw_driver.h"
#include "../driver_utils/file_utils.h"
#include "../service_utils/service_utils.h"



#define MHYPROT_SERVICE_NAME "mhyprotect"
#define MHYPROT_DISPLAY_NAME "mhyprotect"
#define MHYPROT_SYSFILE_NAME "mhyprot.sys"

#define MHYPROT_DEVICE_NAME "\\\\?\\\\mhyprotect"

#define MHYPROT_IOCTL_INITIALIZE                0x80034000
#define MHYPROT_IOCTL_READ_KERNEL_MEMORY        0x83064000
#define MHYPROT_IOCTL_READ_WRITE_USER_MEMORY    0x81074000
#define MHYPROT_IOCTL_ENUM_PROCESS_MODULES      0x82054000
#define MHYPROT_IOCTL_GET_SYSTEM_UPTIME         0x80134000
#define MHYPROT_IOCTL_ENUM_PROCESS_THREADS      0x83024000
#define MHYPROT_IOCTL_TERMINATE_PROCESS         0x81034000

#define MHYPROT_ACTION_READ	0x0
#define MHYPROT_ACTION_WRITE	0x1

#define MHYPROT_OFFSET_SEEDMAP 	0xA0E8

namespace mhyprotect
{
	typedef struct _MHYPROT_INITIALIZE
	{
		DWORD		_m_001;
		DWORD		_m_002;
		DWORD64		_m_003;
	} MHYPROT_INITIALIZE, *PMHYPROT_INITIALIZE;

	typedef struct _MHYPROT_KERNEL_READ_REQUEST
	{
		union _HEADER
		{
			DWORD		result;
			DWORD64		address;
		} header;
		ULONG size;
	} MHYPROT_KERNEL_READ_REQUEST, *PMHYPROT_KERNEL_READ_REQUEST;

	typedef struct _MHYPROT_USER_READ_WRITE_REQUEST
	{
		DWORD64 random_key;
		DWORD action;
		DWORD unknown_00;
		DWORD process_id;
		DWORD unknown_01;
		DWORD64 buffer;
		DWORD64 address;
		ULONG size;
		ULONG unknown_02;
	} MHYPROT_USER_READ_WRITE_REQUEST, *PMHYPROT_USER_READ_WRITE_REQUEST;

	typedef struct _MHYPROT_TERMINATE_PROCESS_REQUEST
	{
		uint64_t response;
		uint32_t process_id;
	} MHYPROT_TERMINATE_PROCESS_REQUEST, * PMHYPROT_TERMINATE_PROCESS_REQUEST;

	namespace detail
	{
		inline HANDLE device_handle;
		inline uint64_t seedmap[312];
		inline SC_HANDLE mhyplot_service_handle;
	}

	bool init();
	void unload();
	void clean();

	namespace driver_impl
	{
		bool request_ioctl(DWORD ioctl_code, LPVOID in_buffer, DWORD in_buffer_size);
		bool driver_init();
		uint64_t generate_key(uint64_t seed);
		void encrypt_payload(void* payload, size_t size);

		bool read_kernel_memory(uint64_t address, void* buffer, size_t size);
		template<class T> __forceinline T read_kernel_memory(uint64_t address)
		{
			T buffer;
			read_kernel_memory(address, &buffer, sizeof(T));
			return buffer;
		}

		bool read_user_memory(uint32_t process_id, uint64_t address, void* buffer, size_t size);
		template<class T> __forceinline T read_user_memory(uint32_t process_id, uint64_t address)
		{
			T buffer;
			read_user_memory(process_id, address, &buffer, sizeof(T));
			return buffer;
		}

		bool write_user_memory(uint32_t process_id, uint64_t address, void* buffer, size_t size);
		template<class T> __forceinline bool write_user_memory(uint32_t process_id, uint64_t address, T value)
		{
			return write_user_memory(process_id, address, &value, sizeof(T));
		}

		bool terminate_process(const uint32_t process_id);
	}
}
