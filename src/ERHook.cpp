/**
 * @file ERHook.cpp
 * @author Asiern
 * @brief
 * @version 0.1
 * @date 2022-01-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ERHook.hpp"

void ERHook::getProcessID(void)
{
    HWND hwnd = FindWindowA(NULL, ""); // TODO set window name
    if (hwnd == NULL)
        return;

    DWORD pID;                                                    // Process ID
    GetWindowThreadProcessId(hwnd, &pID);                         // Get Process ID
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID); // Open process
    if (pHandle == INVALID_HANDLE_VALUE)
        return;

    this->PID = pID;
    CloseHandle(pHandle);
}

void ERHook::getBaseAddress(const wchar_t* modName)
{
    // TODO getBaseAddress
}

void ERHook::getGameVersion(void)
{
    // TODO getGameVersion
}

ERHook ::ERHook()
{
    // TODO ERHook
}

ERHook ::~ERHook()
{
    // TODO ~ERHook
}

void ERHook::start(void)
{
    // TODO start
}

void ERHook::stop(void)
{
    // TODO stop
}