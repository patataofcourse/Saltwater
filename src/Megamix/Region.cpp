#include <3ds.h>
#include <CTRPluginFramework.hpp>

namespace Region {

    enum {
        JP = 0x155a00,
        US = 0x18a400,
        EU = 0x18a500,
        KR = 0x18a600,
    };

    std::string RegionCode(u32 region) {
        if (region == US) {
            return "US";
        } else if (region == EU) {
            return "EU";
        } else if (region == JP) {
            return "JP";
        } else if (region == KR) {
            return "KR";
        } else {
            return "UNK";
        }
    }

    std::vector<u32> RHMPatchGameAddresses(u32 region) {
        if (region == US || region == EU) {
            return {
                0x109008, 0x22D57C, 0x22D67C, 0x22D698, 0x22D6B4, 0x22D6D0, 0x240458, 0x24CB28, 0x2553CC, 0x255578, 0x258618,
                0x258E0C, 0x32D434, 0x32D450, 0x32D470, 0x32D4C8, 0x32D548, 0x32D5B0, 0x32D5E8
            };
        } else if (region == KR) {
            return {
                0x109008, 0x22d554, 0x22d654, 0x22d670, 0x22d68c, 0x22d6a8, 0x240430, 0x24cb00, 0x2553a4, 0x255550, 0x2585f0,
                0x258de4, 0x32d434, 0x32d450, 0x32d470, 0x32d4c8, 0x32d548, 0x32d5b0, 0x32d5e8
            };
        } else {
            return {};
        }
    }

    std::vector<u32> RHMPatchGateAddresses(u32 region) {
        if (region == US || region == EU) {
            return { 0x22AE40, 0x240FB0, 0x2552D8, 0x32D5FC, 0x32D614, 0x32D62C, 0x32D644, 0x32D65C, 0x32D6B8, 0x32D770 };
        } else if (region == KR) {
            return { 0x22ae18, 0x240f88, 0x2552b0, 0x32d5fc, 0x32d614, 0x32d62c, 0x32d644, 0x32d65c, 0x32d6b8, 0x32d770 };
        } else {
            return {};
        }
    }
}