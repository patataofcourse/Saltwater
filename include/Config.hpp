#ifndef CONFIG_H
#define CONFIG_H

struct Config {
    bool game, tempo, gate;
    Config();
    Config(char* file);
    static Config FromFile(std::string fname);
};

#endif