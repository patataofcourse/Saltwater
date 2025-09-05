#include "CTRPF.hpp"

#include "Config.hpp"

#include "Megamix.hpp"

using Megamix::BTKS;

#define CHAR4_LE(char1, char2, char3, char4) ( \
    ((u32)(char4) << 24) | ((u32)(char3) << 16) | \
    ((u32)(char2) <<  8) | ((u32)(char1) <<  0) \
)

namespace Stuff {
    static inline std::string FileMapToString(Config::TickflowMap in) {
        std::string out = "{";
        for (auto item = in.cbegin(); item != in.cend(); ++item) {
            out += Format("%X: %s, ", item->first, item->second.c_str());
        }
        out += "}";
        return out;
    }

    static inline std::string TempoMapToString(std::map<u32, Megamix::TempoTable*> in) {

        std::string out = "{";
        for (auto item = in.cbegin(); item != in.cend(); ++item) {
            out += Format("%X: Tempo of size %d, ", item->first, ((int*)item->second->pos)[-1]);
        }
        out += "}";
        return out;
    } 
}