#ifndef SALTWATER_H
#define SALTWATER_H

#define STR(X) #X
#define STRC(X) STR(X)

#ifdef RELEASE

#define VERSION_MAJOR 0
#define VERSION_MINOR 2
#define VERSION_MICRO 0

#define VERSION "v" STRC(VERSION_MAJOR) "." STRC(VERSION_MINOR) "." STRC(VERSION_MICRO)

#else
#define VERSION "rev " STRC(COMMIT)
#endif

#endif