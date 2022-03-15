#ifndef CONFIG_H
#define CONFIG_H

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include <string>

struct Config {
    bool loadBtks;
    std::string btksPath;
    Config();
    Config(char* file, CTRPluginFramework::File);
    static Config FromFile(std::string fname);
};

#endif