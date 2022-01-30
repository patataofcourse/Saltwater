#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix/Region.hpp"

using namespace CTRPluginFramework;
using std::vector;

namespace Megamix {
    int* result = new int[3];

    int* LoadC00Bin(const std::string &path="_:/rhmm/C00.bin") {
        #define C00_SIZE 0x200000

        char* rhmpatchBuffer = new char[C00_SIZE];
        File* file = new File();

        result[0] = File::Open(*file, path, File::Mode::READ);

        result[1] = file->Read(rhmpatchBuffer, C00_SIZE);

        result[2] = 0xC0000000; //Process::GetFreeMemoryRegion(C00_SIZE);

        Process::Patch(result[2], rhmpatchBuffer, C00_SIZE);

        return result;
    }

    void PatchTickflowAddresses(u64 region) {
        
        // Go wild.

        //if (region != Region::US) return; //IDK if it works in EU or now, should prob try

        // Tickflow table
        vector<u32> tf_addresses = {
            0x109008, 0x22D57C, 0x22D67C, 0x22D698, 0x22D6B4, 0x22D6D0, 0x240458, 0x24CB28, 0x2553CC, 0x255578, 0x258618,
            0x258E0C, 0x32D434, 0x32D450, 0x32D470, 0x32D4C8, 0x32D548, 0x32D5B0, 0x32D5E8
        };
        u32 tf_offset = result[2];
        for (int address: tf_addresses) {
            Process::Patch(address, tf_offset);
        }

        // Tempo table
        vector<u32> tempo_addresses = { 0x101C10, 0x12B3B0 };
        u32 tempo_offset = result[2] + 0x1588;
        for (int address: tempo_addresses) {
            Process::Patch(address, tempo_offset);
        }

        // Gate table
        vector<u32> gate_addresses = { 0x22AE40, 0x240FB0, 0x2552D8, 0x32D5FC, 0x32D614, 0x32D62C, 0x32D644, 0x32D65C, 0x32D6B8, 0x32D770 };
        u32 gate_offset = result[2] + 0x3358;
        for (int address: gate_addresses) {
            Process::Patch(address, gate_offset);
        }
    }
}