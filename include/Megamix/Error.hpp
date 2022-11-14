#ifndef MEGAMIX_ERROR_HPP
#define MEGAMIX_ERROR_HPP

#include <string>
#include <CTRPluginFramework.hpp>

using CTRPluginFramework::Process;

namespace Megamix {
    std::string ErrorMessage(int code);

    Process::ExceptionCallbackState CrashHandler(ERRF_ExceptionInfo* exception, CpuRegisters* regs);

    extern u8 errorImg[];
}

#endif