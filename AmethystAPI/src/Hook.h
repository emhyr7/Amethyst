#pragma once
#include <cstdint>
#include <Windows.h>
#include "MinHook.h"
#include "Log.h"
#include <psapi.h>
#include <chrono>
#include <vector>

/*
Returns the position where Minecraft has been loaded into memory
*/
uintptr_t GetMinecraftBaseAddress();

/*
Returns the size of the game (in bytes) while loaded in memory
*/
unsigned long GetMinecraftSize();

/*
Offsets an address from the game binary, with the position the game has been loaded into memory at
*/
uintptr_t SlideAddress(uintptr_t offset);

/*
Finds an address of a function with its signature within the loaded game memory
*/
uintptr_t SigScan(std::string signature);

template <typename T>
void CreateHook(uintptr_t targetAddress, void* detour, T& original) {
    LPVOID address = reinterpret_cast<LPVOID>(targetAddress);

    MH_STATUS status = MH_CreateHook(address, detour, reinterpret_cast<LPVOID*>(&original));

    if (status != MH_OK) {
        // When loaded for the first time MH will not be initialized
        if (status == MH_ERROR_NOT_INITIALIZED) {
            status = MH_Initialize();

            // Log error's if Initialization failed
            if (status != MH_OK) {
                Log::Error("MH_Initialize failed! Reason: {}\n", MH_StatusToString(status));
                throw std::exception();
            }

            // Try creating the hook again
            status = MH_CreateHook(address, detour, reinterpret_cast<LPVOID*>(&original));
            if (status != MH_OK) {
                Log::Error("MH_CreateHook failed! Reason: {}\n", MH_StatusToString(status));
                throw std::exception();
            }
        }
        else {
            Log::Error("MH_CreateHook failed! Reason: {}\n", MH_StatusToString(status));
            throw std::exception();
        }
    }

    status = MH_EnableHook(address);
    if (status != MH_OK) {
        Log::Error("MH_EnableHook failed! Reason: {}\n", MH_StatusToString(status));
        throw std::exception();
    }
}