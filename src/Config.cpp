#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include <Megamix/Region.hpp>

struct Config {
    bool game, tempo, gate;
    Config(char* file, u32 region) {
        int data = 0;
        switch (region) {
            case Region::JP:
                data = file[0] & 0xf0;
                break;
            case Region::US:
                data = file[0] & 0x0f;
                break;
            case Region::EU:
                data = file[1] & 0xf0;
                break;
            case Region::KR:
                data = file[1] & 0x0f;
                break;
            default:
                return;
        }
        if (data & 0b1000) {
            game, tempo, gate = false;
        } else {
            game = data & 0b0100;
            tempo = data & 0b0010;
            gate = data & 0b0001;
        }
    }
    static Config* FromFile(std::string fname, u32 region) {
        return nullptr;
    }
};