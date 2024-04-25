#ifndef MEGAMIX_COMMANDS_HPP
#define MEGAMIX_COMMANDS_HPP

#include "Megamix/Types.hpp"

namespace Megamix{
    enum CustomCommands: u16{
        VersionNumber = 0x201
    };

    extern "C" void tickflowCommandsHook(CTickflow*, u32, u32, u32*);

    void tickflowCommandsHookWrapper();
}

#endif
