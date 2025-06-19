#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"

u8 region;

namespace Region {
    u8 FromCode(u32 code) {
        switch (code) {
            case 0x155a00:
                return JP;
            case 0x18a400:
                return US;
            case 0x18a500:
                return EU;
            case 0x18a600:
                return KR;
            default:
                return UNK;
        }
    }

    std::string Name() {
        switch (region) {
            case US: return "US";
            case EU: return "EU";
            case JP: return "JP";
            case KR: return "KR";
            default: return "UNK";
        }
    }

    // Extra museum rows patch

    std::vector<u32> MuseumRowsInfoAddresses() {
        switch (region) {
            case US:
            case EU:
                return {
                    0x2421e8, 0x24c480, 0x24d408,                     // gRowInfo
                    0x1979e0, 0x1983fc, 0x242350, 0x2424bc, 0x24e010, // gRowInfo1
                    0x2423d4, 0x2619c0,                               // gRowInfo2
                    0x224fe8, 0x225008, 0x225024, 0x225044, 0x225068, // gRowInfo3
                };
            case KR:
                return {
                    0x2421c0, 0x24c458, 0x24d3e0,                     // gRowInfo
                    0x1979e0, 0x1983fc, 0x242328, 0x242494, 0x24dfe8, // gRowInfo1
                    0x2423ac, 0x261998,                               // gRowInfo2
                    0x224fe8, 0x225008, 0x225024, 0x225044, 0x225068, // gRowInfo3
                };

            // TODO:
            case JP:

            default: return {};
        }
    }

    u32 MuseumRowsColorsInitFunc() {
        switch (region) {
            case US:
            case EU:
            case KR:
                return 0x38de58;

            // TODO:
            case JP:

            default: return {};
        }
    }

    std::vector<u32> MuseumRowsColorsAddresses() {
        switch (region) {
            case US:
            case EU:
                return { 0x17d8b4, 0x17e318, 0x17e4c4, 0x241fc4, 0x38e6d8 };
            case KR:
                return { 0x17d8b4, 0x17e318, 0x17e4c4, 0x241f9c, 0x38e6d8 };

            // TODO:
            case JP:

            default: return {};
        }
    }

    std::vector<u32> MuseumRowsR1Cmps() {
        switch (region) {
            case US:
            case EU:
                return { 0x2423c4, 0x2423DC, 0x2619B0 };

            case KR:
                return { 0x24239c, 0x2423b4, 0x261988 };

            // TODO:
            case JP:

            default: return {};
        }
    }

    std::vector<u32> MuseumRowsR8Cmps() {
        switch (region) {
            case US:
            case EU:
                return { 0x242400, 0x2424a0 };
            case KR:
                return { 0x2423d8, 0x242478 };

            //TODO:
            case JP:

            default: return {};
        }
    }

    // Tables and stuff

    u32 GameTable() {
        switch (region) {
            case US:
            case EU:
            case KR:
                return 0x52b498;
            case JP:
                return 0x522498;
            default:
                return 0;
        }
    }

    u32 TempoTable() {
        switch (region) {
            case US:
                return 0x53EF54;
            case EU:
            case KR:
                return 0x53F04C;
            case JP:
                return 0x5324B0;
            default:
                return 0;
        }
    }

    u32 GateTable() {
        switch (region) {
            case US:
            case EU:
            case KR:
                return 0x52E488;
            case JP:
                return 0x525488;
            default:
                return 0;
        }
    }

    // Hooks - Tickflow

    u32 TickflowHookFunc() {
        switch (region) {
            case JP:
                return 0x25a1b4;
            case US:
            case EU:
                return 0x258df4;
            case KR:
                return 0x258dcc;
            default:
                return 0;
        }
    }

    u32 GateHookFunc() {
        switch (region) {
            case JP:
                return 0x242510;
            case US:
            case EU:
                return 0x240f9c;
            case KR:
                return 0x240f74;
            default:
                return 0;
        }
    }

    u32 GatePracHookFunc() {
        switch (region) {
            case JP:
                return 0x32e01c;
            case US:
            case EU:
            case KR:
                return 0x32d630;
            default:
                return 0;
        }
    }

    // Hooks - Tempo

    u32 StrmTempoHookFunc() {
        switch (region) {
            // can't seem to find a JP equivalent?
            case US:
            case EU:
                return 0x276424;
            case KR:
                return 0x2763fc;
            default:
                return 0;
        }
    }

