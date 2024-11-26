#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"

namespace Megamix::Hooks {
    u32 getRegionCTR() {
        //TODO: handle JP region / JP langpack
        if (region == Region::KR)
            return Region::KR_CTR;
        return region;
    }

    u32 getRegionMegamix() {
        //TODO: handle JP region / JP langpack
        return region;
    }
}