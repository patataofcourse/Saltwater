#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    bool loadBtks;
    std::string btksPath;
    Config();
    Config(char* file);
    static Config FromFile(std::string fname);
};

#endif