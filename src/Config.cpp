#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

using CTRPluginFramework::File;

Config::Config() {
    loadBtks = false;
    btksPath = "";
}

Config::Config(char* data, File file) {
    if (*data) {
        btksPath = "";
        loadBtks = false;
    } else {
        loadBtks = true;
        char strlen = *(data+1);
        char* buffer = new char[strlen];
        file.Read(buffer, strlen);
        btksPath = std::string(buffer);
    }
}

Config Config::FromFile(std::string fname) {
    File file(MEGAMIX_BASE_PATH + fname, File::Mode::READ);
    if (file.GetSize() < 6)
        return Config();
    char* contents = new char[6];
    int result = file.Read(contents, 6);
    if (!result && std::string(contents).substr(0, 4) == "SCF\1") {
        return Config(contents + 4, file);
    }
    return Config();
}