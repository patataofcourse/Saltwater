#ifndef RHMREGION_H
#define RHMREGION_H

#include <vector>
#include <string>

#include "types.h"

#include "Megamix/Types.hpp"

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

    std::vector<u32> RetryRemixLocs();

    u32 RegionFSHookFunc();
    u32 RegionOtherHookFunc();

    typedef void* (*IsFileCachedSignature) (void* cachedFileManager, s32 fileId, Megamix::FileInfo* fileInfo);
    typedef bool (*CacheFileSignature) (void* cachedFileManager, s32 fileId, Megamix::FileInfo* fileInfo);

    typedef Result (*TryOpenFileSignature) (Megamix::FileInputStream::FileBase* fileBase, wchar_t* filePath, u32 mode);
    typedef Result (*CloseFileSignature) (void* ptr);
    typedef Result (*TryGetSizeSignature) (Megamix::FileInputStream::FileBase* fileBase, s64 position);

    u32 DoOpenFileFunc();
    u32 CacheFileManagerPos();
    u32 FileInputStreamVtable();
    IsFileCachedSignature IsFileCachedFunc();
    CacheFileSignature CacheFileFunc();
    TryOpenFileSignature TryOpenFileFunc();
    CloseFileSignature CloseFileFunc();
    TryGetSizeSignature TryGetSizeFunc();

}

#endif