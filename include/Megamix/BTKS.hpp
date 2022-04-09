#ifndef MEGAMIX_BTKS_H
#define MEGAMIX_BTKS_H

namespace Megamix {
    struct BTKS {
        u32 start;
        bool loaded = false;
        char* tickflow = new char[0x100000];
        char* strings = new char[0x100000];
        //TODO: tempo

        int LoadFile(std::string filename);
        void Unload();

        private:
            struct Pointer {
                u32 pointerPos;
                u8 pointerType;
            };
    };

    extern BTKS btks;
}

#endif