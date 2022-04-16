#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

#include <map>

using CTRPluginFramework::File;

Config::Config() {
    tickflows = {};
}

Config::Config(map tf) {
    tickflows = tf;
}

Config Config::FromFile(std::string fname) {
    File file(fname, File::Mode::READ);
    char* contents = new char[4];
    int result = file.Read(contents, 4);
    if (result || file.GetSize() < 4) return Config();
    if (result == 0 && std::string(contents) == "SCF\2") {
        map tfmap = {};
        while (true) {
            u16* index = 0;
            result = file.Read(index, 2);
            if (result) return Config();
            if (*index == 0xC000) break;
            u16* strlen = 0;
            result = file.Read(index, 2);
            if (result) return Config();
            char* str = new char[*strlen];
            result = file.Read(str, *strlen);
            if (result) return Config();
            tfmap[*index] = std::string(str);
        }
        return Config(tfmap);
    }
    return Config();
}