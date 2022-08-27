#include <3ds.h>
#include <CTRPluginFramework.hpp>

u32 region;

namespace Region {

    enum {
        JP = 0x155a00,
        US = 0x18a400,
        EU = 0x18a500,
        KR = 0x18a600,
    };

    std::string RegionCode() {
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

    // Hooks - Tempo

    u32 StrmTempoHookFunc() {
        switch (region) {
            case US:
                return 0x276424;
            default:
                return 0;
        }
    }

    u32 SeqTempoHookFunc() {
        switch (region) {
            case US:
                return 0x2763c8;
            default:
                return 0;
        }
    }

    u32 AllTempoHookFunc() {
        switch (region) {
            case US:
                return 0x203c08;
            default:
                return 0;
        }
    }
}