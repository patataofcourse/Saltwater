#ifndef MEGAMIX_COMMANDS_HPP
#define MEGAMIX_COMMANDS_HPP

#include "Megamix/Types.hpp"

namespace Megamix {
    void tickflowCommandsHook (CTickflow* self, u32 cmd_num, u32 arg0, u32* args);
}

#endif