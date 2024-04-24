#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"

namespace Megamix{
extern "C" void tickflowCommandsHook(CTickflow* self, u32 cmd_num, u32 arg0, u32* args){
        if (cmd_num == VersionNumber){ //Returns version number
            if (arg0 == 0){ //RHMPatch version
                self->mCondvar = 0x103;
            } else if (arg0 == 1){ //Saltwater version
                self->mCondvar = 0x3;
            }
        }
    }


    void tickflowCommandsHookWrapper() {
        asm(
            "mov r6, r0\n"
            "bl tickflowCommandsHook\n"
        );
    }
}