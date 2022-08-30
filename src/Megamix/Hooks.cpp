#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "rt.h"

#include "Megamix.hpp"
#include "Config.hpp"

using Megamix::TempoTable;

extern "C" {
    extern void* getTickflowOffset(int index);
    extern void* getGateTickflowOffset(int index);
    extern TempoTable* getTempoStrm(Megamix::CSoundManager* this_, u32 id);
    extern TempoTable* getTempoSeq(Megamix::CSoundManager* this_, u32 id);
    extern TempoTable* getTempoAll(Megamix::CSoundManager* this_, u32 id);
}

namespace Megamix::Hooks {
    RT_HOOK tickflowHook;
    RT_HOOK gateHook;
    RT_HOOK tempoStrmHook;
    RT_HOOK tempoSeqHook;
    RT_HOOK tempoAllHook;
    
    void TickflowHooks() {
        rtInitHook(&tickflowHook, Region::TickflowHookFunc(), (u32)getTickflowOffset);
        rtEnableHook(&tickflowHook);
        rtInitHook(&gateHook, Region::GateHookFunc(), (u32)getGateTickflowOffset);
        rtEnableHook(&gateHook);
    }

    void TempoHooks() {
        rtInitHook(&tempoStrmHook, Region::StrmTempoHookFunc(), (u32)getTempoStrm);
        rtEnableHook(&tempoStrmHook);
        rtInitHook(&tempoSeqHook, Region::SeqTempoHookFunc(), (u32)getTempoSeq);
        rtEnableHook(&tempoSeqHook);
        rtInitHook(&tempoAllHook, Region::AllTempoHookFunc(), (u32)getTempoAll);
        rtEnableHook(&tempoAllHook);
    }

    void DisableAllHooks() {
        rtDisableHook(&tickflowHook);
        rtDisableHook(&gateHook);
        rtDisableHook(&tempoStrmHook);
        rtDisableHook(&tempoSeqHook);
        rtDisableHook(&tempoAllHook);
    }
}

void* getTickflowOffset(int index) {
    if (config->tickflows.contains(index)) {
        int result = Megamix::btks.LoadFile(config->tickflows[index]);
        if (!result) {
            return (void*)(Megamix::btks.start);
        } else {
            CTRPluginFramework::MessageBox("Error messages", CTRPluginFramework::Utils::Format("BTKS loader: %s", Megamix::ErrorMessage(result).c_str()))();
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
            CTRPluginFramework::MessageBox("Error messages", CTRPluginFramework::Utils::Format("BTKS loader: %s", Megamix::ErrorMessage(result).c_str()))();    
        }
    }
    return *(void**)(Region::GateTable() + index * 0x24 + 4); // og code
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