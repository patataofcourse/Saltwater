#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

#include <map>

using CTRPluginFramework::File;

int configResult;

Config::Config() {
    tickflows = {};
}

Config::Config(map tf) {
    tickflows = tf;
}

Config* Config::FromFile(std::string fname) {
    File file(fname, File::Mode::READ);
    char* contents = new char[4];
    configResult = file.Read(contents, 4);
    if (configResult || file.GetSize() < 4) return new Config();
    if (configResult == 0 && strcmp(contents, "SCF\2")) {
        map tfmap = {};
        while (true) {
            u16* index = new u16;
            configResult = file.Read(index, 2);
            if (configResult) return new Config();
            if (*index == 0xC000) break;
            
            u16* strlen = new u16;
            configResult = file.Read(strlen, 2);
            if (configResult) return new Config();
            
            char* str = new char[*strlen + 1];
            configResult = file.Read(str, *strlen);
            if (configResult) return new Config();
            str[*strlen] = 0;
            
            tfmap[*index] = std::string(str);
        }
        return new Config(tfmap);
    }
    return new Config();
}