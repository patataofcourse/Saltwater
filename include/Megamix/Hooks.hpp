#ifndef MEGAMIX_HOOKS_H
#define MEGAMIX_HOOKS_H

#include <3ds/types.h>

namespace Megamix::Hooks {
    void initTickflowHooks();
    void initTempoHooks();
    void initRegionHooks();
    void initCommandHooks();

    void disableTickflowHooks();
    void disableTempoHooks();
    void disableRegionHooks();
    void disableCommandHooks();
    inline void disableAllHooks() {
        disableTickflowHooks();
        disableTempoHooks();
        disableRegionHooks();
        disableCommandHooks();
    }

    template<typename T> void StubFunction(u32 address);
}

#endif
