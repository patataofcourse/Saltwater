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
            case 0: // Necessary, as it doesn't execute otherwise.
                return Region::TickflowAsyncSubLocation();
            case InputCheck:
                input_cmd(self, arg0, args);
            break;
            case VersionNumber:
                versionCheck(self, arg0, args);
            break;
            case LanguageCheck:
                languageCheck(self, arg0, args);
            break;
            case DisplayCondvar:
                displayCondvar(self, arg0, args);
            break;
        }
        return Region::TickflowCommandsEnd();
    }

    void input_cmd(CTickflow* self, u32 arg0, u32* args) {
        if (arg0 > 2) {
            self->mCondvar = 0;
            return;
        } else if (arg0 == 2) {
            CSaveData** gSaveData = (CSaveData**)Region::GlobalSaveDataPointer();
            arg0 = (*gSaveData)->mFileData[(*gSaveData)->mCurrentFile].mPlayStyle;
        }

        CInputManager** gInputManager = (CInputManager**)Region::GlobalInputManagerPointer();
        if (arg0 == 0) {
            if (args[0] >= 32) {
                self->mCondvar = 0;
                return;
            }
            
            self->mCondvar = ((*gInputManager)->mPadHandler->mHoldButtons >> args[0]) & 1;
        } else {
            self->mCondvar = ((*gInputManager)->mTouchPanelHandler->mTouchPanelStatus.touch);
        }
    }

    void versionCheck(CTickflow* self, u32 arg0, u32* args) {
                if (arg0 == 0){ //RHMPatch version
                    // OSD::Notify("RHMPatch version:"+SUPPORTED_RHMPATCH);
                    self->mCondvar = SUPPORTED_RHMPATCH;
                } else if (arg0 == 1){ //Saltwater version
                    // OSD::Notify("Saltwater version:"+VERSION_MAJOR*0x100+VERSION_MINOR);
                    self->mCondvar = VERSION_MAJOR*0x100+VERSION_MINOR;
                }
    }

    void languageCheck(CTickflow* self, u32 arg0, u32* args) {
        CSaveData** gSaveData = (CSaveData**)Region::GlobalSaveDataPointer();
        CFileManager** gFileManager = (CFileManager**)Region::GlobalFileManagerPointer();
        int locale = (*gSaveData)->mFileData[(*gSaveData)->mCurrentFile].mLocale;
        if(locale == 1){
            self->mCondvar = 0;
        } else {
            wchar_t sublocale[4];
            utf16_to_utf32((u32*)sublocale, (*gFileManager)->mSublocale, 4);
            std::wstring localews(sublocale);
            std::string locales(localews.begin(), localews.end());
            OSD::Notify("Locale:"+locales);
            if(localews.find(L"EN") != (unsigned int)-1){
                self->mCondvar = 1; 
            } else if (localews.find(L"FR") != (unsigned int)-1) {
                self->mCondvar = 2; 
            } else if (localews.find(L"GE") != (unsigned int)-1) {
                self->mCondvar = 3; 
            } else if (localews.find(L"IT") != (unsigned int)-1) {
                self->mCondvar = 4; 
            } else if (localews.find(L"SP") != (unsigned int)-1) {
                self->mCondvar = 5; 
            } else if (localews.find(L"KR") != (unsigned int)-1) {
                self->mCondvar = 7; 
            } else {
                self->mCondvar = -1;
            }
        }
    }

    void displayCondvar(CTickflow* self, u32 arg0, u32* args) {
            OSD::Notify("Condvar:"+Utils::Format("0x%08x",self->mCondvar));
    }

}
