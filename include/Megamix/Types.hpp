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

    enum MuseumGameIndex : u16 {
        // Used in unused columns
        None = 0x101,

        // Tengoku - prequels
        AgbClapS = 0,
        AgbGhostS = 1,
        AgbHairS = 2,

        // DS - prequels
        NtrChorusS = 3,
        NtrPingPongS = 4,
        NtrRobotS = 5,
        NtrShootingS = 6,

        // Fever - prequels
        RvlBadmintonS = 7,
        RvlFleaS = 8,
        RvlGomaS = 9,
        RvlMuscleS = 0xa,

        // Megamix - prequels
        CtrFruitbasketS = 0xb,
        CtrInterpreterS = 0xc,
        CtrStepS = 0xd,
        CtrWoodCatS = 0xe,

        // Tengoku - standard games
        AgbBatterL = 0xf,
        AgbClapL = 0x10,
        AgbGhostL = 0x11,
        AgbHairL = 0x12,
        AgbHoppingL = 0x13,
        AgbMarcherL = 0x14,
        AgbNightWalkL = 0x15,
        AgbQuizL = 0x16,
        AgbRabbitL = 0x17,
        AgbRatL = 0x18,
        AgbShujiL = 0x19,
        AgbSpaceDanceL = 0x1a,
        AgbTapL = 0x1b,
        AgbTonoL = 0x1c,

        // DS - standard games
        NtrAirBoardL = 0x1d,
        NtrBackbeatL = 0x1e,
        NtrBlueBirdsL = 0x1f,
        NtrBoxShowL = 0x20,
        NtrCameraManL = 0x21,
        NtrChorusL = 0x22,
        NtrFrogL = 0x23,
        NtrIdolL = 0x24,
        NtrNinjaL = 0x25,
        NtrPingPongL = 0x26,
        NtrRobotL = 0x27,
        NtrShootingL = 0x28,
        NtrShortLiveL = 0x29,
        NtrShugyoL = 0x2a,

        // Fever - standard games
        RvlAssembleL = 0x2b,
        RvlBadmintonL = 0x2c,
        RvlBattingL = 0x2d,
        RvlBirdL = 0x2e,
        RvlBookL = 0x2f,
        RvlDateL = 0x30,
        RvlFishingL = 0x31,
        RvlFleaL = 0x32,
        RvlForkL = 0x33,
        RvlGolfL = 0x34,
        RvlGomaL = 0x35,
        RvlInterviewL = 0x36,
        RvlManjuL = 0x37,
        RvlMuscleL = 0x38,
        RvlRapL = 0x39,
        RvlRecieveL = 0x3a,
        RvlRobotL = 0x3b,
        RvlRocketL = 0x3c,
        RvlRotationL = 0x3d,
        RvlSamuraiL = 0x3e,
        RvlSeesawL = 0x3f,
        RvlSortL = 0x40,
        RvlWatchL = 0x41,

        // Megamix - standard games
        CtrBearL = 0x42,
        CtrBlancoL = 0x43,
        CtrChameleonL = 0x44,
        CtrDotSamuraiL = 0x45,
        CtrFruitbasketL = 0x46,
        CtrInterpreterL = 0x47,
        CtrPillowL = 0x48,
        CtrStepL = 0x49,
        CtrSumouL = 0x4a,
        CtrTangoL = 0x4b,
        CtrTeppanL = 0x4c,
        CtrWoodCatL = 0x4d,

        // Tengoku - sequels
        AgbClapA = 0x4e,
        AgbSpaceDanceA = 0x4f,
        AgbTapA = 0x50,

        // DS - sequels
        NtrFrogA = 0x51,
        NtrIdolA = 0x52,
        NtrPingPongA = 0x53,

        // Fever - sequels
        RvlGolfA = 0x54,
        RvlManjuA = 0x55,
        RvlMuscleA = 0x56,

        // Megamix - sequels
        CtrBlancoA = 0x57,
        CtrDotSamuraiA = 0x58,

        // Karate Man
        RvlKarate0 = 0x59,
        RvlKarate1 = 0x5a,
        RvlKarate2 = 0x5b,
        RvlKarate3 = 0x5c,
        RvlKarate4 = 0x5d,

        // Remixes
        RemixLED = 0x5e,
        RemixTED = 0x5f,
        Remix00 = 0x60,
        Remix01 = 0x61,
        Remix02 = 0x62,
        Remix03 = 0x63,
        Remix04 = 0x64,
        Remix05 = 0x65,
        Remix06 = 0x66,
        Remix07 = 0x67,

        // Endless games
        AgbVirus = 0x68,
        NtrCoinToss = 0x69,
        RvlSword = 0x6a,
        CtrChicken = 0x6b,
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

        Color8() {
            this->r = 0;
            this->g = 0;
            this->b = 0;
            this->a = 0;
        }

        Color8(u8 r, u8 g, u8 b, u8 a) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        Color8(u32 color) {
            this->r = color >> 24;
            this->g = color >> 16;
            this->b = color >>  8;
            this->a = color;
        }
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