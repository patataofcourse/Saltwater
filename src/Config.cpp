#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix/Region.hpp"
#include "Config.hpp"

using CTRPluginFramework::File;

Config::Config() {
    game = false; tempo = false; gate = false;
}

Config::Config(char* file, u32 region) {
    int data = 0;
    switch (region) {
        case Region::JP:
            data = file[0] & 0xf0 >> 4;
            break;
        case Region::US:
            data = file[0] & 0x0f;
            break;
        case Region::EU:
            data = file[1] & 0xf0 >> 4;
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
Config Config::FromFile(std::string fname, u32 region) {
    File file(fname, File::Mode::READ);
    char* contents = new char[6];
    int result = file.Read(contents, 6);
    Config out;
    if (std::string(contents).substr(0, 4) == "SCF\0") {
        char* cfgdata = new char[2];
        cfgdata[0] = contents[4]; cfgdata[1] = contents[5];
        out = Config(cfgdata, region);
        delete[] cfgdata;
    } else {
        char cfgdata[2] = {0,0};
        out = Config(cfgdata, region);
        delete[] cfgdata;

    }
    delete[] contents;
    return out;
}