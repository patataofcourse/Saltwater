#ifndef MEGAMIX_COMMANDS_HPP
#define MEGAMIX_COMMANDS_HPP

#include "Megamix/Types.hpp"

namespace Megamix{
    enum CustomCommands: u16{
        InputCheck = 0x200,
        VersionNumber = 0x201,
        LanguageCheck = 0x202,
        
        DisplayCondvar = 0x300
    };

    void tickflowCommandsHookWrapper();

    extern "C" int tickflowCommandsHook(CTickflow* self, u32 cmd_num, u32 arg0, u32* args);

    void input_cmd(CTickflow* self, u32 arg0, u32* args);
    void versionCheck(CTickflow* self, u32 arg0, u32* args);
    void languageCheck(CTickflow* self, u32 arg0, u32* args);
    void displayCondvar(CTickflow* self, u32 arg0, u32* args);
}

#endif
