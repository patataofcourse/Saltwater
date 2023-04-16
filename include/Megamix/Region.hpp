#ifndef RHMREGION_H
#define RHMREGION_H

extern u8 region;

namespace Region {

    enum {
        JP = 0,
        US = 1,
        EU = 2,
        KR = 3,
        UNK,
    };

    u8 FromCode(u32 code);
    std::string Name();

    std::vector<u32> RHMPatchGameAddresses();
    std::vector<u32> RHMPatchTempoAddresses();
    std::vector<u32> RHMPatchGateAddresses();

    u32 GameTable();
    u32 TempoTable();
    u32 GateTable();
    
    u32 TickflowHookFunc();
    u32 GateHookFunc();
    u32 GatePracHookFunc();

    u32 StrmTempoHookFunc();
    u32 SeqTempoHookFunc();
    u32 AllTempoHookFunc();

    u32 TextEnd();
    u32 RodataEnd();
    u32 DataEnd();
    u32 BssEnd();

    std::vector<u32> RetryRemixLocs();


    u32 RegionFSHookFunc();
}

#endif