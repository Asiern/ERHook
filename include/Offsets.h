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
        offs->characterName = 0x7FF3B317C79C;
        offs->soulLevel = 0x7FF3B317C768;
        break;
    default:
        break;
    }
}

#endif;