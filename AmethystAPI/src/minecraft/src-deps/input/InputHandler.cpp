#include "minecraft/src-deps/input/InputHandler.h"
#include "minecraft/src-deps/core/string/StringHash.h"

// This seems to be a 32 bit variation of a hashed string?
int StringToNameId(const std::string& str) {
    int hashed = -2128831035;

    for (char c : str) {
        hashed = 16777619 * (hashed ^ c);
    }

    return hashed;
}

void InputHandler::registerButtonDownHandler(std::string buttonName, std::function<void(FocusImpact, IClientInstance&)> handler, bool suspendable)
{
    this->mButtonDownHandlerMap.emplace(StringToNameId(buttonName), std::make_pair(suspendable, std::move(handler)));
}

void InputHandler::registerButtonUpHandler(std::string buttonName, std::function<void(FocusImpact, IClientInstance&)> handler, bool suspendable)
{
    this->mButtonUpHandlerMap.emplace(StringToNameId(buttonName), std::make_pair(suspendable, std::move(handler)));
}

void InputHandler::unregisterHandlersByName(const std::string& buttonName) {
    int hashedName = StringToNameId(buttonName);

    auto downHandlerRange = mButtonDownHandlerMap.equal_range(hashedName);
    if (downHandlerRange.first != mButtonDownHandlerMap.end()) {
        mButtonDownHandlerMap.erase(downHandlerRange.first, downHandlerRange.second);
    }

    auto upHandlerRange = mButtonUpHandlerMap.equal_range(hashedName);
    if (upHandlerRange.first != mButtonUpHandlerMap.end()) {
        mButtonUpHandlerMap.erase(upHandlerRange.first, upHandlerRange.second);
    }
}