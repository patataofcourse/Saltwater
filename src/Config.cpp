#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

using CTRPluginFramework::File;

Config::Config() {
    game = false; tempo = false; gate = false;
}

Config::Config(char* file) {
    int data = 0;
    switch (region) {
        case Region::JP:
            data = (file[0] & 0xf0) >> 4;
            break;
        case Region::US:
            data = file[0] & 0x0f;
            break;
        case Region::EU:
            data = (file[1] & 0xf0) >> 4;
            break;
        case Region::KR:
            data = file[1] & 0x0f;
            break;
        default:
            return;
    }
    if (!(data & 0b1000)) {
        game, tempo, gate = true;
    } else {
        game = data & 0b0100;
        tempo = data & 0b0010;
        gate = data & 0b0001;
    }
}
Config Config::FromFile(std::string fname) {
    File file(MEGAMIX_BASE_PATH + fname, File::Mode::READ);
    char* contents = new char[6];
    int result = file.Read(contents, 6);
    Config out;
    char* cfgdata = new char[2];
    if (result != 0 || std::string(contents).substr(0, 4) == "SCF\0") {
        cfgdata[0] = contents[4]; cfgdata[1] = contents[5];
        out = Config(cfgdata);
    } else {
        cfgdata[0] = 0; cfgdata[1] = 0;
        out = Config(cfgdata);

    }
    return out;
}