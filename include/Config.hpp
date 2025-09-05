#ifndef CONFIG_H
#define CONFIG_H

#include <3ds.h>
#include "CTRPF.hpp"

#include <string>
#include <map>

struct Config {
    typedef std::map<u16, std::string> TickflowMap;

    TickflowMap tickflows;

    Config();
    Config(Config::TickflowMap map);
    static Config FromFile(std::string fname);
};

extern Config config;
extern int configResult;

#endif