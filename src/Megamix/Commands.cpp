#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"

namespace Megamix{

    void tickflowCommandsHookWrapper() {
        asm(
            "mov r0, r6\n"
            "bl tickflowCommandsHook\n"
            "bx r0\n"
        );
    }

    extern "C" __attribute__((used)) 
    int tickflowCommandsHook(CTickflow* self, u32 cmd_num, u32 arg0, u32* args){
        if (cmd_num == VersionNumber){ //Returns version number
            if (arg0 == 0){ //RHMPatch version
                self->mCondvar = SUPPORTED_RHMPATCH;
            } else if (arg0 == 1){ //Saltwater version
                self->mCondvar = VERSION_MAJOR*0x100+VERSION_MINOR;
            }
        }
        return Region::TickflowCommandsEnd();
    }

}
