#ifndef MEGAMIX_BTKS_H
#define MEGAMIX_BTKS_H

namespace Megamix {
    struct BTKS {
        char* tickflow = nullptr;
        char* strings = nullptr;
        //TODO: tempo

        int LoadFile(std::string filename);
        void Unload();

        private:
            struct Pointer {
                u32 pointerPos;
                u8 pointerType;
            };
    };
}

#endif