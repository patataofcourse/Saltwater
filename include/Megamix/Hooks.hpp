#ifndef MEGAMIX_HOOKS_H
#define MEGAMIX_HOOKS_H

#include <CTRPluginFramework.hpp>

extern int testInt;

namespace Megamix::Hooks {
    extern CTRPluginFramework::HookResult result;
    void Test();
}

#endif