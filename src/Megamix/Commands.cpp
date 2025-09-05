#include <3ds.h>
#include "CTRPF.hpp"

#include <string>

#include "Megamix.hpp"
#include "Saltwater.hpp"

namespace Megamix {
    void input_cmd(CTickflow* self, u32 arg0, u32* args);
    void versionCheck(CTickflow* self, u32 arg0, u32* args);
    void languageCheck(CTickflow* self, u32 arg0, u32* args);
    void displayCondvar(CTickflow* self, u32 arg0, u32* args);
    void msbtWithNum(CTickflow* self, u32 arg0, u32* args);

    void tickflowCommandsHook() {
        asm(
            "mov r0, r6\n"
            "bl tickflowCommandsHookImpl\n"
            "bx r0\n"
        );
    }

    extern "C" __attribute__((used))
    int tickflowCommandsHookImpl(CTickflow* self, u32 cmd_num, u32 arg0, u32* args){
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
            case MSBTWithNum:
                msbtWithNum(self, arg0, args);
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
            // Here, arg0 gets replaced by the playstyle - 0 for buttons, 1 for tap - Results in playstyle-dependant reading
            arg0 = (u32)Game::gSaveData()->fileData[Game::gSaveData()->currentFile].playStyle;
        }

        if (arg0 == 0) {
            // set condvar to 1 if button with flag = 2<<args[0] is pressed

            if (args[0] >= 32) { // We're working with a 32-bit integer here, so flags are limited to bits 1-31
                self->condvar = 0;
                return;
            }
            
            self->condvar = ((u32)Game::gInputManager()->padHandler->holdButtons >> args[0]) & 1;
        } else {
            // set condvar to 1 if screen is pressed
            self->condvar = Game::gInputManager()->touchPanelHandler->touchPanelStatus.touch;
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
        int saveLanguage = Game::gSaveData()->fileData[Game::gSaveData()->currentFile].locale;
        if(saveLanguage == 1){
            self->condvar = 0;
        } else {
            std::u16string sublocale(Game::gFileManager()->sublocale);
            
            sublocale = sublocale.substr(2, 2);
            if (sublocale == u"JP") {
                self->condvar = 0; 
            } else if (sublocale == u"EN") {
                self->condvar = 1; 
            } else if (sublocale == u"FR") {
                self->condvar = 2; 
            } else if (sublocale == u"GE") {
                self->condvar = 3; 
            } else if (sublocale == u"IT") {
                self->condvar = 4; 
            } else if (sublocale == u"SP") {
                self->condvar = 5; 
            } else if (sublocale == u"KR") {
                self->condvar = 6; 
            } else {
                self->condvar = -1;
            }
        }
    }

    void msbtWithNum(CTickflow* self, u32 arg0, u32* args) {
        if (arg0 != 0) return;

        TextBox* textBox = (*Region::BlackbarLayout())->textBox;
        char16_t* out = new char16_t[0x100];

        Region::SWPrintfFunc()(out, 0x100, textBox->textBuf, self->condvar);
        Region::SetTextBoxStringFunc()(textBox, out, 0);

        delete[] out;
    }

    void displayCondvar(CTickflow* self, u32 arg0, u32* args) {
        // Keeping that just in case
        // Screen bottomScreen = OSD::GetBottomScreen();
        // bottomScreen.Draw("Condvar:"+Format("0x%08x",self->condvar), 0, 0, Color::White, Color::Black);
        if(arg0 == 0) {
            OSD::Notify("Condvar:"+Format("0x%08x",self->condvar));
        } else if (arg0 == 1){ 
            OSD::Notify("Condvar:"+Format("%08d",self->condvar));
        }
    }

}
