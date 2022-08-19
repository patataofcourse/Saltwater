#ifndef RHMREGION_H
#define RHMREGION_H

extern u32 region;

namespace Region {

    enum {
        JP = 0x155a00,
        US = 0x18a400,
        EU = 0x18a500,
        KR = 0x18a600,
    };

    std::string RegionCode();

    std::vector<u32> RHMPatchGameAddresses();
    std::vector<u32> RHMPatchTempoAddresses();
    std::vector<u32> RHMPatchGateAddresses();

    u32 GameTable();
    u32 TempoTable();
    u32 GateTable();
    
    u32 TickflowHookFunc();
    u32 GateHookFunc();

    u32 StrmTempoHookFunc();
    u32 SeqTempoHookFunc();
}

#endif