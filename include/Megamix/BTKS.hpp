#ifndef MEGAMIX_BTKS_H
#define MEGAMIX_BTKS_H

#include <map>

namespace Megamix {
    struct BTKS {
        private:
            struct Pointer {
                u32 pointerPos;
                u8 pointerType;
            };

        public:
            struct TempoData {
                float beats;
                u32 time;
                u32 loop_val;
            };

            u32 start;
            bool loaded = false;
            char* tickflow = nullptr;
            char* strings = nullptr;
            std::map<u32, TempoData*> tempos = std::map<u32, TempoData*>();
            u32 tickflowSize;
            u32 stringSize;

            int LoadFile(std::string filename);
            void Unload();
    };

    extern BTKS btks;
}

#endif