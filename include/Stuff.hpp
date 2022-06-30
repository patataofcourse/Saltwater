#include <CTRPluginFramework.hpp>

using CTRPluginFramework::Utils;

namespace Stuff {
    static std::string FileMapToString(std::map<u16, std::string> in) {
        std::string out = "{";
        for (auto item = in.cbegin(); item != in.cend(); ++item) {
            out += Utils::Format("%X: %s, ", item->first, item->second.c_str());
        }
        out += "}";
        return out;
    }
}