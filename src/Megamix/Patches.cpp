#include <array>

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Megamix/Patches.hpp"

#define rgba(value) { (value >> 24) & 0xFF, (value >> 16) & 0xFF, (value >> 8) & 0xFF, value & 0xFF }

namespace Megamix::Patches {
    const u32 MuseumRowCount = 32;

    const MuseumRow MuseumRows[MuseumRowCount] {
        /* E2 */ MuseumRow({ 0x049, 0x04a, 0x02c, 0x101, 0x101 }, "room_05", 0, 0),
        /* E1 */ MuseumRow({ 0x069, 0x068, 0x06b, 0x06a, 0x101 }, "room_05", 0, 0),
        /* E0 */ MuseumRow({ 0x023, 0x05b, 0x006, 0x00e, 0x05d }, "room_05", 0, 0),

        /* 0  */ MuseumRow({ 0x059, 0x005, 0x007, 0x00d, 0x101 }, "stage_gr00", 0, 0),
        /* 1  */ MuseumRow({ 0x002, 0x003, 0x00a, 0x00b, 0x101 }, "stage_gr01", 0, 1),
        /* 2  */ MuseumRow({ 0x069, 0x101, 0x101, 0x101, 0x101 }, "stage_gate_00", 0, 2),
        /* 3  */ MuseumRow({ 0x000, 0x006, 0x008, 0x00c, 0x101 }, "stage_gr02", 0, 3),
        /* 4  */ MuseumRow({ 0x017, 0x01d, 0x02d, 0x044, 0x101 }, "stage_gr03", 0, 4),
        /* 5  */ MuseumRow({ 0x068, 0x101, 0x101, 0x101, 0x101 }, "stage_gate_01", 0, 5),
        /* 6  */ MuseumRow({ 0x001, 0x004, 0x009, 0x00e, 0x101 }, "stage_gr04", 0, 6),
        /* 7  */ MuseumRow({ 0x019, 0x01f, 0x02e, 0x045, 0x101 }, "stage_gr05", 0, 7),
        /* 8  */ MuseumRow({ 0x06b, 0x101, 0x101, 0x101, 0x101 }, "stage_gate_02", 0, 8),
        /* 9  */ MuseumRow({ 0x00f, 0x025, 0x034, 0x04a, 0x05e }, "room_LED", 0, 8),
        /* 10 */ MuseumRow({ 0x05a, 0x027, 0x02c, 0x049, 0x060 }, "room_00", 0, 0),
        /* 11 */ MuseumRow({ 0x012, 0x022, 0x038, 0x046, 0x061 }, "room_01", 0, 1),
        /* 12 */ MuseumRow({ 0x010, 0x028, 0x032, 0x047, 0x062 }, "room_02", 0, 2),
        /* 13 */ MuseumRow({ 0x018, 0x024, 0x037, 0x043, 0x063 }, "room_03", 0, 3),
        /* 14 */ MuseumRow({ 0x011, 0x026, 0x035, 0x04d, 0x064 }, "room_04", 0, 4),
        /* 15 */ MuseumRow({ 0x01b, 0x023, 0x036, 0x04b, 0x065 }, "room_05", 0, 5),
        /* 16 */ MuseumRow({ 0x01c, 0x021, 0x03c, 0x048, 0x101 }, "stage_sk00", 1, 0),
        /* 17 */ MuseumRow({ 0x014, 0x02a, 0x03f, 0x042, 0x101 }, "stage_sk01", 1, 1),
        /* 18 */ MuseumRow({ 0x01a, 0x01e, 0x02f, 0x04c, 0x101 }, "stage_sk02", 1, 2),
        /* 19 */ MuseumRow({ 0x06a, 0x101, 0x101, 0x101, 0x101 }, "stage_gate_03", 1, 3),
        /* 20 */ MuseumRow({ 0x04e, 0x052, 0x056, 0x057, 0x066 }, "room_06", 0, 6),
        /* 21 */ MuseumRow({ 0x050, 0x051, 0x054, 0x058, 0x067 }, "room_07", 0, 7),
        /* 22 */ MuseumRow({ 0x04f, 0x053, 0x055, 0x05d, 0x05f }, "room_TED", 0, 9),
        /* 23 */ MuseumRow({ 0x013, 0x015, 0x016, 0x101, 0x101 }, "bonus_AGB", 0, 0),
        /* 24 */ MuseumRow({ 0x020, 0x029, 0x05b, 0x101, 0x101 }, "bonus_NTR", 0, 0),
        /* 25 */ MuseumRow({ 0x02b, 0x030, 0x031, 0x101, 0x101 }, "bonus_RVL0", 0, 0),
        /* 26 */ MuseumRow({ 0x033, 0x039, 0x03a, 0x101, 0x101 }, "bonus_RVL1", 0, 0),
        /* 27 */ MuseumRow({ 0x03b, 0x03d, 0x03e, 0x101, 0x101 }, "bonus_RVL2", 0, 0),
        /* 28 */ MuseumRow({ 0x040, 0x041, 0x05c, 0x101, 0x101 }, "bonus_RVL3", 0, 0),
    };

