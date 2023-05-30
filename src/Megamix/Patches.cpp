#include <array>

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Megamix/Patches.hpp"

#define rgba(value) { (value >> 24) & 0xff, (value >> 16) & 0xff, (value >> 8) & 0xff, value & 0xff }

namespace Megamix::Patches {
    const u32 museumRowCount = 32;

    const MuseumRow museumRows[museumRowCount] {
        /* E2 */ MuseumRow({ CtrStepL,       CtrSumouL,     RvlBadmintonL, None,            None       }, "room_05",       0, 0),
        /* E1 */ MuseumRow({ NtrCoinToss,    AgbVirus,      CtrChicken,    RvlSword,        None       }, "room_05",       0, 0),
        /* E0 */ MuseumRow({ NtrFrogL,       RvlKarate2,    NtrShootingS,  CtrWoodCatS,     RvlKarate4 }, "room_05",       0, 0),

        /* 0  */ MuseumRow({ RvlKarate0,     NtrRobotS,     RvlBadmintonS, CtrStepS,        None       }, "stage_gr00",    0, 0),
        /* 1  */ MuseumRow({ AgbHairS,       NtrChorusS,    RvlMuscleS,    CtrFruitbasketS, None       }, "stage_gr01",    0, 1),
        /* 2  */ MuseumRow({ NtrCoinToss,    None,          None,          None,            None       }, "stage_gate_00", 0, 2),
        /* 3  */ MuseumRow({ AgbClapS,       NtrShootingS,  RvlFleaS,      CtrInterpreterS, None       }, "stage_gr02",    0, 3),
        /* 4  */ MuseumRow({ AgbRabbitL,     NtrAirBoardL,  RvlBattingL,   CtrChameleonL,   None       }, "stage_gr03",    0, 4),
        /* 5  */ MuseumRow({ AgbVirus,       None,          None,          None,            None       }, "stage_gate_01", 0, 5),
        /* 6  */ MuseumRow({ AgbGhostS,      NtrPingPongS,  RvlGomaS,      CtrWoodCatS,     None       }, "stage_gr04",    0, 6),
        /* 7  */ MuseumRow({ AgbShujiL,      NtrBlueBirdsL, RvlBirdL,      CtrDotSamuraiL,  None       }, "stage_gr05",    0, 7),
        /* 8  */ MuseumRow({ CtrChicken,     None,          None,          None,            None       }, "stage_gate_02", 0, 8),
        /* 9  */ MuseumRow({ AgbBatterL,     NtrNinjaL,     RvlGolfL,      CtrSumouL,       RemixLED   }, "room_LED",      0, 8),
        /* 10 */ MuseumRow({ RvlKarate1,     NtrRobotL,     RvlBadmintonL, CtrStepL,        Remix00    }, "room_00",       0, 0),
        /* 11 */ MuseumRow({ AgbHairL,       NtrChorusL,    RvlMuscleL,    CtrFruitbasketL, Remix01    }, "room_01",       0, 1),
        /* 12 */ MuseumRow({ AgbClapL,       NtrShootingL,  RvlFleaL,      CtrInterpreterL, Remix02    }, "room_02",       0, 2),
        /* 13 */ MuseumRow({ AgbRatL,        NtrIdolL,      RvlManjuL,     CtrBlancoL,      Remix03    }, "room_03",       0, 3),
        /* 14 */ MuseumRow({ AgbGhostL,      NtrPingPongL,  RvlGomaL,      CtrWoodCatL,     Remix04    }, "room_04",       0, 4),
        /* 15 */ MuseumRow({ AgbTapL,        NtrFrogL,      RvlInterviewL, CtrTangoL,       Remix05    }, "room_05",       0, 5),
        /* 16 */ MuseumRow({ AgbTonoL,       NtrCameraManL, RvlRocketL,    CtrPillowL,      None       }, "stage_sk00",    1, 0),
        /* 17 */ MuseumRow({ AgbMarcherL,    NtrShugyoL,    RvlSeesawL,    CtrBearL,        None       }, "stage_sk01",    1, 1),
        /* 18 */ MuseumRow({ AgbSpaceDanceL, NtrBackbeatL,  RvlBookL,      CtrTeppanL,      None       }, "stage_sk02",    1, 2),
        /* 19 */ MuseumRow({ RvlSword,       None,          None,          None,            None       }, "stage_gate_03", 1, 3),
        /* 20 */ MuseumRow({ AgbClapA,       NtrIdolA,      RvlMuscleA,    CtrBlancoA,      Remix06    }, "room_06",       0, 6),
        /* 21 */ MuseumRow({ AgbTapA,        NtrFrogA,      RvlGolfA,      CtrDotSamuraiA,  Remix07    }, "room_07",       0, 7),
        /* 22 */ MuseumRow({ AgbSpaceDanceA, NtrPingPongA,  RvlManjuA,     RvlKarate4,      RemixTED   }, "room_TED",      0, 9),
        /* 23 */ MuseumRow({ AgbHoppingL,    AgbNightWalkL, AgbQuizL,      None,            None       }, "bonus_AGB",     0, 0),
        /* 24 */ MuseumRow({ NtrBoxShowL,    NtrShortLiveL, RvlKarate2,    None,            None       }, "bonus_NTR",     0, 0),
        /* 25 */ MuseumRow({ RvlAssembleL,   RvlDateL,      RvlFishingL,   None,            None       }, "bonus_RVL0",    0, 0),
        /* 26 */ MuseumRow({ RvlForkL,       RvlRapL,       RvlRecieveL,   None,            None       }, "bonus_RVL1",    0, 0),
        /* 27 */ MuseumRow({ RvlRobotL,      RvlRotationL,  RvlSamuraiL,   None,            None       }, "bonus_RVL2",    0, 0),
        /* 28 */ MuseumRow({ RvlSortL,       RvlWatchL,     RvlKarate3,    None,            None       }, "bonus_RVL3",    0, 0),
    };

