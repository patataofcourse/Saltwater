#include <3ds.h>
#include <CTRPluginFramework.hpp>

using namespace CTRPluginFramework;

namespace Megamix {
    char* rhmpatchBuffer = new char[0x200000]; //Should be 200KB

    int LoadC00Bin(const std::string &path="_:/rhmm/C00.bin") {
        File* file = new File();
        u32 result = File::Open(*file, path, File::Mode::READ);
        return result;
    }

    void PatchTickflowAddresses() {

    }
}