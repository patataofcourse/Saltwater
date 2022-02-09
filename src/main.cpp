#include <3ds.h>
#include "csvc.h"
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

int C00Result;
u32 region;
Config* config;

namespace CTRPluginFramework
{
    // This patch the NFC disabling the touchscreen when scanning an amiibo, which prevents ctrpf to be used
    static void    ToggleTouchscreenForceOn(void)
    {
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
    void    PatchProcess(FwkSettings &settings)
    {
        ToggleTouchscreenForceOn();

        // Init region and config
        region = CTRPluginFramework::Process::GetTitleID();
        config = Config::FromFile("/spicerack/saltwater.cfg", region);

        // RHMPatch recreation
        C00Result = Megamix::LoadC00Bin(region);
        if (C00Result == 0) {
            Megamix::PatchTickflowAddresses(region, config);
        }
    }

    // This function is called when the process exits
    // Useful to save settings, undo patchs or clean up things
    void    OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
    }

    void    InitMenu(PluginMenu &menu)
    {
        // Example entry
        menu += new MenuEntry("RHMPatch load status", nullptr, [](MenuEntry *entry)
        {
            char* out = new char[10];

            sprintf(out, "%d", C00Result);

            MessageBox("C00.bin result", std::string(out))();
        });

        // Example entry
        menu += new MenuEntry("RHMPatch location", nullptr, [](MenuEntry *entry)
        {
            char* out = new char[10];

            sprintf(out, "%x", Megamix::rhmpatchBuffer);

            MessageBox("RHMPatch location", std::string(out))();
        });

        // Example entry
        menu += new MenuEntry("Settings", nullptr, [](MenuEntry *entry)
        {
            char* out = new char[10];

            sprintf(out, "%d/%d/%d\nobviously 1 is true and 0 is false", config->game, config->tempo, config->gate);

            MessageBox("Settings (game/tempo/gate)", std::string(out))();
        });
    }

    int main(void)
    {
        PluginMenu *menu = new PluginMenu("Some thing or another", 0, 0, 1, "h");

        // Synnchronize the menu with frame event
        menu->SynchronizeWithFrame(true);

        // Init our menu entries & folders
        InitMenu(*menu);

        // Launch menu and mainloop
        menu->Run();

        delete menu;

        // Exit plugin
        return (0);
    }
}
