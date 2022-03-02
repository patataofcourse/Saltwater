#ifndef RHMREGION_H
#define RHMREGION_H

extern u32 region;

namespace Region {

    enum {
        US = 0x18a400,
        EU = 0x18a500,
        JP = 0x155a00,
        KR = 0x18a600,
    };

    std::string RegionCode();

    std::vector<u32> RHMPatchGameAddresses();
    std::vector<u32> RHMPatchTempoAddresses();
    std::vector<u32> RHMPatchGateAddresses();

    u32 GameTable();
    u32 TickflowHookFunc();
}

#endif