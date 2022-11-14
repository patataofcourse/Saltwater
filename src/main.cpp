#include <3ds.h>
#include "csvc.h"
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

#include "Stuff.hpp"

Config* config;
using Megamix::btks;

const char* version = VERSION;

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

        Process::exceptionCallback = Megamix::CrashHandler;

        // Init region and config
        region = CTRPluginFramework::Process::GetTitleID();
        config = Config::FromFile(MEGAMIX_CONFIG_PATH);
    
        Megamix::Hooks::TickflowHooks();
        if (region != Region::JP)
            Megamix::Hooks::TempoHooks();
    }

    // This function is called when the process exits
    // Useful to save settings, undo patchs or clean up things
    void    OnProcessExit(void) {
        Megamix::Hooks::DisableAllHooks();
        ToggleTouchscreenForceOn();
        delete config;
    }

    void    InitMenu(PluginMenu &menu) {

        menu += new MenuEntry("Config values", nullptr, [](MenuEntry *entry)
        {

            MessageBox("Settings", Utils::Format(
                "Result: %d",
                configResult
            ))();
        });

        menu += new MenuEntry("Tickflow contents", nullptr, [](MenuEntry *entry)
        {

            MessageBox("Map shit", Stuff::FileMapToString(config->tickflows))();
        });

        menu += new MenuEntry("Tempo contents (do this w a loaded btks)", nullptr, [](MenuEntry *entry)
        {

            MessageBox("Map shit", Stuff::TempoMapToString(btks.tempos))();
        });
        
    }

    int main(void) {

        // re-enable rhmpatch if needed
        if (*(bool*)CTRPluginFramework::FwkSettings::Get().Header->config) {
            File::Rename("/luma/titles/000400000018A400/_code.ips", "/luma/titles/000400000018A400/code.ips");
        }

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

        // Exit plugin
        return (0);
    }
}
