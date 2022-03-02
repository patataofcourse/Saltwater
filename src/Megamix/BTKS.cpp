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
        if (!result) return result;
        if (magicBuf != "BTKS")
            return -6; // Not a BTKS file
        
        result = file.Read(intBuf, 4); // Filesize - not that useful rn
        if (!result) return result;
        int filesize = *intBuf;

        result = file.Read(intBuf, 4); // Format version - supported: rev0
        if (!result) return result;
        int version = *intBuf;
        if (version != 0)
            return -7; // Unsupported version
        
        // Since the only supported version is rev0 we can expect it to always be that

        result = file.Read(intBuf, 4); // Number of sections - expected: 3 or 4
        if (!result) return result;
        int numSections = *intBuf;

        // Section code! This is where things get more complicated

        for (int i = 0; i < numSections; i++) {
            result = file.Read(magicBuf, 4); // Section magic
            if (!result) return result;

            std::string magic = std::string(magicBuf);
            if (magic == "FLOW" || magic == "PTRO" || magic == "TMPO" || magic == "STRD") {                
                return -8; // Not implemented
            }
            else
                return -9; // Unknown section
        }

        return result;
    }

    void BTKS::Unload() {
        free(tickflow);
        free(strings);
    }
}