#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

using CTRPluginFramework::File;

Config::Config() {
    loadBtks = false;
    btksPath = "";
}

Config::Config(char* file) {
    btksPath = "";
    if (*file) {
        loadBtks = false;
    } else {
        loadBtks = true;
        char strlen = *(file+1);
        for (int i = 0; i < strlen; i++)
            loadBtks += *(file+i+1);
    }
}

Config Config::FromFile(std::string fname) {
    File file(MEGAMIX_BASE_PATH + fname, File::Mode::READ);
    char* contents = new char[6];
    int result = file.Read(contents, 6);
    Config out;
    if (!result && std::string(contents).substr(0, 4) == "SCF\1") {
        out = Config(contents + 4);
    } else {
        char* cfgdata = {0};
        out = Config(cfgdata);

    }
    return out;
}