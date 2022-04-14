#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"
#include "Config.hpp"

#include <map>

using CTRPluginFramework::File;

Config::Config() {
    tickflows = {};
}

Config Config::FromFile(std::string fname) {
    File file(MEGAMIX_BASE_PATH + fname, File::Mode::READ);
    char* contents = new char[6];
    int result = file.Read(contents, 6);
    if (result || file.GetSize() < 6) return Config();
    if (result == 0 && std::string(contents).substr(0, 4) == "SCF\1") {
        //TODO
    }
    return Config();
}