    const MuseumRowColor MuseumRowColors[MuseumRowCount] {
        /* E2 */ MuseumRowColor(rgba(0x424242FF), rgba(0x00000000)),
        /* E1 */ MuseumRowColor(rgba(0x424242FF), rgba(0x00000000)),
        /* E0 */ MuseumRowColor(rgba(0x424242FF), rgba(0x00000000)),

        /* 0  */ MuseumRowColor(rgba(0xFFED2AFF), rgba(0x8C640000)),
        /* 1  */ MuseumRowColor(rgba(0x4AF8F1FF), rgba(0x01494C00)),
        /* 2  */ MuseumRowColor(rgba(0xDCD9D9FF), rgba(0x85561600)),
        /* 3  */ MuseumRowColor(rgba(0xFFA030FF), rgba(0x783A0000)),
        /* 4  */ MuseumRowColor(rgba(0x3667CAFF), rgba(0x04007800)),
        /* 5  */ MuseumRowColor(rgba(0xCECACAFF), rgba(0x82393900)),
        /* 6  */ MuseumRowColor(rgba(0xEB4040FF), rgba(0x4F000000)),
        /* 7  */ MuseumRowColor(rgba(0x9A43B7FF), rgba(0x0F013F00)),
        /* 8  */ MuseumRowColor(rgba(0xAFAFAFFF), rgba(0x284D2F00)),
        /* 9  */ MuseumRowColor(rgba(0x2BA820FF), rgba(0x013F0D00)),
        /* 10 */ MuseumRowColor(rgba(0x2BA820FF), rgba(0x013F0D00)),
        /* 11 */ MuseumRowColor(rgba(0x2BA820FF), rgba(0x013F0D00)),
        /* 12 */ MuseumRowColor(rgba(0x2BA820FF), rgba(0x013F0D00)),
        /* 13 */ MuseumRowColor(rgba(0x2BA820FF), rgba(0x013F0D00)),
        /* 14 */ MuseumRowColor(rgba(0x2BA820FF), rgba(0x013F0D00)),
        /* 15 */ MuseumRowColor(rgba(0x2BA820FF), rgba(0x013F0D00)),
        /* 16 */ MuseumRowColor(rgba(0xFFCAEDFF), rgba(0x7E3E9200)),
        /* 17 */ MuseumRowColor(rgba(0xB7ECE2FF), rgba(0x2E818600)),
        /* 18 */ MuseumRowColor(rgba(0xECE5B7FF), rgba(0xADAA0800)),
        /* 19 */ MuseumRowColor(rgba(0x8C8C8CFF), rgba(0x503C5000)),
        /* 20 */ MuseumRowColor(rgba(0xF7F2B9FF), rgba(0xAA943C00)),
        /* 21 */ MuseumRowColor(rgba(0xF7F2B9FF), rgba(0xAA943C00)),
        /* 22 */ MuseumRowColor(rgba(0xF7F2B9FF), rgba(0xAA943C00)),
        /* 23 */ MuseumRowColor(rgba(0x78500AFF), rgba(0x643C3200)),
        /* 24 */ MuseumRowColor(rgba(0x78500AFF), rgba(0x643C3200)),
        /* 25 */ MuseumRowColor(rgba(0x78500AFF), rgba(0x643C3200)),
        /* 26 */ MuseumRowColor(rgba(0x78500AFF), rgba(0x643C3200)),
        /* 27 */ MuseumRowColor(rgba(0x78500AFF), rgba(0x643C3200)),
        /* 28 */ MuseumRowColor(rgba(0x78500AFF), rgba(0x643C3200)),
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

    void PatchMuseumExtraRows() {
        for (auto address : Region::MuseumRowsInfoAddresses()) {
            Process::Patch(address, (u32) MuseumRows);
        }

        u32 compare_r1_instruction = // cmp r1, MUSEUM_ROW_COUNT
            make_cmp_immediate_instruction(1, MuseumRowCount);

        for (auto address : Region::MuseumRowsR1Cmps()) {
            Process::Patch(address, compare_r1_instruction);
        }

        u32 compare_r8_instruction = // cmp r1, MUSEUM_ROW_COUNT
            make_cmp_immediate_instruction(1, MuseumRowCount);

        for (auto address : Region::MuseumRowsR8Cmps()) {
            Process::Patch(address, compare_r8_instruction);
        }

        // FIXME: for this to work we need to stop c++ from overwriting the
        // colors when it runs the constructor (@ 0x38DE58) for the array
        for (auto address : Region::MuseumRowsColorsAddresses()) {
            Process::Patch(address, (u32) MuseumRowColors);
        }
    }
}
