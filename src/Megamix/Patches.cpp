#include <array>
#include <vector>
#include <algorithm>
#include <optional>

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

namespace Megamix::Patches {
    std::vector<MuseumRow> museumRows {
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

    std::vector<MuseumRowColor> museumRowColors {
        /* 0  */ MuseumRowColor(0xFFED2AFF, 0x8C640000),
        /* 1  */ MuseumRowColor(0x4AF8F1FF, 0x01494C00),
        /* 2  */ MuseumRowColor(0xDCD9D9FF, 0x85561600),
        /* 3  */ MuseumRowColor(0xFFA030FF, 0x783A0000),
        /* 4  */ MuseumRowColor(0x3667CAFF, 0x04007800),
        /* 5  */ MuseumRowColor(0xCECACAFF, 0x82393900),
        /* 6  */ MuseumRowColor(0xEB4040FF, 0x4F000000),
        /* 7  */ MuseumRowColor(0x9A43B7FF, 0x0F013F00),
        /* 8  */ MuseumRowColor(0xAFAFAFFF, 0x284D2F00),
        /* 9  */ MuseumRowColor(0x2BA820FF, 0x013F0D00),
        /* 10 */ MuseumRowColor(0x2BA820FF, 0x013F0D00),
        /* 11 */ MuseumRowColor(0x2BA820FF, 0x013F0D00),
        /* 12 */ MuseumRowColor(0x2BA820FF, 0x013F0D00),
        /* 13 */ MuseumRowColor(0x2BA820FF, 0x013F0D00),
        /* 14 */ MuseumRowColor(0x2BA820FF, 0x013F0D00),
        /* 15 */ MuseumRowColor(0x2BA820FF, 0x013F0D00),
        /* 16 */ MuseumRowColor(0xFFCAEDFF, 0x7E3E9200),
        /* 17 */ MuseumRowColor(0xB7ECE2FF, 0x2E818600),
        /* 18 */ MuseumRowColor(0xECE5B7FF, 0xADAA0800),
        /* 19 */ MuseumRowColor(0x8C8C8CFF, 0x503C5000),
        /* 20 */ MuseumRowColor(0xF7F2B9FF, 0xAA943C00),
        /* 21 */ MuseumRowColor(0xF7F2B9FF, 0xAA943C00),
        /* 22 */ MuseumRowColor(0xF7F2B9FF, 0xAA943C00),
        /* 23 */ MuseumRowColor(0x78500AFF, 0x643C3200),
        /* 24 */ MuseumRowColor(0x78500AFF, 0x643C3200),
        /* 25 */ MuseumRowColor(0x78500AFF, 0x643C3200),
        /* 26 */ MuseumRowColor(0x78500AFF, 0x643C3200),
        /* 27 */ MuseumRowColor(0x78500AFF, 0x643C3200),
        /* 28 */ MuseumRowColor(0x78500AFF, 0x643C3200),
    };

    // see section F5.1.35 in the arm A-profile reference manual
    // register is 4 bits, value is 12 bits
    u32 make_cmp_immediate_instruction(u32 reg, u32 value) {
        // 1110 == no condition
        const u32 cond = 0b1110 << 28;
        const u32 cmp_imm_base = 0b0000'00110'10'1 << 20;
        reg <<= 16;

        return cond | cmp_imm_base | reg | value;
    }

    std::optional<u16> SlotIdToMuseumGameId(u16 gameId) {
        if (gameId < 0x100) {
            return gameId;
        }

        switch (gameId) {
            case 0x103: return AgbVirus;
            case 0x107: return NtrCoinToss;
            case 0x10b: return RvlSword;
            case 0x10f: return CtrChicken;
        }

        return std::nullopt;
    }

    void AddExtraRowsToFront() {
        std::vector<MuseumRow>      extraMuseumRows {};
        std::vector<MuseumRowColor> extraMuseumRowColors {};

        auto PushExtraRow = [&](std::array<u16, 5> newRowIds) {
            extraMuseumRows.emplace_back(newRowIds, "navi_char_name_Question", 0, 0);
            extraMuseumRowColors.emplace_back(0x424242ff, 0x00000000);
        };

        // not all slots are valid museum games
        u32 validGameIds = 0;
        for (auto &[key, _] : config->tickflows) {
            if (SlotIdToMuseumGameId(key).has_value()) {
                validGameIds += 1;
            }
        }

        // museum don't support rows with only 2 games
        if (validGameIds == 2) {
            for (auto &pair : config->tickflows) {
                std::optional<u16> id = SlotIdToMuseumGameId(pair.first);
                if (!id.has_value()) {
                    continue;
                }

                PushExtraRow({ id.value(), None, None, None, None });
            }
        } else {
            std::array<u16, 5> newRowIds { None, None, None, None, None };
            size_t newRowLength = 0;

            for (auto &pair : config->tickflows) {
                std::optional<u16> id = SlotIdToMuseumGameId(pair.first);
                if (!id.has_value()) {
                    continue;
                }

                newRowIds[newRowLength] = id.value();
                newRowLength += 1;

                if (newRowLength == 5) {
                    PushExtraRow(newRowIds);

                    newRowIds = { None, None, None, None, None };
                    newRowLength = 0;
                }
            }

            // museum don't support rows with only 2 games
            if (newRowLength == 2) {
                auto &last_extra_row = extraMuseumRows.back();

                newRowIds[2] = last_extra_row.gameIndices[4];

                last_extra_row.columnCount -= 1;
                last_extra_row.gameIndices[4] = None;
            }

            if (newRowLength != 0) {
                PushExtraRow(newRowIds);
            }
        }

        std::reverse(extraMuseumRows.begin(), extraMuseumRows.end());

        // concat extra rows with original rows
        std::vector<MuseumRow> originalMuseumRows = std::move(museumRows);
        museumRows.reserve(originalMuseumRows.size() + extraMuseumRows.size());
        museumRows.insert(museumRows.end(), extraMuseumRows.begin(), extraMuseumRows.end());
        museumRows.insert(museumRows.end(), originalMuseumRows.begin(), originalMuseumRows.end());

        // concat extra row colors with original row colors
        std::vector<MuseumRowColor> originalMuseumRowColors = std::move(museumRowColors);
        museumRowColors.reserve(originalMuseumRowColors.size() + extraMuseumRowColors.size());
        museumRowColors.insert(
            museumRowColors.end(),
            extraMuseumRowColors.begin(),
            extraMuseumRowColors.end()
        );
        museumRowColors.insert(
            museumRowColors.end(),
            originalMuseumRowColors.begin(),
            originalMuseumRowColors.end()
        );
    }

    void PatchMuseumExtraRows() {
        AddExtraRowsToFront();

        // patch in new museum rows
        for (auto address : Region::MuseumRowsInfoAddresses()) {
            Process::Patch(address, (u32) museumRows.data());
        }

        u32 compare_r1_instruction = // cmp r1, MUSEUM_ROW_COUNT
            make_cmp_immediate_instruction(1, museumRows.size());

        for (auto address : Region::MuseumRowsR1Cmps()) {
            Process::Patch(address, compare_r1_instruction);
        }

        u32 compare_r8_instruction = // cmp r8, MUSEUM_ROW_COUNT
            make_cmp_immediate_instruction(8, museumRows.size());

        for (auto address : Region::MuseumRowsR8Cmps()) {
            Process::Patch(address, compare_r8_instruction);
        }

        Hooks::StubFunction<void>(Region::MuseumRowsColorsInitFunc());

        for (auto address : Region::MuseumRowsColorsAddresses()) {
            Process::Patch(address, (u32) museumRowColors.data());
        }
    }
}
