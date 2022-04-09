#include <3ds.h>
#include "csvc.h"
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

int C00Result;
int btksResult;
Config config;
using Megamix::btks;

namespace CTRPluginFramework
{
    // This patch the NFC disabling the touchscreen when scanning an amiibo, which prevents ctrpf to be used
    static void    ToggleTouchscreenForceOn(void) {
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
    void    PatchProcess(FwkSettings &settings) {
        ToggleTouchscreenForceOn();

        // Init region and config
        region = CTRPluginFramework::Process::GetTitleID();
        config = Config::FromFile("saltwater.cfg");
    
        btksResult = btks.LoadFile("saltwater.btk");
    
        Megamix::Hooks::TickflowHooks();
    }

    // This function is called when the process exits
    // Useful to save settings, undo patchs or clean up things
    void    OnProcessExit(void) {
        Megamix::Hooks::DisableAllHooks();
        ToggleTouchscreenForceOn();
    }

    void    InitMenu(PluginMenu &menu) {

        menu += new MenuEntry("Config values", nullptr, [](MenuEntry *entry)
        {

            MessageBox("Settings", Utils::Format("Loaded: %d\nPath: %s", config.loadBtks, config.btksPath.c_str()))();
        });

        menu += new MenuEntry("Test hook results", nullptr, [](MenuEntry *entry)
        {
            char* out = new char[0xFF];

            sprintf(out, "%d", testInt);

            MessageBox("Hook test shit", std::string(out))();
        });

        menu += new MenuEntry("BTKS loader result", nullptr, [](MenuEntry *entry)
        {
            char* out = new char[0x10];

            sprintf(out, "%d", btksResult);

            MessageBox("BTKS loader result", std::string(out))();
        });
    }

    int main(void) {
        PluginMenu *menu = new PluginMenu("Saltwater debug menu", 0, 1, 0, "Rhythm Heaven Megamix patch", 1);

        // Synnchronize the menu with frame event
        menu->SynchronizeWithFrame(true);

        // Init our menu entries & folders
        InitMenu(*menu);

        if (btksResult) {
            MessageBox("Error messages", Utils::Format("BTKS loader: %s", Megamix::ErrorMessage(btksResult).c_str()))();
        }

        // Launch menu and mainloop
        menu->Run();

        delete menu;

        // Exit plugin
        return (0);
    }
}
