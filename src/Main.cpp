#include "ERHook.hpp"
#include <TlHelp32.h>
#include <Windows.h>
#include <iostream>
#include <thread>
using namespace std;

// Function used to exit the program
void ENDPressed(ERHook* hook)
{
    while (true)
    {
        if (GetKeyState(VK_END) & 0x8000) // END button pressed
        {
            // Stop hook
            hook->stop();
            return; // exit function
        }
    }
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Look for memory leaks

    ERHook hook;
    cout << "Hooking..." << endl;
    // Hook to process
    while (!hook.isHooked())
    {
        hook.start();
    }
    cout << "Hooked" << endl;

    // Create a thread to exit when the 'END' button is pressed
    std::thread exitThread(ENDPressed, &hook);

    // Print some values
    while (hook.isHooked())
    {
        std::cout << "Press END to exit..." << std::endl;
        Sleep(500);
        system("cls");
    }

    // Join thread and exit
    exitThread.join();
    return 0;
}