#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"

using CTRPluginFramework::File;

namespace Megamix {
    BTKS btks;

    int BTKS::LoadFile(const std::string filename) {
        File file(MEGAMIX_BASE_PATH + filename, File::Mode::READ);
        u32 result;
        char* magicBuf = new char[8];
        magicBuf[4] = 0; // terminator
        u32* intBuf = new u32[1];

        //Header
        result = file.Read(magicBuf, 4); // Magic
        if (result) return result;
        if (strcmp(magicBuf,"BTKS")) {
            return -6; // Not a BTKS file
        }

        result = file.Read(intBuf, 4); // Filesize - not that useful rn
        if (result) return result;
        int filesize = *intBuf;

        result = file.Read(intBuf, 4); // Format version - supported: rev0
        if (result) return result;
        int version = *intBuf;
        if (version != 0)
            return -7; // Unsupported version
        
        // Since the only supported version is rev0 we can expect it to always be that

        result = file.Read(intBuf, 4); // Number of sections - expected: 3 or 4
        if (result) return result;
        int numSections = *intBuf;

        // Section code! This is where things get more complicated

        Pointer* pointers = nullptr;
        int numPointers = 0;

        for (int i = 0; i < numSections; i++) {
            result = file.Read(magicBuf, 4); // Section magic
            if (result) return result;
            
            // Tickflow section
            if (!strcmp(magicBuf, "FLOW")) {
                result = file.Read(intBuf, 4); // Section size
                if (result) return result;
                tickflowSize = *intBuf - 0xC;
                tickflow = new char[tickflowSize];
                result = file.Read(intBuf, 4); // Position of start sub
                if (result) return result;
                start = *intBuf;
                if (tickflowSize > 0x100000)
                    return -12; //file too big
                tickflow = (char*) malloc(tickflowSize);
                result = file.Read(tickflow, tickflowSize); // Tickflow data
                if (result) return result;
            }
            // Pointer section
            else if (!strcmp(magicBuf, "PTRO")) {
                result = file.Read(intBuf, 4); // Section size
                if (result) return result;
                int extraBytes = *intBuf - 0x08;
                result = file.Read(intBuf, 4); // Number of pointers
                if (result) return result;
                numPointers = *intBuf;
                extraBytes -= 5 * numPointers;
                pointers = new Pointer[numPointers];
                result = file.Read(pointers, 5*numPointers); // Pointer data
                if (result) return result;
                file.Read(nullptr, extraBytes); // Extra bytes that may be there for whatever reason
            }
            else if (!strcmp(magicBuf, "STRD")) {
                result = file.Read(intBuf, 4); // Section size
                if (result) return result;
                stringSize = *intBuf - 0x08;
                if (stringSize > 0x80000)
                    return -12; //file too big
                strings = (char*) malloc(stringSize);
                result = file.Read(strings, stringSize);
            }
            else if (!strcmp(magicBuf, "TMPO")) {
                //TODO
                return -8; // Not implemented
            }
            else {
                return -9; // Unknown section
            }
        }

        if (tickflow == nullptr || strings == nullptr) {
            return -11; //missing required section
        }
        
        for (int i = 0; i < numPointers; i++) {
            Pointer pointer = pointers[i];
            if (pointer.pointerType == 0)
                *(u32*)((u32)tickflow + pointer.pointerPos) += (u32)(strings);
            else if (pointer.pointerType == 1)
                *(u32*)((u32)tickflow + pointer.pointerPos) += (u32)(tickflow); 
            else
                return -10; //unknown pointer type
        }
        start = (u32)tickflow + start;
        loaded = true;

        return 0;
    }

    void BTKS::Unload() {
        loaded = false;
    }
}