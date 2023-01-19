#include <3ds.h>
#include "csvc.h"
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

#include "Stuff.hpp"

Config* config;
using Megamix::btks;

const char* version = VERSION;

// tired of typing these names
namespace ctrpf = CTRPluginFramework;
namespace CTRPluginFramework {
    void PatchProcess(FwkSettings &settings);
    void OnProcessExit(void);
    int main(void);
}

// This patches the NFC disabling the touchscreen when scanning an amiibo, which prevents ctrpf to be used
// TODO: maybe move this somewhere else?
static void ToggleTouchscreenForceOn(void) {
    static u32 original = 0;
    static u32 *patchAddress = nullptr;

    if (patchAddress && original)
    {
        *patchAddress = original;
        return;
    }

    static const std::vector<u32> pattern =
    {
        0xE59F10C0, 0xE5840004, 0xE5841000, 0xE5DD0000,
        0xE5C40008, 0xE28DD03C, 0xE8BD80F0, 0xE5D51001,
        0xE1D400D4, 0xE3510003, 0x159F0034, 0x1A000003
    };

    Result  res;
    Handle  processHandle;
    s64     textTotalSize = 0;
    s64     startAddress = 0;
    u32 *   found;

    if (R_FAILED(svcOpenProcess(&processHandle, 16)))
        return;

    svcGetProcessInfo(&textTotalSize, processHandle, 0x10002);
    svcGetProcessInfo(&startAddress, processHandle, 0x10005);
    if(R_FAILED(svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, processHandle, (u32)startAddress, textTotalSize)))
        goto exit;

    found = (u32 *)Utils::Search<u32>(0x14000000, (u32)textTotalSize, pattern);

    if (found != nullptr)
    {
        original = found[13];
        patchAddress = (u32 *)PA_FROM_VA((found + 13));
        found[13] = 0xE1A00000;
    }

    svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, textTotalSize);
    exit:
    svcCloseHandle(processHandle);
}

// This function is called before main and before the game starts
// Useful to do code edits safely
void ctrpf::PatchProcess(ctrpf::FwkSettings &settings) {
    ToggleTouchscreenForceOn();

    // Crash handler (WIP)
    Process::exceptionCallback = Megamix::CrashHandler;

    // Init region and config
    region = ctrpf::Process::GetTitleID();
    config = Config::FromFile(MEGAMIX_CONFIG_PATH);

    // Start hooks
    Megamix::Hooks::TickflowHooks();
    if (region != Region::JP) //TODO
        Megamix::Hooks::TempoHooks();
}

// This function is called when the process exits
// Useful to save settings, undo patchs or clean up things
void ctrpf::OnProcessExit(void) {
    Megamix::Hooks::DisableAllHooks(); // Probably not needed, but still
    ToggleTouchscreenForceOn();
    delete config;
}

#ifndef RELEASE
void InitMenu(ctrpf::PluginMenu &menu) {
    menu += new ctrpf::MenuEntry("Config values", nullptr, [](ctrpf::MenuEntry *entry) {
        ctrpf::MessageBox("Settings", Utils::Format(
            "Result: %d",
            configResult
        ))();
    });

    menu += new ctrpf::MenuEntry("Tickflow contents", nullptr, [](ctrpf::MenuEntry *entry) {
        ctrpf::MessageBox("Map shit", Stuff::FileMapToString(config->tickflows))();
    });

    menu += new ctrpf::MenuEntry("Tempo contents (do this w a loaded btks)", nullptr, [](ctrpf::MenuEntry *entry) {
        ctrpf::MessageBox("Map shit", Stuff::TempoMapToString(btks.tempos))();
    });
}
#endif

int ctrpf::main(void) {
    
#ifdef RELEASE
    Process::WaitForExit();
#else
    PluginMenu *menu = new PluginMenu(Utils::Format("Saltwater %s debug", VERSION), "", 1);

    // Synnchronize the menu with frame event
    menu->SynchronizeWithFrame(true);

    // Init our menu entries & folders
    InitMenu(*menu);

    // Launch menu and mainloop
    menu->Run();

    delete menu;
#endif

    return 0;
}
