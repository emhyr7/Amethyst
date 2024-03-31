#pragma once
#include <vector>
#include "minecraft/src-client/common/client/options/Options.h"
#include "amethyst/Log.h"
#include <minecraft/src-client/common/client/input/ClientInputHandler.h>

class AmethystContext;

namespace Amethyst {
    class InputManager {
    public:
        InputManager(AmethystContext* amethyst);
        void RegisterNewInput(std::string actionName, std::vector<int> keys, bool allowRemapping = true);
        void AddButtonDownHandler(const std::string& actionName, std::function<void(FocusImpact, IClientInstance&)> handler, bool suspendable);
        void AddButtonUpHandler(const std::string& actionName, std::function<void(FocusImpact, IClientInstance&)> handler, bool suspendable);
        void Shutdown();

    private:
        void RemoveButtonHandlers();

    public:
        std::vector<std::string> mRegisteredInputs;

    private:
        AmethystContext* mAmethyst;
    };
}