#pragma once

#include <cstdint>

#include <ToolboxPlugin.h>

class IncreaseMinimapClickRange : public ToolboxPlugin {
public:
    const char* Name() const override { return "Increase Minimap Click Range"; }

    void Initialize(ImGuiContext*, ImGuiAllocFns, HMODULE) override;
    void SignalTerminate();
};
