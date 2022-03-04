#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"

using CTRPluginFramework::File;

namespace Megamix {
    int BTKS::LoadFile(const std::string filename) {
        File file(MEGAMIX_BASE_PATH + filename, File::Mode::READ);
        u32 result;
        char* magicBuf = new char[4];
        u32* intBuf = new u32[1];

        //Header
        result = file.Read(magicBuf, 4); // Magic
        if (result) return result;
        if (magicBuf != "BTKS")
            return -6; // Not a BTKS file
        
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
        if (!result) return result;
        int numSections = *intBuf;

        // Section code! This is where things get more complicated

        Pointer* pointers;

        for (int i = 0; i < numSections; i++) {
            result = file.Read(magicBuf, 4); // Section magic
            if (result) return result;
            std::string magic = std::string(magicBuf);
            
            // Tickflow section
            if (magic == "FLOW") {
                result = file.Read(intBuf, 4); // Section size
                if (result) return result;
                int tickflowSize = *intBuf - 0x8;
                tickflow = (char*) malloc(tickflowSize);
                result = file.Read(intBuf, 4); // Position of start sub
                if (result) return result;
                start = *intBuf;
                result = file.Read(tickflow, tickflowSize); // Tickflow data
                if (result) return result;
            }
            // Pointer section
            else if (magic == "PTRO") {
                result = file.Read(intBuf, 4); // Section size
                if (result) return result;
                int extraBytes = *intBuf - 0x08;
                result = file.Read(intBuf, 4); // Number of pointers
                if (result) return result;
                int numPointers = *intBuf;
                extraBytes -= 5 * numPointers;
                pointers = new Pointer[numPointers];
                result = file.Read(pointers, 5*numPointers); // Pointer data
                if (result) return result;
                file.Read(nullptr, extraBytes); // Extra bytes that may be there for whatever reason
            }
            else if (magic == "TMPO" || magic == "STRD") {
                return -8; // Not implemented
            }
            else
                return -9; // Unknown section
        }

        return 0;
    }

    void BTKS::Unload() {
        free(tickflow);
        free(strings);
    }
}