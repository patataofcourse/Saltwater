#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix/Region.hpp"

using namespace CTRPluginFramework;
using std::vector;

namespace Megamix {
    char* rhmpatchBuffer = new char[0x200000]; //Should be 2MB

    int LoadC00Bin(const std::string &path="_:/saffron/C00.bin") {
        File* file = new File();
        File::Open(*file, path, File::Mode::READ);
        u32 result = file->Read(rhmpatchBuffer, 0x200000);
        return result;
    }

    void PatchTickflowAddresses(u64 region) {
        if (region != Region::US) return; //IDK if it works in EU or now, should prob try

        // Game table
        vector<u32> game_addresses = {
            0x109008, 0x22D57C, 0x22D67C, 0x22D698, 0x22D6B4, 0x22D6D0, 0x240458, 0x24CB28, 0x2553CC, 0x255578, 0x258618,
            0x258E0C, 0x32D434, 0x32D450, 0x32D470, 0x32D4C8, 0x32D548, 0x32D5B0, 0x32D5E8
        };
        u32 game_offset = (u32) rhmpatchBuffer;
        for (int address: game_addresses) {
            Process::Patch(address, game_offset);
        }

        // Tempo table
        vector<u32> tempo_addresses = { 0x101C10, 0x12B3B0 };
        u32 tempo_offset = (u32) rhmpatchBuffer + 0x1588;
        for (int address: tempo_addresses) {
            Process::Patch(address, tempo_offset);
        }

        // Gate table
        vector<u32> gate_addresses = { 0x22AE40, 0x240FB0, 0x2552D8, 0x32D5FC, 0x32D614, 0x32D62C, 0x32D644, 0x32D65C, 0x32D6B8, 0x32D770 };
        u32 gate_offset = (u32) rhmpatchBuffer + 0x3358;
        for (int address: gate_addresses) {
            Process::Patch(address, gate_offset);
        }

        // Process::Pause();
    }
}