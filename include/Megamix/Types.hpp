#ifndef RHMTYPES_H
#define RHMTYPES_H

namespace Megamix {
    typedef u32 TempoTable[4];

    struct SM_TempoTable {
        u32 id;
        TempoTable* tempo;
    };

    struct CSoundManager {
        u8 pad0[0x8C];
        TempoTable* tempoTable;
        SM_TempoTable* tableStrm;
        SM_TempoTable* tableSeq;
        u16 numberTempos;
    };
}

#endif