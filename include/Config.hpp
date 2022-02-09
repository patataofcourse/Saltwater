#ifndef CONFIG_H
#define CONFIG_H

struct Config {
    bool game, tempo, gate;
    Config(char* file, u32 region);
    static Config* FromFile(std::string fname, u32 region);
};

#endif