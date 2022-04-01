#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "rt.h"

#include "Megamix.hpp"

int testInt = 0;

extern "C" {
    extern void* getTickflowOffset(int index);
    extern void* getGateTickflowOffset(int index);
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
        b getGateTickflowOffset       \n\
    ");
}

namespace Megamix::Hooks {
    RT_HOOK tickflowHook;
    RT_HOOK gateHook;
    
    void TickflowHooks() {
        rtInitHook(&tickflowHook, Region::TickflowHookFunc(), (u32)getTickflowOffset_wrapper);
        rtEnableHook(&tickflowHook);
        rtInitHook(&gateHook, Region::GateHookFunc(), (u32)getGateTickflowOffset_wrapper);
        rtEnableHook(&gateHook);
    }

    void DisableAllHooks() {
        rtDisableHook(&tickflowHook);
        rtDisableHook(&gateHook);
    }
}

void* getTickflowOffset(int index) {
    testInt = index;
    if (index == 0x4A) {
        return *(void**)(Region::GameTable() + index * 0x34 + 4); // original code
    } else {
        return *(void**)(Region::GameTable() + 0x4A * 0x34 + 4);  // troll
    }
}

void* getGateTickflowOffset(int index) {
    testInt = index + 0x100;
    return *(void**)(Region::GameTable() + 0x4A * 0x34 + 4); // still sumo lmao
}