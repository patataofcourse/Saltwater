#ifndef MEGAMIX_ERROR_HPP
#define MEGAMIX_ERROR_HPP

#include <string>
#include <CTRPluginFramework.hpp>

#define CALL_STACK_SIZE 5

using CTRPluginFramework::Process;

namespace Megamix {
    std::string ErrorMessage(int code);

    Process::ExceptionCallbackState CrashHandler(ERRF_ExceptionInfo* exception, CpuRegisters* regs);

    extern u8 errorImg[];

    enum class CrashType {
        Extended,
        Short,
    };

    struct ShortCrashInfo {
        CrashType type;
        u8 region;
        ERRF_ExceptionType excType;
        bool release;

        union {
            struct {
                u8 major;
                u8 minor;
                u8 patch;
            } ver;
            u32 commit;
        } version;

        u32 pc;
        u32 lr;
        u32 cpsr;
        u32 statusRegisterA; // IFSR, DFSR, FPEXC
        u32 statusRegisterB; // FAR, FPINST

        u32 callStack[CALL_STACK_SIZE];
    };

    struct CrashInfo {
        ShortCrashInfo info;

        u32 registers[14];
        u32 stackLength;
        u8 stackDump[0x100];
    };
}

#endif