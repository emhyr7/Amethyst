# Projects

Mono-repository for the core AmethystAPI projects.

**AmethystAPI**: Provides re-usable functionality such as Sigscanning, Hooking and headers for Minecraft.

**AmethystRuntime**: A mod which loads other mods at runtime and manages calling functions like `ModInitialize` and `ModTick`

**AmethystLauncher**: An application which is in charge of starting minecraft and injecting the AmethystRuntime mod

# Creating a mod
```cmake
cmake_minimum_required(VERSION 3.12)
project(ModName) # Replace with the name of your mod
set(MOD_VERSION "@1.0.0") # Replace with the version of your mod, must be prefixed by @

# Define only RelWithDebInfo as the available build configuration
set(CMAKE_CONFIGURATION_TYPES "RelWithDebInfo" CACHE STRING "Build configurations" FORCE)
set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)

# Project Configuration
set(AmethystFolder "$ENV{appdata}/Amethyst")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${AmethystFolder}/mods/${PROJECT_NAME}${MOD_VERSION}")

find_library(AMETHYST_API AmethystAPI PATHS "${AmethystFolder}/lib")
include_directories(${AmethystFolder}/include)

file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")
file(GLOB_RECURSE H_FILES "src/*.h")
add_library(${PROJECT_NAME} SHARED ${CPP_SOURCES} ${H_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${AMETHYST_API})
target_link_libraries(${PROJECT_NAME} PRIVATE "${AmethystFolder}/lib/fmt.lib")
target_link_libraries(${PROJECT_NAME} PRIVATE "${AmethystFolder}/lib/libMinHook.x64.lib")
```

## Mod Functions
A mod can define functions that will be called by AmethystRuntime.
```cpp
// Called when a mod is loaded by AmethystAPI, used to create hooks
extern "C" __declspec(dllexport) void Initialize();

// Called every 50ms, currently not hooked into a real tick function
extern "C" __declspec(dllexport) void OnTick();

// ClientInstance::onStartJoinGame
extern "C" __declspec(dllexport) void OnStartJoinGame(ClientInstance* clientInstance);

// Hooked ScreenView::setupAndRender for mods to draw UI
extern "C" __declspec(dllexport) void OnRenderUI(ScreenView* screenView, UIRenderContext* ctx);

// Used to destroy hooks for hot-reloading
extern "C" __declspec(dllexport) void Shutdown();
```

## Goals of AmethystAPI
 - AmethystAPI does not aim to have completed headers
 - Add to headers as needed for projects
 - Sticking to one version (currently 1.20.51.1)
 - Follow the same folder structure as Minecraft