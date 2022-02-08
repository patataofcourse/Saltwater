#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix/Region.hpp"

using namespace CTRPluginFramework;
using std::vector;

namespace Megamix {
    char* rhmpatchBuffer = new char[0x200000]; //Should be 2MB

    int LoadC00Bin(u32 region, const std::string &path="_:/spicerack/C00.bin") {
        File file(path + '.' + Region::RegionCode(region), File::Mode::READ);
        u32 result = file.Read(rhmpatchBuffer, 0x200000);
        return result;
    }

    void PatchTickflowAddresses(u32 region) {

        // Game table
        vector<u32> game_addresses = Region::RHMPatchGameAddresses(region);
        u32 game_offset = (u32) rhmpatchBuffer;
        for (int address: game_addresses) {
            Process::Patch(address, game_offset);
        }

        // Tempo table
        vector<u32> tempo_addresses = Region::RHMPatchTempoAddresses(region); // Stays the same for all 3 supported regions
        
        u32 tempo_offset = (u32) rhmpatchBuffer + 0x1588;
        for (int address: tempo_addresses) {
            Process::Patch(address, tempo_offset);
        }

        // Gate table
        vector<u32> gate_addresses = Region::RHMPatchGateAddresses(region);
        u32 gate_offset = (u32) rhmpatchBuffer + 0x3358;
        for (int address: gate_addresses) {
            Process::Patch(address, gate_offset);
        }
    }
}