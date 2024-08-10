#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

using CTRPluginFramework::OSD;

using Megamix::TempoTable;

namespace Megamix::Hooks {
    void* getTickflowOffset(int index) {
        if (config->tickflows.contains(index)) {
            int result = Megamix::btks.LoadFile(config->tickflows[index]);
            if (!result) {
                return (void*)(Megamix::btks.start);
            } else {
                OSD::Notify(CTRPluginFramework::Utils::Format("Error: %s", Megamix::ErrorMessage(result).c_str()));
            }
        }
        return *(void**)(Region::GameTable() + index * 0x34 + 4);  // og code
    }

    void* getGateTickflowOffset(int index) {
        if (config->tickflows.contains(index + 0x100)) {
            int result = Megamix::btks.LoadFile(config->tickflows[index + 0x100]);
            if (!result) {
                return (void*)(Megamix::btks.start);
            } else {
                OSD::Notify(CTRPluginFramework::Utils::Format("Error: %s", Megamix::ErrorMessage(result).c_str()));    
            }
        }
        return *(void**)(Region::GateTable() + index * 0x24 + 4); // og code
    }

    void* getGatePracticeTickflowOffset(int index) {
        if (config->tickflows.contains((index >> 2) + 0x110)) {
            int result = Megamix::btks.LoadFile(config->tickflows[(index >> 2) + 0x110]);
            if (!result) {
                return (void*)(Megamix::btks.start);
            } else {
                OSD::Notify(CTRPluginFramework::Utils::Format("Error: %s", Megamix::ErrorMessage(result).c_str()));    
            }
        }
        return *(void**)(Region::GateTable() + index * 0x24 + 8); // og code
    }

    TempoTable* getTempoStrm(Megamix::CSoundManager* this_, u32 id) {
        if (Megamix::btks.tempos.contains(id)) {
            return Megamix::btks.tempos[id];
        } else { // Original code
            for (s32 low = 0, high = this_->numberTempos; low <= high;) {
                s32 current_num = (low + high) / 2;
                Megamix::SM_TempoTable* current = &this_->tableStrm[current_num];
                if (current->id > id) high = current_num - 1;
                if (current->id < id) low = current_num + 1;
                if (current->id == id) return current->tempo;
            }
            return 0;
        }
    }

    TempoTable* getTempoSeq(Megamix::CSoundManager* this_, u32 id) {
        if (Megamix::btks.tempos.contains(id)) {
            return Megamix::btks.tempos[id];
        } else { // Original code
            for (s32 low = 0, high = this_->numberTempos; low <= high;) {
                s32 current_num = (low + high) / 2;
                Megamix::SM_TempoTable* current = &this_->tableSeq[current_num];
                if (current->id > id) high = current_num - 1;
                if (current->id < id) low = current_num + 1;
                if (current->id == id) return current->tempo;
            }
            return 0;
        }
    }

    TempoTable* getTempoAll(Megamix::CSoundManager* this_, u32 id) {
        if (Megamix::btks.tempos.contains(id)) {
            return Megamix::btks.tempos[id];
        } else { // Original code
            for (int i = 0; i < this_->numberTempos; i++) {
                Megamix::TempoTable* current = &this_->tempoTable[i];
                if (current->id1 == id || current->id2 == id)
                    return current;
            }
            return 0;
        }
    }
}