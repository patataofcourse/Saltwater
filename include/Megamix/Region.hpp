#ifndef RHMREGION_H
#define RHMREGION_H

#include <vector>
#include <string>

#include "types.h"

extern u8 region;

namespace Region {

    enum {
        JP = 0,
        US = 1,
        EU = 2,
        KR = 3,
        KR_CTR = 5, // in the SDK, KR is 5 instead of 3
        UNK
    };

    u8 FromCode(u32 code);
    std::string Name();

    std::vector<u32> MuseumRowsInfoAddresses();
    std::vector<u32> MuseumRowsColorsAddresses();
    u32              MuseumRowsColorsInitFunc();
    // TODO: maybe make this into MuseumRowsCmps that returns a
    // map of int -> vector<u8> other regions might use different registers
    std::vector<u32> MuseumRowsR1Cmps();
    std::vector<u32> MuseumRowsR8Cmps();

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

    u32 TickflowCommandsSwitch();
    u32 TickflowCommandsEnd();
    u32 TickflowAsyncSubLocation();

    u32 GlobalSaveDataPointer();
    u32 GlobalInputManagerPointer();
    u32 GlobalFileManagerPointer();

    std::vector<u32> RetryRemixLocs();

    u32 RegionFSHookFunc();
    u32 RegionOtherHookFunc();

    typedef u32 (*GetU32VariableSignature) (u32 pos);
    typedef void (*SetU32VariableSignature) (u32 pos, u32 value);
    typedef u8 (*GetU8VariableSignature) (u32 pos);
    typedef void (*SetU8VariableSignature) (u32 pos, u8 value);

    GetU32VariableSignature GetU32VariableFunc();
    SetU32VariableSignature SetU32VariableFunc();
    GetU8VariableSignature GetU8VariableFunc();
    SetU8VariableSignature SetU8VariableFunc();
}

#endif