#include <3ds.h>
#include <CTRPluginFramework.hpp>
#include <map>

#include "Megamix.hpp"

using CTRPluginFramework::File;
using CTRPluginFramework::OSD;

namespace Megamix {
    BTKS btks;

    int BTKS::LoadFile(const std::string filename) {
        this->Unload();

        if (params.mod_loaded_msg)
            OSD::Notify("Loading BTKS file...");
        
        File file(MEGAMIX_MODS_PATH + filename + ".btk", File::Mode::READ);
        u32 result;
        char* magicBuf = new char[8];
        magicBuf[4] = 0; // terminator
        u32* intBuf = new u32[1];

        //Header
        result = file.Read(magicBuf, 4); // Magic
        if (result) return result;
        if (strcmp(magicBuf,"BTKS")) {
            return -6; // Not a BTKS file
        }

        result = file.Read(intBuf, 4); // Filesize - not that useful rn
        if (result) return result;
        int filesize = *intBuf;

        result = file.Read(intBuf, 4); // Format version - supported: rev2
        if (result) return result;
        int version = *intBuf;
        if (version != 2)
            return -7; // Unsupported version
        
        // Since the only supported version is rev2 we can expect it to always be that

        result = file.Read(intBuf, 4); // Header size - expected: 0x18
        if (result) return result;
        int headerEnd = *intBuf;

        result = file.Read(intBuf, 4); // Number of sections - expected: 3 or 4
        if (result) return result;
        int numSections = *intBuf;

        result = file.Read(intBuf, 4); // Tickflow format - supported: 0 (US/EU/KR)
        if (result) return result;
        if (!(*intBuf == 0 || (region == Region::JP && *intBuf == 1)))
            return -13; // Unsupported Tickflow format

        // Seek to end of header
        result = file.Seek(headerEnd, File::SeekPos::SET);
        if (result) return result;

        // Section code! This is where things get more complicated

        Pointer* pointers = nullptr;
        int numPointers = 0;

        for (int i = 0; i < numSections; i++) {
            result = file.Read(magicBuf, 4); // Section magic
            if (result) return result;
            
            // Tickflow section
            if (!strcmp(magicBuf, "FLOW")) {
                result = file.Read(intBuf, 4); // Section size
                if (result) return result;
                tickflowSize = *intBuf - 0xC;
                tickflow = new char[tickflowSize];
                result = file.Read(intBuf, 4); // Position of start sub
                if (result) return result;
                start = *intBuf;
                if (tickflowSize > 0x100000)
                    return -12; //file too big
                tickflow = (char*) malloc(tickflowSize);
                result = file.Read(tickflow, tickflowSize); // Tickflow data
                if (result) return result;
            }
            // Pointer section
            else if (!strcmp(magicBuf, "PTRO")) {
                result = file.Read(intBuf, 4); // Section size
                if (result) return result;
                int extraBytes = *intBuf - 0x08;
                result = file.Read(intBuf, 4); // Number of pointers
                if (result) return result;
                numPointers = *intBuf;
                extraBytes -= 5 * numPointers;
                pointers = new Pointer[numPointers];
                for (int i = 0; i < numPointers; i++) {
                    result = file.Read(&pointers[i], 5); // Pointer data
                    if (result) return result;
                }
                file.Read(nullptr, extraBytes); // Extra bytes that may be there for whatever reason
            }
            else if (!strcmp(magicBuf, "STRD")) {
                result = file.Read(intBuf, 4); // Section size
                if (result) return result;
                stringSize = *intBuf - 0x08;
                if (stringSize > 0x80000)
                    return -12; //file too big
                strings = (char*) malloc(stringSize);
                result = file.Read(strings, stringSize);
            }
            else if (!strcmp(magicBuf, "TMPO")) {
                result = file.Read(intBuf, 4); // Section size
                if (result) return result;
                int extraBytes = *intBuf - 0xC;
                result = file.Read(intBuf, 4);
                if (result) return result;
                u32 tempoAmount = *intBuf;

                for (u32 i = 0; i < tempoAmount; i++) {
                    result = file.Read(intBuf, 4);
                    if (result) return result;
                    u32 id = *intBuf;
                    result = file.Read(intBuf, 4);
                    if (result) return result;
                    u32 dataSize = *intBuf;
                    result = file.Read(intBuf, 4);
                    if (result) return result;
                    bool is_streamed = *(bool*)intBuf;
                    Tempo* data = new Tempo[dataSize];
                    for (u32 i = 0; i < dataSize; i++) {
                        float* floatBuf = new float;
                        result = file.Read(floatBuf, 4);
                        if (result) return result;
                        data[i].beats = *floatBuf;
                        result = file.Read(intBuf, 4);
                        if (result) return result;
                        data[i].time = *intBuf;
                        result = file.Read(intBuf, 4);
                        if (result) return result;
                        data[i].flag8 = ((u16*)intBuf)[0];
                        data[i].flagA = ((u16*)intBuf)[1];
                    }
                    
                    if (is_streamed && data->flag8 != 1) {
                        u32 time = data->time;
                        float beats = data->beats;
                        u32 time_added = time + 2000;
                        data->beats = (float)time_added * beats / (float)time;
                        data->time = time_added;
                    }

                    TempoTable* tempo = new TempoTable;

                    tempo->id1 = tempo->id2 = id;
                    tempo->unk8 = (u8)is_streamed;
                    tempo->unkA = 0x7D00; //seems to always be this?
                    tempo->pos = data;

                    tempos[id] = tempo;
                }

            }
            else {
                return -9; // Unknown section
            }
        }

        if (tickflow == nullptr || strings == nullptr) {
            return -11; //missing required section
        }

        for (int i = 0; i < numPointers; i++) {
            Pointer pointer = pointers[i];
            if (pointer.pointerType == 0)
                *(u32*)((u32)tickflow + pointer.pointerPos) += (u32)(strings);
            else if (pointer.pointerType == 1)
                *(u32*)((u32)tickflow + pointer.pointerPos) += (u32)(tickflow); 
            else
                return -10; //unknown pointer type
        }
        start = (u32)tickflow + start;
        loaded = true;

        if (params.mod_loaded_msg)
            OSD::Notify("Tickflow loaded!");

        return 0;
    }

    void BTKS::Unload() {
        loaded = false;
        delete[] tickflow;
        delete[] strings;
        
        //this may not be required? but just in case
        for (auto tempo: tempos) {
            delete[] tempo.second->pos;
            delete tempo.second;
        }

        tempos.clear();
    }
}