#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "rt.h"

#include "Megamix.hpp"
#include "Config.hpp"

extern "C" {
    extern void* getTickflowOffset(int index);
    extern void* getGateTickflowOffset(int index);
    extern void* getTempoStrm(Megamix::CSoundManager* this_, u32 id);
}

static NAKED void getTickflowOffset_wrapper() {
    __asm__ ("\
        .extern getTickflowOffset  \n\
        b getTickflowOffset          \
    ");
}

static NAKED void getGateTickflowOffset_wrapper() {
    __asm__ ("\
        .extern getGateTickflowOffset \n\
        b getGateTickflowOffset         \
    ");
}

static NAKED void getTempoStrm_wrapper() {
    __asm__ ("\
        .extern getTempoStrm \n\
        b getTempoStrm         \
    ");
}

namespace Megamix::Hooks {
    RT_HOOK tickflowHook;
    RT_HOOK gateHook;
    RT_HOOK tempoStrmHook;
    
    void TickflowHooks() {
        rtInitHook(&tickflowHook, Region::TickflowHookFunc(), (u32)getTickflowOffset_wrapper);
        rtEnableHook(&tickflowHook);
        rtInitHook(&gateHook, Region::GateHookFunc(), (u32)getGateTickflowOffset_wrapper);
        rtEnableHook(&gateHook);
        rtInitHook(&tempoStrmHook, Region::StrmTempoHookFunc(), (u32)getTempoStrm_wrapper);
        rtEnableHook(&tempoStrmHook);
    }

    void DisableAllHooks() {
        rtDisableHook(&tickflowHook);
        rtDisableHook(&gateHook);
        rtDisableHook(&tempoStrmHook);
    }
}

void* getTickflowOffset(int index) {
    if (config->tickflows.contains(index)) {
        int result = Megamix::btks.LoadFile(config->tickflows[index]);
        if (result) {
            CTRPluginFramework::MessageBox("Error messages", CTRPluginFramework::Utils::Format("BTKS loader: %s", Megamix::ErrorMessage(result).c_str()))();
            return *(void**)(Region::GameTable() + index * 0x34 + 4);  // og code
        }
        return (void*)(Megamix::btks.start);
    } else {
        return *(void**)(Region::GameTable() + index * 0x34 + 4);  // og code
    }
}

void* getGateTickflowOffset(int index) {
    if (config->tickflows.contains(index + 0x100)) {
        int result = Megamix::btks.LoadFile(config->tickflows[index + 0x100]);
        if (result) {
            CTRPluginFramework::MessageBox("Error messages", CTRPluginFramework::Utils::Format("BTKS loader: %s\n%s", Megamix::ErrorMessage(result).c_str(), config->tickflows[index+0x100]))();    
            return *(void**)(Region::GateTable() + index * 0x24 + 4); // og code
        }
        return (void*)(Megamix::btks.start);
    } else {
        return *(void**)(Region::GateTable() + index * 0x24 + 4); // og code
    }
}

void* getTempoStrm(Megamix::CSoundManager* this_, u32 id) {
    if (Megamix::btks.tempos.contains(id)) {
        return 0; //TODO: still need a bunch more stuff before this
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