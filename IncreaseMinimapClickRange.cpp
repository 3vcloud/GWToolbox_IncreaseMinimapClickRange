#include "IncreaseMinimapClickRange.h"

#include <GWCA/Utilities/Scanner.h>
#include <GWCA/Utilities/MemoryPatcher.h>

namespace {
    GW::MemoryPatcher minimap_range_click_to_target_patch;
}

DLLAPI ToolboxPlugin* ToolboxPluginInstance()
{
    static IncreaseMinimapClickRange instance;
    return &instance;
}
void IncreaseMinimapClickRange::SignalTerminate() {
    if(minimap_range_click_to_target_patch.GetIsEnable())
        minimap_range_click_to_target_patch.TogglePatch(false);
}

void IncreaseMinimapClickRange::Initialize(ImGuiContext* ctx, ImGuiAllocFns fns, HMODULE toolbox_dll)
{
    ToolboxPlugin::Initialize(ctx, fns, toolbox_dll);

    // Find and replace the max range that minimap targetting works at to be full compass range.
    GW::Scanner::Initialize(toolbox_dll);

    // Disable compiler warnings about how we're using const char arrays here
#pragma warning( push )
#pragma warning( disable : 4838 ) // conversion from 'int' to 'const char' requires a narrowing conversio
#pragma warning( disable : 4309 ) // 'initializing': truncation of constant value

    // 600.f * 600.f == 360000.f == 0x00c8af48
    const char current_minimap_target_range[] = {0x00, 0xc8, 0xaf, 0x48};
    // 5000.f * 5000.f == 25000000.f == 0x20bcbe4b
    const char new_minimap_target_range[] = {0x20, 0xbc, 0xbe, 0x4b};

#pragma warning( pop )

    uintptr_t address_of_range_value = GW::Scanner::Find(current_minimap_target_range, "xxxx",0,GW::Scanner::RDATA);
    if (!address_of_range_value) {
        MessageBox(NULL, Name(), "Failed to get signature for address_of_range_value",0);
        return;
    }

    minimap_range_click_to_target_patch.SetPatch(address_of_range_value, new_minimap_target_range, sizeof(new_minimap_target_range));
    minimap_range_click_to_target_patch.TogglePatch(true);
}
