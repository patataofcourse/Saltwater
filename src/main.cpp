#include <3ds.h>
#include "csvc.h"
#include <CTRPluginFramework.hpp>

#include <vector>

#include "Megamix.hpp"

char* cheerReadersOriginal = new char[0x13];
int* C00Result;
u32 region;

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

        // Init region
        region = CTRPluginFramework::Process::GetTitleID();

        // Patches Cheer Readers to say "It's up to you" instead of "1, 2, 3"
        u32 address = Region::CheerReadersPatch(region);
        u32 patchSize = 0x13;
        char patch[patchSize] = {0x72, 0x76, 0x6c, 0x42, 0x6f, 0x6f, 0x6b, 0x4c, 0x5f, 0x6c, 0x79, 0x6c, 0x69, 0x63, 0x5f, 0x68, 0x65, 0x79, 0x00};
        Process::Patch(address, patch, patchSize, cheerReadersOriginal);
    }

    // This function is called when the process exits
    // Useful to save settings, undo patchs or clean up things
    void    OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
    }

    void    InitMenu(PluginMenu &menu)
    {
        // Create your entries here, or elsewhere
        // You can create your entries whenever/wherever you feel like it
        
        // Example entry
        menu += new MenuEntry("Cheer Readers original value", nullptr, [](MenuEntry *entry)
        {
            std::string body(cheerReadersOriginal);

            MessageBox("Cheer Readers original value", body)();
        });

        // Example entry
        menu += new MenuEntry("RHMPatch load status", nullptr, [](MenuEntry *entry)
        {
            char* out = new char[5];

            sprintf(out, "%d - %d", C00Result[0], C00Result[1]);

            MessageBox("C00.bin results: Open - Read", std::string(out))();
        });

        // Example entry
        menu += new MenuEntry("Title ID", nullptr, [](MenuEntry *entry)
        {
            char* out = new char[5];

            sprintf(out, "%x", region);

            MessageBox("Title ID / region", std::string(out))();
        });

        // Example entry
        menu += new MenuEntry("RHMPatch address shit", nullptr, [](MenuEntry *entry)
        {
            char* out = new char[5];

            sprintf(out, "%08x", C00Result[2]);

            MessageBox("C00.bin results: Open - Read", std::string(out))();
        });
    }

    int     main(void)
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
