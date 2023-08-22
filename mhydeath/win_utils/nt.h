#pragma once
#include <Windows.h>

//
// windows native definitions
//

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L)
#define STATUS_UNSUCCESSFUL              ((NTSTATUS)0xC0000001L)
#define STATUS_NOT_IMPLEMENTED           ((NTSTATUS)0xC0000002L)
#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)
#define STATUS_INVALID_CID               ((NTSTATUS)0xC000000BL)
#define STATUS_NO_SUCH_DEVICE            ((NTSTATUS)0xC000000EL)
#define STATUS_NO_SUCH_FILE              ((NTSTATUS)0xC000000FL)
#define STATUS_INVALID_DEVICE_REQUEST    ((NTSTATUS)0xC0000010L)
#define STATUS_MORE_PROCESSING_REQUIRED  ((NTSTATUS)0xC0000016L)
#define STATUS_CONFLICTING_ADDRESSES     ((NTSTATUS)0xC0000018L)
#define STATUS_NO_MORE_ENTRIES           ((NTSTATUS)0x8000001AL)
#define STATUS_BUFFER_TOO_SMALL          ((NTSTATUS)0xC0000023L)
#define STATUS_INVALID_PAGE_PROTECTION   ((NTSTATUS)0xC0000045L)
#define STATUS_PROCEDURE_NOT_FOUND       ((NTSTATUS)0xC000007AL)
#define STATUS_INSUFFICIENT_RESOURCES    ((NTSTATUS)0xC000009AL)
#define STATUS_INSTRUCTION_MISALIGNMENT  ((NTSTATUS)0xC00000AAL)
#define STATUS_INTERNAL_ERROR            ((NTSTATUS)0xC00000E5L)
#define STATUS_INVALID_PARAMETER_1       ((NTSTATUS)0xC00000EFL)
#define STATUS_INVALID_PARAMETER_2       ((NTSTATUS)0xC00000F0L)
#define STATUS_INVALID_PARAMETER_3       ((NTSTATUS)0xC00000F1L)
#define STATUS_INVALID_PARAMETER_4       ((NTSTATUS)0xC00000F2L)
#define STATUS_INVALID_PARAMETER_5       ((NTSTATUS)0xC00000F3L)
#define STATUS_INVALID_PARAMETER_6       ((NTSTATUS)0xC00000F4L)
#define STATUS_INVALID_PARAMETER_7       ((NTSTATUS)0xC00000F5L)
#define STATUS_INVALID_PARAMETER_8       ((NTSTATUS)0xC00000F6L)
#define STATUS_INVALID_PARAMETER_9       ((NTSTATUS)0xC00000F7L)
#define STATUS_INVALID_PARAMETER_10      ((NTSTATUS)0xC00000F8L)
#define STATUS_INVALID_PARAMETER_11      ((NTSTATUS)0xC00000F9L)
#define STATUS_INVALID_PARAMETER_12      ((NTSTATUS)0xC00000FAL)
#define STATUS_INVALID_ADDRESS           ((NTSTATUS)0xC0000141L)
#define STATUS_DATATYPE_MISALIGNMENT_ERROR ((NTSTATUS)0xC00002C5L)

typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemBasicInformation                  = 0,
    SystemProcessorInformation              = 1,
    SystemPerformanceInformation            = 2,
    SystemTimeOfDayInformation              = 3,
    SystemPathInformation                   = 4,
    SystemProcessInformation                = 5,
    SystemCallCountInformation              = 6,
    SystemDeviceInformation                 = 7,
    SystemProcessorPerformanceInformation   = 8,
    SystemFlagsInformation                  = 9,
    SystemCallTimeInformation               = 10,
    SystemModuleInformation                 = 11,
    SystemLocksInformation                  = 12,
    SystemStackTraceInformation             = 13,
    SystemPagedPoolInformation              = 14,
    SystemNonPagedPoolInformation           = 15,
    SystemHandleInformation                 = 16,
    SystemObjectInformation                 = 17,
    SystemPageFileInformation               = 18,
    SystemVdmInstemulInformation            = 19,
    SystemVdmBopInformation                 = 20,
    SystemFileCacheInformation              = 21,
    SystemPoolTagInformation                = 22,
    SystemInterruptInformation              = 23,
    SystemDpcBehaviorInformation            = 24,
    SystemFullMemoryInformation             = 25,
    SystemLoadGdiDriverInformation          = 26,
    SystemUnloadGdiDriverInformation        = 27,
    SystemTimeAdjustmentInformation         = 28,
    SystemSummaryMemoryInformation          = 29,
    SystemMirrorMemoryInformation           = 30,
    SystemPerformanceTraceInformation       = 31,
    SystemObsolete0                         = 32,
    SystemExceptionInformation              = 33,
    SystemCrashDumpStateInformation         = 34,
    SystemKernelDebuggerInformation         = 35,
    SystemContextSwitchInformation          = 36,
    SystemRegistryQuotaInformation          = 37,
    SystemExtendServiceTableInformation     = 38,
    SystemPrioritySeperation                = 39,
    SystemVerifierAddDriverInformation      = 40,
    SystemVerifierRemoveDriverInformation   = 41,
    SystemProcessorIdleInformation          = 42,
    SystemLegacyDriverInformation           = 43,
    SystemCurrentTimeZoneInformation        = 44,
    SystemLookasideInformation              = 45,
    SystemTimeSlipNotification              = 46,
    SystemSessionCreate                     = 47,
    SystemSessionDetach                     = 48,
    SystemSessionInformation                = 49,
    SystemRangeStartInformation             = 50,
    SystemVerifierInformation               = 51,
    SystemVerifierThunkExtend               = 52,
    SystemSessionProcessInformation         = 53,
    SystemLoadGdiDriverInSystemSpace        = 54,
    SystemNumaProcessorMap                  = 55,
    SystemPrefetcherInformation             = 56,
    SystemExtendedProcessInformation        = 57,
    SystemRecommendedSharedDataAlignment    = 58,
    SystemComPlusPackage                    = 59,
    SystemNumaAvailableMemory               = 60,
    SystemProcessorPowerInformation         = 61,
    SystemEmulationBasicInformation         = 62,
    SystemEmulationProcessorInformation     = 63,
    SystemExtendedHandleInformation         = 64,
    SystemLostDelayedWriteInformation       = 65,
    SystemBigPoolInformation                = 66,
    SystemSessionPoolTagInformation         = 67,
    SystemSessionMappedViewInformation      = 68,
    SystemHotpatchInformation               = 69,
    SystemObjectSecurityMode                = 70,
    SystemWatchdogTimerHandler              = 71,
    SystemWatchdogTimerInformation          = 72,
    SystemLogicalProcessorInformation       = 73,
    SystemWow64SharedInformation            = 74,
    SystemRegisterFirmwareTableInformationHandler = 75,
    SystemFirmwareTableInformation          = 76,
    SystemModuleInformationEx               = 77,
    SystemVerifierTriageInformation         = 78,
    SystemSuperfetchInformation             = 79,
    SystemMemoryListInformation             = 80,
    SystemFileCacheInformationEx            = 81,
    MaxSystemInfoClass                      = 82
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY
{
    ULONG Unknow1;
    ULONG Unknow2;
    ULONG Unknow3;
    ULONG Unknow4;
    PVOID DllBase;
    ULONG Size;
    ULONG Flags;
    USHORT Index;
    USHORT NameLength;
    USHORT LoadCount;
    USHORT ModuleNameOffset;
    char ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, * PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION
{
    ULONG Count;
    SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

typedef NTSTATUS(WINAPI* pNtQuerySystemInformation)(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID                   SystemInformation,
    IN ULONG                    SystemInformationLength,
    OUT PULONG                  ReturnLength
);
