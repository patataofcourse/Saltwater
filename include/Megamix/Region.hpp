#ifndef RHMREGION_H
#define RHMREGION_H

#include <vector>
#include <string>

#include "types.h"
#include "Megamix/Types.hpp"

#include "Megamix.hpp"

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

    Megamix::CSaveData** GlobalSaveDataPointer();
    Megamix::CInputManager** GlobalInputManagerPointer();
    Megamix::CFileManager** GlobalFileManagerPointer();

    std::vector<u32> RetryRemixLocs();

    u32 RegionFSHookFunc();
    u32 RegionOtherHookFunc();

    Megamix::CSaveManager** SaveManager();
    Megamix::CBlackBarManager** BlackbarLayout();

    typedef int (*SWPrintfSignature) (char16_t* buffer, size_t size, const char16_t* format, ...);
    typedef u32 (*SetTextBoxStringSignature) (Megamix::TextBox *, const char16_t *, u32);

    SWPrintfSignature SWPrintfFunc();
    SetTextBoxStringSignature SetTextBoxStringFunc();

    Megamix::UnkStruct0054ef10** D_0054ef10();

    typedef bool (*IsGateGameValidSignature) (Megamix::GateGameIndex index);
    typedef u16 (*GetGateScoreSignature) (Megamix::CSaveData* self, Megamix::GateGameIndex index, s32 file);
    typedef void (*SetGateScoreSignature) (Megamix::CSaveData* self, Megamix::GateGameIndex index, u16 score, s32 file);
    typedef void (*SaveGameSignature) (Megamix::CSaveManager* self);

    IsGateGameValidSignature IsGateGameValidFunc();
    GetGateScoreSignature GetGateScoreFunc();
    SetGateScoreSignature SetGateScoreFunc();
    SaveGameSignature SaveGameFunc();

}

#endif