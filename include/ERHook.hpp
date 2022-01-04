/**
 * @file ERHook.hpp
 * @author Asiern
 * @brief
 * @version 0.1
 * @date 2022-01-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ERHOOK_H
#define ERHOOK_H

#include <Windows.h>

#include <TlHelp32.h>

#include "Info.h"
#include "Offsets.h"

class ERHook
{
    DWORD PID;
    uintptr_t baseAddress;
    info Info;
    offsets Offsets;
    void getProcessID(void);
    void getBaseAddress(const wchar_t* modName);
    void getGameVersion(void);

  public:
    ERHook();
    ~ERHook();
    void start(void);
    void stop(void);
};

#endif