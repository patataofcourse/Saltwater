// for internal usage in Hooks.cpp - please don't call any of these functions directly

#include <3ds.h>

#include "Megamix.hpp"

namespace Megamix::Hooks {
    // Charting.cpp
    void* getTickflowOffset(int index);
    void* getGateTickflowOffset(int index);
    void* getGatePracticeTickflowOffset(int index);

    Megamix::TempoTable* getTempoStrm(Megamix::CSoundManager* this_, u32 id);
    Megamix::TempoTable* getTempoSeq(Megamix::CSoundManager* this_, u32 id);
    Megamix::TempoTable* getTempoAll(Megamix::CSoundManager* this_, u32 id);

    // FS.cpp
    void doReadFile(CFileManager* self, FileInfo* fileInfo);

    // Misc.cpp
    u32 getRegionCTR();
    u32 getRegionMegamix();
}