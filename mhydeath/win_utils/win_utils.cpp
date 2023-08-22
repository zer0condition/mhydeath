#include "win_utils.h"

//
// find the process id by specific name using ToolHelp32Snapshot
//
uint32_t win_utils::find_process_id(const std::string_view process_name)
{
	PROCESSENTRY32 processentry = {};

	const unique_handle snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0), &CloseHandle);

	if (!CHECK_HANDLE(snapshot.get()))
	{
		//(("[!] Failed to create ToolHelp32Snapshot [0x%lX]\n"), GetLastError());
		return 0;
	}

	processentry.dwSize = sizeof(MODULEENTRY32);

	while (Process32Next(snapshot.get(), &processentry) == TRUE)
	{
		if (process_name.compare(processentry.szExeFile) == 0)
		{
			return processentry.th32ProcessID;
		}
	}

	return 0;
}

//
// lookup base address of specific module that loaded in the system
// by NtQuerySystemInformation api
//
uint64_t win_utils::obtain_sysmodule_address(const std::string_view target_module_name)
{
	const HMODULE module_handle = GetModuleHandle(TEXT(("ntdll.dll")));

	if (!CHECK_HANDLE(module_handle))
		return 0;

	pNtQuerySystemInformation NtQuerySystemInformation = (pNtQuerySystemInformation)GetProcAddress(module_handle, "NtQuerySystemInformation");

	if (!NtQuerySystemInformation)
		return 0;

	NTSTATUS status;
	PVOID buffer;
	ULONG alloc_size = 0x10000;
	ULONG needed_size;

	do
	{
		buffer = calloc(1, alloc_size);

		if (!buffer)
			return 0;

		status = NtQuerySystemInformation(SystemModuleInformation, buffer, alloc_size, &needed_size);

		if (!NT_SUCCESS(status) && status != STATUS_INFO_LENGTH_MISMATCH)
		{
			free(buffer);
			return 0;
		}

		if (status == STATUS_INFO_LENGTH_MISMATCH)
		{
			free(buffer);
			buffer = NULL;
			alloc_size *= 2;
		}

	} while (status == STATUS_INFO_LENGTH_MISMATCH);

	if (!buffer)
		return 0;

	PSYSTEM_MODULE_INFORMATION module_information = (PSYSTEM_MODULE_INFORMATION)buffer;


	for (ULONG i = 0; i < module_information->Count; i++)
	{
		SYSTEM_MODULE_INFORMATION_ENTRY module_entry = module_information->Module[i];
		ULONG_PTR module_address = (ULONG_PTR)module_entry.DllBase;

		if (module_address < MIN_ADDRESS)
			continue;

		PCHAR module_name = module_entry.ImageName + module_entry.ModuleNameOffset;

		if (target_module_name.compare(module_name) == 0 || std::string(module_name).find(("mhyprot")) != std::string::npos)
			return module_address;
	}

	free(buffer);
	return 0;
}