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
    has_map = true;
}

Config* Config::FromFile(std::string fname) {
    File file(fname, File::Mode::READ);
    char* contents = new char[4];
    int result = file.Read(contents, 4);
    if (result || file.GetSize() < 4) return new Config();
    if (result == 0 && std::string(contents) == "SCF\2") {
        map tfmap = {};
        while (true) {
            u16* index = 0;
            result = file.Read(index, 2);
            if (result) return new Config();
            if (*index == 0xC000) break;
            
            u16* strlen = 0;
            result = file.Read(index, 2);
            if (result) return new Config();
            
            char* str = new char[*strlen];
            result = file.Read(str, *strlen);
            if (result) return new Config();
            
            tfmap[*index] = std::string(str);
        }
        return new Config(tfmap);
    }
    return new Config();
}