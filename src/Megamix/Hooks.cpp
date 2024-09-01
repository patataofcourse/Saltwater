#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "external/rt.h"

#include "Megamix.hpp"
#include "Config.hpp"

using CTRPluginFramework::OSD;

using Megamix::TempoTable;

namespace Megamix::Hooks {
    RT_HOOK tickflowHook;
    RT_HOOK gateHook;
    RT_HOOK gatePracHook;

    RT_HOOK tempoStrmHook;
    RT_HOOK tempoSeqHook;
    RT_HOOK tempoAllHook;

    RT_HOOK regionFSHook;
    RT_HOOK regionOtherHook;

    RT_HOOK tickflowCommandsHook;

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

    u32 getRegionCTR() {
        //TODO: handle JP region / JP langpack
        if (region == Region::KR)
            return Region::KR_CTR;
        return region;
    }

    u32 getRegionMegamix() {
        //TODO: handle JP region / JP langpack
        return region;
    }


    void TickflowHooks() {
        rtInitHook(&tickflowHook, Region::TickflowHookFunc(), (u32)getTickflowOffset);
        rtEnableHook(&tickflowHook);
        rtInitHook(&gateHook, Region::GateHookFunc(), (u32)getGateTickflowOffset);
        rtEnableHook(&gateHook);
        rtInitHook(&gatePracHook, Region::GatePracHookFunc(), (u32)getGatePracticeTickflowOffset);
        rtEnableHook(&gatePracHook);
    }

    void TempoHooks() {
        rtInitHook(&tempoStrmHook, Region::StrmTempoHookFunc(), (u32)getTempoStrm);
        rtEnableHook(&tempoStrmHook);
        rtInitHook(&tempoSeqHook, Region::SeqTempoHookFunc(), (u32)getTempoSeq);
        rtEnableHook(&tempoSeqHook);
        rtInitHook(&tempoAllHook, Region::AllTempoHookFunc(), (u32)getTempoAll);
        rtEnableHook(&tempoAllHook);
    }

    void RegionHooks() {
        if (region != Region::JP){
            rtInitHook(&regionFSHook, Region::RegionFSHookFunc(), (u32)getRegionMegamix);
            rtEnableHook(&regionFSHook);
        }
        rtInitHook(&regionOtherHook, Region::RegionOtherHookFunc(), (u32)getRegionCTR);
        rtEnableHook(&regionOtherHook);
    }

    void CommandHook() {
        rtInitHook(&tickflowCommandsHook, Region::TickflowCommandsSwitch(), (u32)tickflowCommandsHookWrapper);
        rtEnableHook(&tickflowCommandsHook);
    }

    void DisableAllHooks() {
        rtDisableHook(&tickflowHook);
        rtDisableHook(&gateHook);
        rtDisableHook(&tempoStrmHook);
        rtDisableHook(&tempoSeqHook);
        rtDisableHook(&tempoAllHook);
        rtDisableHook(&regionFSHook);
        rtDisableHook(&regionOtherHook);
        rtDisableHook(&tickflowCommandsHook);
    }

    template<typename T>
    T StubbedFunction() {
        return {};
    }

    template<>
    void StubbedFunction<void>() {
    }

    // redirects function at address to a stubbed function
    template<typename T>
    void StubFunction(u32 address) {
        rtGenerateJumpCode(
            (u32)   StubbedFunction<T>,
            (u32 *) address
        );
    }

    // template instantiations
    // if either StubbedFunction or StubFunction is used with a type, a template
    // instantiation must be added with that type
    template void StubFunction<void>(u32);

}
