#pragma once
#include <Windows.h>
#include <string>

#include "../win_utils/win_utils.h"
#include "../mhyprotect/mhyprotect.h"


#define CHECK_SC_MANAGER_HANDLE(x, ret_type)												\
if (!CHECK_HANDLE(x))																		\
{																							\
	return ret_type;																		\
}																							\

namespace service_utils
{
	SC_HANDLE open_sc_manager();

	SC_HANDLE create_service(const std::string_view driver_path);
	bool delete_service(SC_HANDLE service_handle, bool close_on_fail = true, bool close_on_success = true);

	bool delete_service_with_check(const std::string_view service_name);

	bool start_service(SC_HANDLE service_handle);
	bool stop_service(SC_HANDLE service_handle);
}