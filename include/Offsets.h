/**
 * @file Offsets.h
 * @author Asiern
 * @brief
 * @version 0.1
 * @date 2022-01-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef OFFSETS_H
#define OFFSETS_H
#ifdef USING_WINSOCK2
#include <WinSock2.h>
#else
#include <Windows.h>
#endif;

#define VER_1_02_3 0

typedef struct
{
    char* bytes;
    char* mask;
    uintptr_t address;
} aob;

typedef struct
{
    aob worldChr;
    aob gameData;
    DWORD characterName;
    DWORD soulLevel;
    DWORD health;
    DWORD focus;
    DWORD stamina;
    DWORD saveSlot;
    DWORD lastGrace;
    DWORD playTime;

} offsets;

// TODO loadOffsets
inline void loadOffsets(int versionCode, offsets* offs)
{
    switch (versionCode)
    {
    case VER_1_02_3:
        offs->worldChr.bytes = "\x48\x8B\x05\xFF\xFF\xFF\xFF\x48\x85\xC0\x74\x0F\x48\x39\x88\xFF\xFF\xFF\xFF\x75\x06"
                               "\x89\xB1\x5C\x03\x00\x00\x0F\x28\x05\xFF\xFF\xFF\xFF\x4C\x8D\x45\xE7";
        offs->worldChr.mask = "___????________????___________????____";
        offs->gameData.bytes = "\x48\x8B\x05\xFF\xFF\xFF\xFF\x48\x85\xC0\x74\x05\x48\x8B\x40\x58\xC3\xC3";
        offs->gameData.mask = "___????___________";
        break;
    default:
        break;
    }
}

#endif;