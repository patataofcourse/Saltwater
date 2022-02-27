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
        result = file.Read(magicBuf, 4);
        if (!result) return result;

        if (magicBuf != "BTKS")
            return -6; // Not a BTKS file
        
        file.Read(intBuf, 4) //filesize

        //TODO: continue

        return result;
    }
}