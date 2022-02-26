#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "rt.h"

extern "C" {
    int testInt = 0;

    int* getAssetsOffset(int gameId) {
        testInt = gameId;
        return *(int**)(0x52b498 + gameId * 0x34 + 4); // original code
    }
}

static NAKED void getAssetsOffset_wrapper() {
    __asm__ ("\
        .extern getAssetsOffset  \n\
        b getAssetsOffset          \
    ");
}

//todo: internationalize offsets
namespace Megamix::Hooks {
    RT_HOOK testHook;
    
    void Test() {
        rtInitHook(&testHook, 0x258df4, (u32)getAssetsOffset_wrapper);
        rtEnableHook(&testHook);
    }
}