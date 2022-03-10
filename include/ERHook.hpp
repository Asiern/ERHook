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

#ifdef USING_WINSOCK2
#include <WinSock2.h>
#else
#include <Windows.h>
#endif;

#include <TlHelp32.h>

#include <string>

#include "Info.h"
#include "Offsets.h"

// Error codes
#define SUCCESS 0
#define ERR_PID_NOT_FOUND 1
#define ERR_MOD_ADD_NOT_FOUND 2
#define ERR_VER_NOT_FOUND 3

class ERHook
{
    DWORD PID;             // Process ID
    uintptr_t baseAddress; // Module base address
    info Info;             // Game info
    offsets Offsets;       // Memory offsets
    bool hooked;           // Hook status

    bool getProcessID(void);
    bool getBaseAddress(const wchar_t* modName);
    bool getGameVersion(void);

    // MemoryTools
    template <typename T> T readMemory(uintptr_t address);
    template <typename T> void writeMemory(uintptr_t address, T value);
    std::string ERHook::readMemoryString(uintptr_t address);
    void ERHook::writeMemoryString(uintptr_t address, std::string value);
    void ERHook::patch(BYTE* destination, BYTE* src, unsigned int size);

  public:
    ERHook();
    ~ERHook();
    int start(void);
    int stop(void);
    bool isHooked(void);
    DWORD getPID(void);
};

template <typename T> inline T ERHook::readMemory(uintptr_t address)
{
    T value;
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->_pID);
    ReadProcessMemory(pHandle, (LPCVOID)(address), &value, sizeof(value), NULL);
    CloseHandle(pHandle);
    return value;
}

template <typename T> inline void ERHook::writeMemory(uintptr_t address, T value)
{
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, this->_pID);
    WriteProcessMemory(pHandle, (LPVOID)(address), &value, sizeof(value), NULL);
    CloseHandle(pHandle);
}
#endif