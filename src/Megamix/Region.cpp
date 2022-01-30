#include <3ds.h>
#include <CTRPluginFramework.hpp>

namespace Megamix {
    class Region {
        public:
            int region;

            enum {
                US = 0x000400000018a400,
                EU = 0x000400000018a500,
                JP = 0x0004000000155a00
            };

            Region() {
                CTRPluginFramework::Process::GetTitleID();
            }

            u32 CheerLeadersPatch() {
                if (this->region == US) {
                    return 0x50a8d6;
                } else if (this->region == EU) {
                    return 0x50a94a;
                } else {
                    return 0;
                }
            }
    };
}