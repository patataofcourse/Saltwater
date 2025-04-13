#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"

using CTRPluginFramework::OSD;
using CTRPluginFramework::Utils;

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
        switch(cmd_num){
             // Necessary, as our hook overrides case 0.
            case 0:
                return Region::TickflowAsyncSubLocation();
            
            // 0x200 range - regular custom commands
            case InputCheck:
                input_cmd(self, arg0, args);
                break;
            case VersionNumber:
                versionCheck(self, arg0, args);
                break;
            case LanguageCheck:
                languageCheck(self, arg0, args);
                break;
            case EndlessSave:
                endlessSave(self, arg0, args);
                break;
            
            // 0x300 range - debugging commands
            case DisplayCondvar:
                displayCondvar(self, arg0, args);
                break;
        }
        return Region::TickflowCommandsEnd();
    }

    void input_cmd(CTickflow* self, u32 arg0, u32* args) {
        if (arg0 > 2) {
            self->condvar = 0;
            return;
        } else if (arg0 == 2) {
            CSaveData** gSaveData = (CSaveData**)Region::GlobalSaveDataPointer();
            // Here, arg0 gets replaced by the playstyle - 0 for buttons, 1 for tap - Results in playstyle-dependant reading
            arg0 = (u32)(*gSaveData)->fileData[(*gSaveData)->currentFile].playStyle;
        }

        CInputManager** gInputManager = (CInputManager**)Region::GlobalInputManagerPointer();
        if (arg0 == 0) {
            if (args[0] >= 32) { // We're working with a 32-bit integer here, so flags are limited to bits 1-31
                self->condvar = 0;
                return;
            }
            
            self->condvar = ((u32)(*gInputManager)->padHandler->holdButtons >> args[0]) & 1;
        } else {
            self->condvar = ((*gInputManager)->touchPanelHandler->touchPanelStatus.touch);
        }
    }

    void versionCheck(CTickflow* self, u32 arg0, u32* args) {
        if (arg0 == 0){ //RHMPatch version
            self->condvar = SUPPORTED_RHMPATCH;
        } else if (arg0 == 1){ //Saltwater version
            self->condvar = VERSION_MAJOR*0x10+VERSION_MINOR;
        }
    }

    void languageCheck(CTickflow* self, u32 arg0, u32* args) {
        if (arg0 != 0) return;
        CSaveData** gSaveData = (CSaveData**)Region::GlobalSaveDataPointer();
        CFileManager** gFileManager = (CFileManager**)Region::GlobalFileManagerPointer();
        int saveLanguage = (*gSaveData)->fileData[(*gSaveData)->currentFile].locale;
        if(saveLanguage == 1){
            self->condvar = 0;
        } else {
            wchar_t sublocale[5];
            utf16_to_utf32((u32*)sublocale, (*gFileManager)->sublocale, 4);
            sublocale[4] = '\0';
            std::wstring localews(sublocale);
            if(localews.find(L"JP") != (unsigned int)-1){
                self->condvar = 0; 
            } else if (localews.find(L"EN") != (unsigned int)-1){
                self->condvar = 1; 
            } else if (localews.find(L"FR") != (unsigned int)-1) {
                self->condvar = 2; 
            } else if (localews.find(L"GE") != (unsigned int)-1) {
                self->condvar = 3; 
            } else if (localews.find(L"IT") != (unsigned int)-1) {
                self->condvar = 4; 
            } else if (localews.find(L"SP") != (unsigned int)-1) {
                self->condvar = 5; 
            } else if (localews.find(L"KR") != (unsigned int)-1) {
                self->condvar = 6; 
            } else {
                self->condvar = -1;
            }
        }
    }

    void endlessSave(CTickflow* self, u32 arg0, u32* args) {
        using enum Megamix::GateGameIndex;

        if (arg0 != 0) return;

        // alternatively, load the current slot loaded with the tickflow hook into a global, and use that instead
        // that way we can avoid the UB on non-gate slots
        GateGameIndex slot = (*Region::D_0054ef10())->currentGateSlot;
        OSD::Notify(Utils::Format("%d", slot), CTRPluginFramework::Color::Red);
        if ((slot & Difficulty) != Endless || !Region::IsGateGameValidFunc()(slot))
            return;
        
        CSaveManager* gSaveManager = *Region::SaveManager();

        u32 oldScore = Region::GetGateScoreFunc()(gSaveManager, slot, -1);

        OSD::Notify(Utils::Format("%x %d %d", gSaveManager, self->condvar, oldScore), CTRPluginFramework::Color::Blue);
        
        if (oldScore < self->condvar) {
            Region::SetGateScoreFunc()(gSaveManager, slot, self->condvar, -1);
            Region::SaveGameFunc()(gSaveManager);
        }
    }

    void displayCondvar(CTickflow* self, u32 arg0, u32* args) {
        // Keeping that just in case
        // Screen bottomScreen = OSD::GetBottomScreen();
        // bottomScreen.Draw("Condvar:"+Utils::Format("0x%08x",self->condvar), 0, 0, Color::White, Color::Black);
        if(arg0 == 0) {
            OSD::Notify("Condvar:"+Utils::Format("0x%08x",self->condvar));
        } else if (arg0 == 1){ 
            OSD::Notify("Condvar:"+Utils::Format("%08d",self->condvar));
        }
    }

}
