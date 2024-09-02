#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "external/rt.h"

#include "Megamix.hpp"
#include "Config.hpp"

#include "./HookFuncs/HookFuncs.h"

namespace Megamix::Hooks {
    // Hook functions have been moved to the HookFuncs folder

    RT_HOOK tickflowHook;
    RT_HOOK gateHook;
    RT_HOOK gatePracHook;

    RT_HOOK tempoStrmHook;
    RT_HOOK tempoSeqHook;
    RT_HOOK tempoAllHook;

    RT_HOOK regionFSHook;
    RT_HOOK regionOtherHook;

    RT_HOOK tickflowCommandsHook;

    RT_HOOK fsOpenFileHook;


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

    void FSHooks() {
        rtInitHook(&fsOpenFileHook, Region::DoOpenFileHookFunc(), (u32)doReadFile);
        rtEnableHook(&fsOpenFileHook);
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
        rtDisableHook(&fsOpenFileHook);
    }

    template<typename T>
    T StubbedFunction() {
        return {};
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
    template<>
    void StubbedFunction<void>() {
    }

    template void StubFunction<void>(u32);

}
