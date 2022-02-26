#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "rt.h"

extern "C" {
    int testInt = 0;

    int* getStartOffset(int index) {
        testInt = index;
        if (index == 0x4A) {
            return *(int**)(0x52b498 + index * 0x34 + 4); // original code
        } else {
            return  *(int**)(0x52b498 + 0x4A * 0x34 + 4); // troll
        }
    }
}

static NAKED void getStartOffset_wrapper() {
    __asm__ ("\
        .extern getStartOffset  \n\
        b getStartOffset          \
    ");
}

//todo: internationalize offsets
namespace Megamix::Hooks {
    RT_HOOK testHook;
    
    void Test() {
        rtInitHook(&testHook, 0x258df4, (u32)getStartOffset_wrapper);
        rtEnableHook(&testHook);
    }
}