    const MuseumRowColor museumRowColors[museumRowCount] {
        /* E2 */ MuseumRowColor(rgba(0x424242ff), rgba(0x00000000)),
        /* E1 */ MuseumRowColor(rgba(0x424242ff), rgba(0x00000000)),
        /* E0 */ MuseumRowColor(rgba(0x424242ff), rgba(0x00000000)),

        /* 0  */ MuseumRowColor(rgba(0xffed2aff), rgba(0x8c640000)),
        /* 1  */ MuseumRowColor(rgba(0x4af8f1ff), rgba(0x01494c00)),
        /* 2  */ MuseumRowColor(rgba(0xdcd9d9ff), rgba(0x85561600)),
        /* 3  */ MuseumRowColor(rgba(0xffa030ff), rgba(0x783a0000)),
        /* 4  */ MuseumRowColor(rgba(0x3667caff), rgba(0x04007800)),
        /* 5  */ MuseumRowColor(rgba(0xcecacaff), rgba(0x82393900)),
        /* 6  */ MuseumRowColor(rgba(0xeb4040ff), rgba(0x4f000000)),
        /* 7  */ MuseumRowColor(rgba(0x9a43b7ff), rgba(0x0f013f00)),
        /* 8  */ MuseumRowColor(rgba(0xafafafff), rgba(0x284d2f00)),
        /* 9  */ MuseumRowColor(rgba(0x2ba820ff), rgba(0x013f0d00)),
        /* 10 */ MuseumRowColor(rgba(0x2ba820ff), rgba(0x013f0d00)),
        /* 11 */ MuseumRowColor(rgba(0x2ba820ff), rgba(0x013f0d00)),
        /* 12 */ MuseumRowColor(rgba(0x2ba820ff), rgba(0x013f0d00)),
        /* 13 */ MuseumRowColor(rgba(0x2ba820ff), rgba(0x013f0d00)),
        /* 14 */ MuseumRowColor(rgba(0x2ba820ff), rgba(0x013f0d00)),
        /* 15 */ MuseumRowColor(rgba(0x2ba820ff), rgba(0x013f0d00)),
        /* 16 */ MuseumRowColor(rgba(0xffcaedff), rgba(0x7e3e9200)),
        /* 17 */ MuseumRowColor(rgba(0xb7ece2ff), rgba(0x2e818600)),
        /* 18 */ MuseumRowColor(rgba(0xece5b7ff), rgba(0xadaa0800)),
        /* 19 */ MuseumRowColor(rgba(0x8c8c8cff), rgba(0x503c5000)),
        /* 20 */ MuseumRowColor(rgba(0xf7f2b9ff), rgba(0xaa943c00)),
        /* 21 */ MuseumRowColor(rgba(0xf7f2b9ff), rgba(0xaa943c00)),
        /* 22 */ MuseumRowColor(rgba(0xf7f2b9ff), rgba(0xaa943c00)),
        /* 23 */ MuseumRowColor(rgba(0x78500aff), rgba(0x643c3200)),
        /* 24 */ MuseumRowColor(rgba(0x78500aff), rgba(0x643c3200)),
        /* 25 */ MuseumRowColor(rgba(0x78500aff), rgba(0x643c3200)),
        /* 26 */ MuseumRowColor(rgba(0x78500aff), rgba(0x643c3200)),
        /* 27 */ MuseumRowColor(rgba(0x78500aff), rgba(0x643c3200)),
        /* 28 */ MuseumRowColor(rgba(0x78500aff), rgba(0x643c3200)),
    };

    // see section F5.1.35 in the arm A-profile reference manual
    // register is 4 bits, value is 12 bits
    u32 MakeCmpImmediateInstruction(u32 reg, u32 value) {
        // 1110 == no condition
        const u32 cond = 0b1110 << 28;
        const u32 cmpImmBase = 0b0000'00110'10'1 << 20;
        reg <<= 16;

        return cond | cmpImmBase | reg | value;
    }

    void PatchMuseumExtraRows() {
        for (auto address : Region::MuseumRowsInfoAddresses()) {
            Process::Patch(address, (u32) museumRows);
        }

        u32 compareR1Instruction = // cmp r1, MUSEUM_ROW_COUNT
            MakeCmpImmediateInstruction(1, museumRowCount);

        for (auto address : Region::MuseumRowsR1Cmps()) {
            Process::Patch(address, compareR1Instruction);
        }

        u32 compareR8Instruction = // cmp r1, MUSEUM_ROW_COUNT
            MakeCmpImmediateInstruction(8, museumRowCount);

        for (auto address : Region::MuseumRowsR8Cmps()) {
            Process::Patch(address, compareR8Instruction);
        }

        // FIXME: for this to work we need to stop c++ from overwriting the
        // colors when it runs the constructor (@ 0x38DE58) for the array
        for (auto address : Region::MuseumRowsColorsAddresses()) {
            Process::Patch(address, (u32) museumRowColors);
        }
    }
}
