#include <3ds.h>
#include <CTRPluginFramework.hpp>

namespace Region {

    enum {
        US = 0x000400000018a400,
        EU = 0x000400000018a500,
        JP = 0x0004000000155a00,
        K0 = 0x000400000018A600,
    };

    u32 CheerReadersPatch(u64 region) {
        if (region == US) {
            return 0x50a8d6;
        } else if (region == EU) {
            return 0x50a94a;
        } else {
            return 0;
        }
    }
}