#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

using namespace CTRPluginFramework;
using std::vector;

namespace Megamix {
    char* rhmpatchBuffer = new char[0x200000]; //Should be 2MB

    int LoadC00Bin(const std::string path="C00.bin") {
        File file(MEGAMIX_BASE_PATH + path + '.' + Region::RegionCode(), File::Mode::READ);
        u32 result = file.Read(rhmpatchBuffer, 0x200000);
        return result;
    }

    void PatchTickflowAddresses(Config config) {
        if (region == Region::JP) {
            return;
        };

        // Game table
        if (config.game){
            vector<u32> game_addresses = Region::RHMPatchGameAddresses();
            u32 game_offset = (u32) rhmpatchBuffer;
            for (int address: game_addresses) {
                Process::Patch(address, game_offset);
            }
        }

        // Tempo table
        if (config.tempo) {
            vector<u32> tempo_addresses = Region::RHMPatchTempoAddresses();
            u32 tempo_offset = (u32) rhmpatchBuffer + 0x1588;
            for (int address: tempo_addresses) {
                Process::Patch(address, tempo_offset);
            }
        }
        
        // Gate table
        if (config.gate) {
            vector<u32> gate_addresses = Region::RHMPatchGateAddresses();
            u32 gate_offset = (u32) rhmpatchBuffer + 0x3358;
            for (int address: gate_addresses) {
                Process::Patch(address, gate_offset);
            }
        }
    }
}