#include <3ds.h>
#include "CTRPF.hpp"

#include "csvc.h"
#include "external/plgldr.h"

#include "Megamix.hpp"
#include "Config.hpp"
#include "Saltwater.hpp"
#include "Stuff.hpp"

const char* version = VERSION;

SaltwaterParams params;

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

    found = (u32 *)CTRPF::Utils::Search<u32>(0x14000000, (u32)textTotalSize, pattern);

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
void CTRPF::PatchProcess(CTRPF::FwkSettings&) {
    ToggleTouchscreenForceOn();

    // plugin params - these are used for shorter types (bools usually)
    params = *reinterpret_cast<SaltwaterParams*>(CTRPF::FwkSettings::Header->config);

    if (params.rhmpatch) {
        // barista moved RHMPatch's code.ips, move it back to where it was
        CTRPF::File::Rename(
            "/luma/titles/000400000018a400/code.old.ips",
            "/luma/titles/000400000018a400/code.ips"
        );
    }

    if (params.plgldr) {
        // had to specifically turn on the plgldr, turn it off to not cause any issues
        plgLdrInit();
        PLGLDR__SetPluginLoaderState(false);
        plgLdrExit(); 
    }

    // Init region and config
    auto region_res = Megamix::initGameInterface(CTRPF::Process::GetTitleID());
    if (!region_res.has_value()) {
        Megamix::panic("what the hell how did you get this\nyou're running saltwater on something that isn't megamix");
    }
    region = Region::FromCode(CTRPF::Process::GetTitleID()); //TODO: remove
    config = Config::FromFile(MEGAMIX_CONFIG_PATH);

    // Start hooks, apply patches
    Megamix::Hooks::initTickflowHooks();
    Megamix::Hooks::initRegionHooks();
    Megamix::Patches::PatchRetryRemix();
    if (region != Region::JP) {
        //TODO: find out how to make the tempo hooks JP-compatible
        Megamix::Hooks::initTempoHooks();
        //TODO: find out how to make the tickflow commands hook JP-compatible
        Megamix::Hooks::initCommandHooks();
    }

    if (region != Region::JP && params.extra_rows) {
        Megamix::Patches::PatchMuseumExtraRows();
    }
}

// This function is called when the process exits
// Useful to save settings, undo patchs or clean up things
void CTRPF::OnProcessExit(void) {
    Megamix::Hooks::disableAllHooks(); // Probably not needed, but still
    ToggleTouchscreenForceOn();
}

#ifndef RELEASE
void InitMenu(CTRPF::PluginMenu &menu) {
    menu += new CTRPF::MenuEntry("Config values", nullptr, [](CTRPF::MenuEntry*) {
        CTRPF::MessageBox("Settings", Format(
            "Result: %d",
            configResult
        ))();
    });

    menu += new CTRPF::MenuEntry("Tickflow contents", nullptr, [](CTRPF::MenuEntry*) {
        CTRPF::MessageBox("Map shit", Stuff::FileMapToString(config.tickflows))();
    });

    menu += new CTRPF::MenuEntry("Tempo contents (do this w a loaded btks)", nullptr, [](CTRPF::MenuEntry*) {
        CTRPF::MessageBox("Map shit", Stuff::TempoMapToString(Megamix::btks.tempos))();
    });

    menu += new CTRPF::MenuEntry("Force a crash (prefetch)", nullptr, [](CTRPF::MenuEntry*) {
        ((void(*)(void))nullptr)();
    });

    menu += new CTRPF::MenuEntry("Force a crash (data)", nullptr, [](CTRPF::MenuEntry*) {
        *(volatile int*)nullptr = 100;
    });
}
#endif

int CTRPF::main(void) {
    // Crash handler
    Process::exceptionCallback = Megamix::CrashHandler;

    if (params.barista != 0xD06) {
        Megamix::panic("You seem to be running Saltwater as a regular CTRPlugin. You must run Saltwater from the Barista launcher!");
    }

#ifdef RELEASE
    Process::WaitForExit();
#else
    PluginMenu *menu = new PluginMenu(Format("Saltwater %s debug", VERSION), "", 1);

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
