#include <3ds.h>
#include <CTRPluginFramework.hpp>

using CTRPluginFramework::File;

namespace Megamix {
    char* rhmpatchBuffer = new char[0x200000]; //Should be 200KB

    int LoadC00Bin(const std::string &path="_:/rhmm/C00.bin") {
        File* file = new File();
        File::Open(*file, path, File::Mode::READ);
        u32 result = file->Read(rhmpatchBuffer, 0x200000);
        return result;
    }

    void PatchTickflowAddresses() {

    }
}