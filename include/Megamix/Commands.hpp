#ifndef MEGAMIX_COMMANDS_HPP
#define MEGAMIX_COMMANDS_HPP

#include "Megamix/Types.hpp"

namespace Megamix{
    enum CustomCommands: u16{
        InputCheck = 0x200,
        VersionNumber = 0x201,
        LanguageCheck = 0x202,
        MSBTWithNum = 0x206,
        
        DisplayCondvar = 0x300
    };

    void tickflowCommandsHook();
}

#endif
