#ifndef RHMREGION_H
#define RHMREGION_H

namespace Region {

    enum {
        US = 0x18a400,
        EU = 0x18a500,
        JP = 0x155a00,
        K0 = 0x18a600,
    };

    std::string RegionCode(u32 region);

    std::vector<u32> RHMPatchGameAddresses(u32 region);
    std::vector<u32> RHMPatchTempoAddresses(u32 region);
    std::vector<u32> RHMPatchGateAddresses(u32 region);

}

#endif