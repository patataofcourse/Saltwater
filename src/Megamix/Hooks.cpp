#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "rt.h"

#include "Megamix.hpp"

int testInt = 0;

extern "C" {
    extern int* getTickflowOffset(int index);
}

static NAKED void getTickflowOffset_wrapper() {
    __asm__ ("\
        .extern getTickflowOffset  \n\
        b getTickflowOffset          \
    ");
}

int* getTickflowOffset(int index) {
        testInt = index;
        if (index == 0x4A) {
            return *(int**)(0x52b498 + index * 0x34 + 4); // original code
        } else {
            return  *(int**)(0x52b498 + 0x4A * 0x34 + 4); // troll
        }
    }

//todo: internationalize offsets
namespace Megamix::Hooks {
    RT_HOOK testHook;
    
    void TickflowHook() {
        rtInitHook(&testHook, 0x258df4, (u32)getTickflowOffset_wrapper);
        rtEnableHook(&testHook);
    }
}