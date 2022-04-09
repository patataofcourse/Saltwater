#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include <string>

#include "Megamix.hpp"

//TODO: add enum
namespace Megamix {
    std::string ErrorMessage(int code) {
        switch (code) {
            // CTRPF errors
            case 0:
                return "No errors!";
            case -1:
                return "Invalid path for file";
            case -2:
                return "File not open, are you sure it exists?";
            case -3:
                return "The mode passed when opened the file doesn't allow this operation";
            case -4:
                return "Invalid argument on file reading";
            case -5:
                return "Unexpected error on file reading";
            // Saltwater / BTKS errors
            case -6:
                return "Not a BTKS file";
            case -7:
                return "Unsupported BTKS version";
            case -8:
                return "Not implemented";
            case -9:
                return "Unknown BTKS section type";
            case -10:
                return "Unknown BTKS pointer type";
            case -11:
                return "Missing BTKS section";
            default:
                return "Unknown error code";
        }
    }
}
