#ifndef MEGAMIX_BTKS_H
#define MEGAMIX_BTKS_H

#include <map>
#include <string>

#include "Megamix/Types.hpp"

namespace Megamix {
    struct BTKS {
        private:
            struct Pointer {
                u32 pointerPos;
                u8 pointerType;
            };

        public:
            u32 start;
            bool loaded = false;
            char* tickflow = nullptr;
            char* strings = nullptr;
            std::map<u32, TempoTable*> tempos = std::map<u32, TempoTable*>();
            u32 tickflowSize;
            u32 stringSize;

            int LoadFile(std::string filename);
            void Unload();
    };

    extern BTKS btks;
}

#endif