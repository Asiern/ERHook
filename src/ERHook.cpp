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
#include <iostream>

/**
 * @brief Get process id
 *
 * @return true if PID found
 * @return false if PID not found
 */
bool ERHook::getProcessID(void)
{
    HWND hwnd = FindWindow(NULL, "ELDEN RING�");
    if (hwnd == NULL)
    {
        std::cout << "Window not found" << std::endl;
        return false;
    }

    DWORD pID;                                                    // Process ID
    GetWindowThreadProcessId(hwnd, &pID);                         // Get Process ID
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID); // Open process
    if (pHandle == INVALID_HANDLE_VALUE)
        return false;

    this->PID = pID;
    std::cout << "PID: " << pID << std::endl;
    CloseHandle(pHandle);
    return true;
}

/**
 * @brief Get module start address
 *
 * @param modName modeule name
 * @return true if addres found
 * @return false if address not found
 */
bool ERHook::getBaseAddress(const wchar_t* modName)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->PID);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32W modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32FirstW(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    this->baseAddress = (uintptr_t)modEntry.modBaseAddr;
                    return true;
                }
            } while (Module32NextW(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return false;
}

bool ERHook::getGameVersion(void)
{
    // TODO getGameVersion
    return false;
}

/**
 * @brief Write BYTES to memory
 *
 * @param destination destination address
 * @param src source address
 * @param size number of bytes to write
 */
void ERHook::patch(BYTE* destination, BYTE* src, unsigned int size)
{
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->PID);
    DWORD oldprotection;
    VirtualProtectEx(pHandle, destination, size, PAGE_EXECUTE_READWRITE, &oldprotection);
    WriteProcessMemory(pHandle, destination, src, size, nullptr);
    VirtualProtectEx(pHandle, destination, size, oldprotection, &oldprotection);
    CloseHandle(pHandle);
}

/**
 * @brief Read string values from memory
 *
 * @param address
 * @return std::string
 */
std::string ERHook::readMemoryString(uintptr_t address)
{
    char val[20];
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->PID);
    ReadProcessMemory(pHandle, (LPCVOID)address, &val, sizeof(val), NULL);
    CloseHandle(pHandle);
    return std::string(val);
}

/**
 * @brief Write strings to memory
 *
 * @param address
 * @param value
 */
void ERHook::writeMemoryString(uintptr_t address, std::string value)
{
    SIZE_T BytesToWrite = value.length() + 1;
    SIZE_T BytesWritten;
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->PID);
    WriteProcessMemory(pHandle, (LPVOID)address, (LPCVOID)value.c_str(), BytesToWrite, &BytesWritten);
    CloseHandle(pHandle);
}

ERHook::ERHook()
{
    // TODO ERHook
    this->hooked = false;
}

ERHook::~ERHook()
{
    // TODO ~ERHook
}

/**
 * @brief Launch hook
 *
 * @return int status
 */
int ERHook::start(void)
{
    // Get PID
    if (getProcessID() == false)
    {
        return ERR_PID_NOT_FOUND;
    }

    // Get Base Address
    if (getBaseAddress(L"eldenring.exe") == false)
    {
        return ERR_MOD_ADD_NOT_FOUND;
    }

    std::cout << "BASE: 0x" << this->baseAddress << std::endl;

    //// Set proper offsets
    // if (getGameVersion() == false)
    //{
    //     return ERR_VER_NOT_FOUND;
    // }
    loadOffsets(VER_1_02_3, &this->Offsets);

    // Change hooked status
    hooked = true;
    return SUCCESS;
}

/**
 * @brief Stop hook
 *
 * @return int status
 */
int ERHook::stop(void)
{
    // TODO stop

    // Change hooked status
    this->hooked = false;

    return SUCCESS;
}

/**
 * @brief Get hook status
 *
 * @return true if hooked
 * @return false if not hooked
 */
bool ERHook::isHooked(void)
{
    return this->hooked;
}

/**
 * @brief Get process id
 *
 * @return DWORD Process ID
 */
DWORD ERHook::getPID(void)
{
    return this->PID;
}