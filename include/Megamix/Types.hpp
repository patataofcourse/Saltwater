#ifndef RHMTYPES_H
#define RHMTYPES_H

#include <array>

#include "types.h"

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
        u8 unk8;
        u16 unkA;
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

    struct MuseumRow {
        u32 columnCount;
        std::array<u16, 5> gameIndices;
        u8 pad[2];
        const char *titleId;
        u32 highIndex;
        u32 lowIndex;

        MuseumRow(std::array<u16, 5> gameIndices, const char *titleId, u32 highIndex, u32 lowIndex) {
            if      (gameIndices[1] == 0x101) this->columnCount = 1;
            else if (gameIndices[3] == 0x101) this->columnCount = 3;
            else if (gameIndices[4] == 0x101) this->columnCount = 4;
            else                              this->columnCount = 5;

            this->gameIndices = gameIndices;
            this->titleId = titleId;
            this->highIndex = highIndex;
            this->lowIndex = lowIndex;
        }
    };

    struct Color8 {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    };

    struct MuseumRowColor {
        Color8 unk1;
        Color8 background;
        Color8 edgeFade;
        Color8 unk3;

        MuseumRowColor(Color8 background, Color8 edgeFade) {
            this->unk1 = { 0x00, 0x00, 0x00, 0x00 };
            this->unk3 = { 0xFF, 0xFF, 0xFF, 0x6E };

            this->background = background;
            this->edgeFade = edgeFade;
        }
    };
}

#endif