#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include <string>

#include "Megamix.hpp"

using CTRPluginFramework::Process;
using CTRPluginFramework::Utils;
using CTRPluginFramework::Screen;
using CTRPluginFramework::Color;
using CTRPluginFramework::Controller;
using CTRPluginFramework::Key;
using CTRPluginFramework::OSD;

//TODO: add enum
namespace Megamix {
    u8 errorImg[] = {};

    std::string ErrorMessage(int code) {
        switch (code) {
            // CTRPF errors
            case 0:
                return "No errors!";
            case -1:
                return "Invalid path for file";
            case -2:
                return "File not open, are you sure it exists?";
            case -3:
                return "The mode passed when opened the file doesn't allow this operation";
            case -4:
                return "Invalid argument on file reading";
            case -5:
                return "Unexpected error on file reading";
            
            // Saltwater / BTKS errors
            case -6:
                return "Not a BTKS file";
            case -7:
                return "Unsupported BTKS version";
            case -8:
                return "Not implemented";
            case -9:
                return "Unknown BTKS section type";
            case -10:
                return "Unknown BTKS pointer type";
            case -11:
                return "Missing BTKS section";
            case -12:
                return "File does not fit on memory";
            case -13:
                return "Unsupported Tickflow format for this game and version";
            
            default:
                return "Unknown error code";
        }
    }

    const char* MemSection(u32 far) {
        if (far < 0x00100000) {
            return "NULL";
        //TODO: game sections
        /*
        } else if (far < Region::TextEnd()) {
            return "TEXT";
        } else if (far < Region::RodataEnd()) {
            return "RODA";
        } else if (far < Region::DataEnd()) {
            return "DATA";
        */
        } else if (far < 0x04000000) {
            // return "BSSO";
            return "CODE";
        } else if (far >= 0x06000000 && far < 0x07000000) {
            return "SLWM";
        //TODO: Saltwater sections
        } else if (far < 0x08000000) {
            return "SLWT";
        } else if (far < 0x10000000) {
            return "MMEM";
        } else if (far < 0x14000000) {
            return "SMEM";
        } else if (far < 0x18000000) {
            return "LINE";
        } else if (far >= 0x1E800000 && far < 0x1F000000) {
            return "NMEM";
        } else {
            return "UNKN";
        }
    }

    std::string CrashCode(ERRF_ExceptionInfo* info, CpuRegisters* regs) {
        u32 fsr_status = (info->fsr & 0xf);
        switch (info->type) {
            case ERRF_EXCEPTION_PREFETCH_ABORT:
                return Utils::Format("0-%02d-%s-%08X", fsr_status, MemSection(regs->pc), regs->pc);
            case ERRF_EXCEPTION_DATA_ABORT:
                fsr_status += ((info->fsr >> 10) & 1) * 0x10;
                return Utils::Format("1-%02d-%s-%07X-%08X", fsr_status, MemSection(info->far), regs->pc, info->far);
            case ERRF_EXCEPTION_VFP:
                return Utils::Format("2-%07X", regs->pc);
            case ERRF_EXCEPTION_UNDEFINED:
                return Utils::Format("3-%07X", regs->pc);
            default:
                return "INVALID";
        }
    }

    static bool first = true;
    static bool full_info = false;
    static bool faded = false;
    static std::string dump_location = "";

    namespace ErrorScreen {
        void InfoScreen(ERRF_ExceptionInfo* info, CpuRegisters* regs) {
            Screen screen = OSD::GetTopScreen();
            if (!faded) {
                screen.Fade(0.3);
                faded = true;
            }
            screen.DrawRect(16, 16, 368, 208, Color(0, 0, 0));

            u32 posY = 20;
            posY = screen.Draw("Oh, no!", 20, posY, Color(255, 0, 0));
            posY = screen.Draw(Utils::Format("Error %s", CrashCode(info, regs).c_str()), 20, posY, Color(255, 127, 127));
            posY += 10;
            posY = screen.Draw("Something went wrong!", 20, posY);
            posY = screen.Draw("But don't panic, report the error to the SpiceRack", 20, posY);
            posY = screen.Draw("Discord server (discord.gg/xAKFPaERRG)", 20, posY);
            posY += 10;
            
            if (dump_location != "") {
                posY = screen.Draw(std::string("Crash dumped to ").append(dump_location), 20, posY);
                posY += 10;
            } else {
                posY = screen.Draw("> Press A to dump crash (WIP)", 20, posY);
            }
            posY = screen.Draw("> Press B to return to the home menu", 20, posY);
            posY = screen.Draw("> Press Y to see dev info (WIP)", 20, posY);
            
            OSD::SwapBuffers();
        }

        void DevScreen(ERRF_ExceptionInfo* info, CpuRegisters* regs) {
            Screen screen = OSD::GetTopScreen();
            screen.DrawRect(16, 16, 368, 208, Color(0, 0, 0));

            u32 posY = 20;

            posY = screen.Draw("Debug info", 20, posY);
            posY = screen.Draw(Utils::Format("@ %08x -> %08x (@ PC -> LR)", regs->pc, regs->lr), 20, posY);
            posY = screen.Draw(Utils::Format("Exception type %d", info->type), 20, posY);
            posY += 10;
            posY = screen.Draw("Call stack goes here", 20, posY);
            posY += 10;

            if (dump_location != "") {
                posY = screen.Draw(std::string("Crash dumped to ").append(dump_location), 20, posY);
                posY += 10;
            } else {
                posY = screen.Draw("> Press A to dump crash (WIP)", 20, posY);
            }
            posY = screen.Draw("> Press B to return to the home menu", 20, posY);
            posY = screen.Draw("> Press Y to go back", 20, posY);

            OSD::SwapBuffers();

        }
    }

    Process::ExceptionCallbackState CrashHandler(ERRF_ExceptionInfo* info, CpuRegisters* regs) {
        if (first) {
            first = false;
            if (full_info) {
                ErrorScreen::DevScreen(info, regs);
            } else {
                ErrorScreen::InfoScreen(info, regs);
            }
        }
        
        Controller::Update();
       if (Controller::IsKeyPressed(Key::B)) {
            return Process::ExceptionCallbackState::EXCB_RETURN_HOME;
        } else {
            if (Controller::IsKeyPressed(Key::A)) {
                // todo: export actual crash instead of just going to luma
                return Process::ExceptionCallbackState::EXCB_DEFAULT_HANDLER;
            } else if (Controller::IsKeyPressed(Key::Y)) {
                first = true;
                full_info = !full_info;
            }
            return Process::ExceptionCallbackState::EXCB_LOOP;
        }
    }
}
