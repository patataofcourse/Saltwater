#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"

using CTRPluginFramework::Utils;
using Megamix::BTKS;

namespace Stuff {
    static inline std::string FileMapToString(std::map<u16, std::string> in) {
        std::string out = "{";
        for (auto item = in.cbegin(); item != in.cend(); ++item) {
            out += Utils::Format("%X: %s, ", item->first, item->second.c_str());
        }
        out += "}";
        return out;
    }

    static inline std::string TempoMapToString(std::map<u32, BTKS::TempoData*> in) {

        std::string out = "{";
        for (auto item = in.cbegin(); item != in.cend(); ++item) {
            out += Utils::Format("%X: Tempo of size %d, ", item->first, ((int*)item->second)[-1]);
        }
        out += "}";
        return out;
    } 
}