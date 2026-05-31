#include <3ds.h>
#include "CTRPF.hpp"

#include "Megamix.hpp"
#include "Config.hpp"
#include "Stuff.hpp"

#include <map>

using CTRPluginFramework::File;

int configResult;

Config config;

Config::Config() {
    tickflows = {};
}

Config::Config(TickflowMap tf) {
    tickflows = tf;
}

Config Config::FromFile(std::string fname) {
    File file(fname, File::Mode::READ);
    u32 magic;
    configResult = file.Read(&magic, 4);
    if (configResult || file.GetSize() < 4) return Config();

    if (configResult == 0 && magic == CHAR4_LE('S', 'C', 'F', '\2')) {
        TickflowMap tfmap = {};
        
        while (true) {
            u16 index;
            configResult = file.Read(&index, 2);
            if (configResult) return Config();
            if (index == 0xC000) break;
            
            u16 strlen;
            configResult = file.Read(&strlen, 2);
            if (configResult) return Config();
            
            std::string str;
            str.resize(strlen);
            configResult = file.Read(str.data(), strlen);
            if (configResult) return Config();
            str[strlen] = 0;
            
            tfmap[index] = str;
        }
        return Config(tfmap);
    }
    return Config();
}