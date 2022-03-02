#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "rt.h"

#include "Megamix.hpp"

int testInt = 0;

extern "C" {
    extern void* getTickflowOffset(int index);
}

static NAKED void getTickflowOffset_wrapper() {
    __asm__ ("\
        .extern getTickflowOffset  \n\
        b getTickflowOffset          \
    ");
}

void* getTickflowOffset(int index) {
        testInt = index;
        if (index == 0x4A) {
            return *(void**)(Region::GameTable() + index * 0x34 + 4); // original code
        } else {
            return *(void**)(Region::GameTable() + 0x4A * 0x34 + 4);  // troll
        }
    }

//todo: internationalize offsets
namespace Megamix::Hooks {
    RT_HOOK testHook;
    
    void TickflowHook() {
        rtInitHook(&testHook, Region::TickflowHookFunc(), (u32)getTickflowOffset_wrapper);
        rtEnableHook(&testHook);
    }
}