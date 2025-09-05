#ifndef MEGAMIX_HOOKS_H
#define MEGAMIX_HOOKS_H

#include <3ds/types.h>

namespace Megamix::Hooks {
    void TickflowHooks();
    void TempoHooks();
    void RegionHooks();
    void CommandHook();
    void DisableAllHooks();

    template<typename T> T StubbedFunction();
    template<typename T> void StubFunction(u32 address);
}

#endif
