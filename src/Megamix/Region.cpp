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

    // RHMPatch

    std::vector<u32> RHMPatchGameAddresses() {
        switch (region) {
            case US:
            case EU:
                return {
                    0x109008, 0x22D57C, 0x22D67C, 0x22D698, 0x22D6B4, 0x22D6D0, 0x240458, 0x24CB28, 0x2553CC, 0x255578, 0x258618,
                    0x258E0C, 0x32D434, 0x32D450, 0x32D470, 0x32D4C8, 0x32D548, 0x32D5B0, 0x32D5E8
                };
            case KR:
                return {
                    0x109008, 0x22d554, 0x22d654, 0x22d670, 0x22d68c, 0x22d6a8, 0x240430, 0x24cb00, 0x2553a4, 0x255550, 0x2585f0,
                    0x258de4, 0x32d434, 0x32d450, 0x32d470, 0x32d4c8, 0x32d548, 0x32d5b0, 0x32d5e8
                };
            case JP:
                return {
                    0x108864, 0x22e934, 0x22ea34, 0x22ea50, 0x22ea6c, 0x22ea88, 0x241a0c, 0x24e068, 0x2565c0, 0x256a00, 0x2599d8,
                    0x25a1cc, 0x32de20, 0x32de3c, 0x32de5c, 0x32deb4, 0x32df34, 0x32df9c, 0x32dfd4
                };
            default: return {};
        }
    }

    std::vector<u32> RHMPatchTempoAddresses() {
        switch (region) {
            case US:
            case EU:
            case KR:
                return { 0x101C10, 0x12B3B0 };
            case JP:
                return { 0x101c08, 0x120878 };
            default: return {};
        }
    }

    std::vector<u32> RHMPatchGateAddresses() {
        switch (region) {
            case US:
            case EU:
                return { 0x22AE40, 0x240FB0, 0x2552D8, 0x32D5FC, 0x32D614, 0x32D62C, 0x32D644, 0x32D65C, 0x32D6B8, 0x32D770 };
            case KR:
                return { 0x22ae18, 0x240f88, 0x2552b0, 0x32d5fc, 0x32d614, 0x32d62c, 0x32d644, 0x32d65c, 0x32d6b8, 0x32d770 };
            case JP:
                return { 0x22c1fc, 0x242524, 0x2564cc, 0x32dfe8, 0x32e000, 0x32e018, 0x32e030, 0x32e048, 0x32e0a4, 0x32e15c };
            default: return { };
        }
    }

    // Extra museum rows patch

    std::vector<u32> MuseumRowsInfoAddresses() {
        switch (region) {
            case US:
                return {
                    0x2421E8, 0x2421E8, 0x24C480, 0x24D408,           // gRowInfo
                    0x1979E0, 0x1983FC, 0x242350, 0x2424BC, 0x24E010, // gRowInfo1
                    0x2423D4, 0x2619C0,                               // gRowInfo2
                    0x224FE8, 0x225008, 0x225024, 0x225044, 0x225068, // gRowInfo3
                };

            // TODO:
            case EU:
            case KR:
            case JP:

            default: return {};
        }
    }

    u32 MuseumRowsColorsInitFunc() {
        switch (region) {
            case US:
                return 0x38de58;

            // TODO:
            case EU:
            case KR:
            case JP:

            default: return {};
        }
    }

    std::vector<u32> MuseumRowsColorsAddresses() {
        switch (region) {
            case US:
                return { 0x17D8B4, 0x17E318, 0x17E4C4, 0x241FC4, 0x38E6D8 };

            // TODO:
            case EU:
            case KR:
            case JP:

            default: return {};
        }
    }

    std::vector<u32> MuseumRowsR1Cmps() {
        switch (region) {
            case US:
                return { 0x2423C4, 0x2423DC, 0x2619B0 };

            // TODO:
            case EU:
            case KR:
            case JP:

            default: return {};
        }
    }

    std::vector<u32> MuseumRowsR8Cmps() {
        switch (region) {
            case US:
                return { 0x242400, 0x2424A0 };

            // TODO:
            case EU:
            case KR:
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
            case KR:
                return 0x28c070;
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
}