#include <3ds.h>
#include <CTRPluginFramework.hpp>

namespace Region {

    enum {
        JP = 0x155a00,
        US = 0x18a400,
        EU = 0x18a500,
        KR = 0x18a600,
    };

    std::string RegionCode(u32 region) {
        if (region == US) {
            return "US";
        } else if (region == EU) {
            return "EU";
        } else if (region == JP) {
            return "JP";
        } else if (region == KR) {
            return "KR";
        } else {
            return "UNK";
        }
    }
}