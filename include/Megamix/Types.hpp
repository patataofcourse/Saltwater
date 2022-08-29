#ifndef RHMTYPES_H
#define RHMTYPES_H

namespace Megamix {
    struct Tempo {
        float beats;
        u32 time;
        u16 flag8;
        u16 flagA;
    };

    struct TempoTable {
        u32 id1;
        u32 id2;
        Tempo* pos;
    };

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