    u32 SeqTempoHookFunc() {
        switch (region) {
            // can't seem to find a JP equivalent?
            case US:
            case EU:
                return 0x2763c8;
            case KR:
                return 0x2763a0;
            default:
                return 0;
        }
    }

    u32 AllTempoHookFunc() {
        switch (region) {
            case JP:
                // TODO: this function combines getTempoFromTable and US' func_0024f47c, adapt the hook
                return 0x25098c;
            case US:
            case EU:
            case KR:
                return 0x203c08;
            default:
                return 0;
        }
    }

    // Code sections

    u32 TextEnd() {
        return 0x39a000;
    }

    u32 RodataEnd() {
        switch (region) {
            case JP:
                return 0x518000;
            case US:
            case EU:
            case KR:
                return 0x521000;
            default:
                return 0;
        }
    }

    u32 DataEnd() {
        switch (region) {
            case JP:
                return 0x540754;
            case US:
                return 0x54f074;
            case EU:
                return 0x54f16c;
            case KR:
            default:
                return 0;
        }
    }

    u32 BssEnd() {
        switch (region) {
            case JP:
                return 0x5ce1f0;
            case US:
                return 0x5dc2f0;
            case EU:
            case KR:
                return 0x5dc2f0;
            default:
                return 0;
        }
    }

    // Various locations used for the Tickflow Command flow

    u32 TickflowCommandsSwitch() {
        switch (region) {
            case JP:
                return 0x25e054;
            case US:
            case EU:
                return 0x25c3c0;
            case KR:
                return 0x25c398;
            default:
                return 0;
        }
    }

    u32 TickflowCommandsEnd() {
        switch (region) {
            case JP:
                return 0x262eac;
            case US:
            case EU:
                return 0x2613cc;
            case KR:
                return 0x2613a4;
            default:
                return 0;
        }
    }

    // Location of the code for async_sub (fixes custom commands)

    u32 TickflowAsyncSubLocation() {
        switch (region) {
            case JP:
                return 0x25e338;
            case US:
            case EU:
                return 0x25c6c0;
            case KR:
                return 0x25c698;
            default:
                return 0;
        }
    }

    // Locations of global variables

    u32 GlobalSaveDataPointer(){
        switch (region) {
            case US:
                return 0x54d350;
            case EU:
            case KR:
                return 0x54d448;
            case JP:
            default:
                return 0;
        }
    }

    u32 GlobalInputManagerPointer(){
        switch (region) {
            case US:
                return 0x54eed0;
            case EU:
            case KR:
                return 0x54efc8;
            case JP:
            default:
                return 0;
        }
    }

    u32 GlobalFileManagerPointer(){
        switch (region) {
            case US:
                return 0x54eedc;
            case EU:
            case KR:
                return 0x54efd4;
            case JP:
            default:
                return 0;
        }
    }

    // RHMPatch's retry remix sub patch locations
    std::vector<u32> RetryRemixLocs() {
        switch (region) {
            case JP:
                return {0x19a180, 0x16485c, 0x1f8e78};
            case US:
            case EU:
            case KR:
                return {0x198c9c, 0x16302c, 0x1f7f84};
            default:
                return {};
        }
    }

    // Region checker
    u32 RegionFSHookFunc() {
        switch (region) {
            case US:
            case EU:
                return 0x28c070;
            case KR:
                return 0x28c048;
            default: // this function doesn't exist in JP afaik
                return 0;

        }
    }

    u32 RegionOtherHookFunc() {
        switch (region) {
            case JP:
                return 0x11932c;
            case US:
            case EU:
            case KR:
                return 0x119560;
            default:
                return 0;

        }
    }

    // 0xB2 / 0xB3 / 0xB4 variable functions

    GetU32VariableSignature GetU32VariableFunc() {
        switch (region) {
            case US:
                return (GetU32VariableSignature)0x203a60;
            case EU:
            case KR:
            case JP:
            default:
                return 0;
        }
    }


    SetU32VariableSignature SetU32VariableFunc() {
        switch (region) {
            case US:
                return (SetU32VariableSignature)0x3701c0;
            case JP:
            case EU:
            case KR:
            default:
                return 0;
        }
    }

    GetU8VariableSignature GetU8VariableFunc() {
        switch (region) {
            case US:
                return (GetU8VariableSignature)0x221e38;
            case JP:
            case EU:
            case KR:
            default:
                return 0;
        }
    }

    SetU8VariableSignature SetU8VariableFunc() {
        switch (region) {
            case US:
                return (SetU8VariableSignature)0x2346a0;
            case JP:
            case EU:
            case KR:
            default:
                return 0;
        }
    }
}