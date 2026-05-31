#ifndef SALTWATER_H
#define SALTWATER_H

#include "external/plgldr.h"
#include <3ds.h>
#include <assert.h>

// Saltwater version

#define STR(X) #X
#define STRC(X) STR(X)

#define HEX2(Y) 0x##Y
#define HEX(Y) HEX2(Y)

#define VERSION_MAJOR 0
#define VERSION_MINOR 3
#define VERSION_PATCH 0

#define SUPPORTED_RHMPATCH 0x103 

#ifdef RELEASE

#define VERSION "v" STRC(VERSION_MAJOR) "." STRC(VERSION_MINOR) "." STRC(VERSION_PATCH)

#else

#define VERSION "rev " STRC(COMMIT)
#define COMMIT_NUM HEX(COMMIT)

#endif

struct SaltwaterParams {
    u16 barista;
    bool rhmpatch;
    bool plgldr;
    bool mod_loaded_msg;
    bool extra_rows;
    u32 null[30];
};
static_assert(sizeof(SaltwaterParams) == sizeof(PluginLoadParameters::config));

extern SaltwaterParams params;

#endif