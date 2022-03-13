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
    // HWND hwnd = FindWindow(NULL, "ELDEN RING™");
    HWND hwnd = FindWindowW(NULL, L"\u0045\u004c\u0044\u0045\u004e\u0020\u0052\u0049\u004e\u0047\u2122");
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
                    this->modBaseSize = modEntry.modBaseSize;
                    return true;
                }
            } while (Module32NextW(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return false;
}

/**
 * @brief Get game version
 *
 * @return true if version found
 * @return false if version not found
 */
bool ERHook::getGameVersion(void)
{
    // TODO getGameVersion
    return true;
}

/**
 * @brief Scan process module by AOB(Array of bytes)
 *
 * @param base Start address
 * @param size Number of bytes to scan
 * @param pattern AOB pattern (48 ?? 40 => /x48/xFF/x40)
 * @param mask AOB mask (48 ?? 40 => x?x)
 * @return void* AOB address uintptr_t
 */
void* ERHook::aobScan(char* base, size_t size, char* pattern, char* mask)
{
    size_t patternLength = strlen(pattern);

    for (unsigned int i = 0; i < this->modBaseSize - patternLength; i++)
    {
        bool match = true;
        for (unsigned int j = 0; j < patternLength; j++)
        {
            if (i + j == size)
                return nullptr;
            if (mask[j] != '?' && pattern[j] != base[i + j])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            return (void*)(base + i);
        }
    }
    return nullptr;
}

/**
 * @brief Scan process by AOB(Array of bytes)
 *
 * @param begin Start memory address
 * @param end End memory address
 * @param pattern AOB pattern (48 ?? 40 => /x48/xFF/x40)
 * @param mask AOB mask (48 ?? 40 => x?x)
 * @return void* AOB address uintptr_t
 */
void* ERHook::aobScanEx(uintptr_t begin, uintptr_t end, char* pattern, char* mask)
{
    uintptr_t current = begin;
    SIZE_T bytesRead;
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->PID);
    DWORD oldprotect;
    char buffer[4096];

    while (current < end)
    {
        VirtualProtectEx(pHandle, (void*)current, sizeof(buffer), PROCESS_ALL_ACCESS, &oldprotect);
        ReadProcessMemory(pHandle, (void*)current, &buffer, sizeof(buffer), &bytesRead);
        VirtualProtectEx(pHandle, (void*)current, sizeof(buffer), oldprotect, NULL);
        if (bytesRead == 0)
        {
            return nullptr;
            CloseHandle(pHandle);
        }

        void* _address = aobScan((char*)&buffer, bytesRead, pattern, mask);

        if (_address != nullptr)
        {
            uintptr_t offset = (uintptr_t)_address - (uintptr_t)&buffer;
            CloseHandle(pHandle);
            return (void*)(current + offset);
        }
        else
        {
            current = current + bytesRead;
        }
    }
    CloseHandle(pHandle);
    return nullptr;
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
 * @return int status code
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

    // Load offsets depending on game version
    if (getGameVersion() == false)
    {
        return ERR_VER_NOT_FOUND;
    }
    loadOffsets(VER_1_02_3, &this->Offsets);

    // AOB Scan
    this->Offsets.gameData.address = (uintptr_t)aobScanEx(this->baseAddress, this->baseAddress + this->modBaseSize,
                                                          this->Offsets.gameData.bytes, this->Offsets.gameData.mask);
    this->Offsets.worldChr.address = (uintptr_t)aobScanEx(this->baseAddress, this->baseAddress + this->modBaseSize,
                                                          this->Offsets.worldChr.bytes, this->Offsets.worldChr.mask);
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
 * @brief Check if process is still active
 *
 */
void ERHook::update(void)
{
    if (this->PID != this->getPID())
        this->hooked = false;
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