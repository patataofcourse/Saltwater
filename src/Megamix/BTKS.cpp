#include <3ds.h>
#include "CTRPF.hpp"
#include <map>

#include "Megamix.hpp"
#include "Saltwater.hpp"
#include "Stuff.hpp"

using CTRPluginFramework::File;
using CTRPluginFramework::OSD;

namespace Megamix {
    BTKS btks;

    struct SectionHeader {
        u32 magic;
        u32 size;
    };

    struct BTKSHeader : SectionHeader {
        u32 revision;
        u32 headerSize;
        u32 numSections;
        u32 tickflowVariant;
    };

    int BTKS::LoadFile(const std::string filename) {
        Unload(); 

        if (params.mod_loaded_msg)
            OSD::Notify("Loading BTKS file...");
        
        File file(MEGAMIX_MODS_PATH + filename + ".btk", File::Mode::READ);
        u32 result;

        // Read header
        // -------------------

        BTKSHeader header;
        result = file.Read(&header, sizeof(header)); // Magic
        if (result) return result;

        // sanity checks on the header

        if (header.magic != CHAR4_LE('B', 'T', 'K', 'S')) {
            return -6; // Not a BTKS file
        }

        // rev2 is the only supported revision
        if (header.revision != 2)
            return -7; // Unsupported version
        
        if (!(header.tickflowVariant == 0 || (region == Region::JP && header.tickflowVariant == 1)))
            return -13; // Unsupported Tickflow format

        

        // Seek to end of header (in case there's some non-standard stuff idfk)
        result = file.Seek(header.headerSize, File::SeekPos::SET);
        if (result) return result;

        // Read sections
        // -------------------

        Pointer* pointers = nullptr;
        int numPointers = 0;

        for (u32 i = 0; i < header.numSections; i++) {
            SectionHeader secHeader;
            result = file.Read(&secHeader, sizeof(secHeader)); // Section magic
            if (result) return result;
            
            // TODO: separate each individual section into a function for readability
            
            // Tickflow section
            if (secHeader.magic == CHAR4_LE('F', 'L', 'O', 'W')) {
                if (tickflowSize != 0) {
                    // TODO: error - repeated FLOW section
                }
                tickflowSize = secHeader.size - 0xC;
                result = file.Read(&start, 4); // Position of start sub
                if (result) return result;
                if (tickflowSize > 0x100000) // 1MiB
                    return -12; //file too big
                tickflow = new char[tickflowSize];
                result = file.Read(tickflow, tickflowSize); // Tickflow data
                if (result) return result;
            }
            // Pointer section
            else if (secHeader.magic == CHAR4_LE('P', 'T', 'R', 'O')) {
                int extraBytes = secHeader.size - 0x08;
                result = file.Read(&numPointers, 4); // Number of pointers
                if (result) return result;
                extraBytes -= 5 * numPointers;
                pointers = new Pointer[numPointers];
                for (int i = 0; i < numPointers; i++) {
                    result = file.Read(&pointers[i], 5); // Pointer data
                    if (result) return result;
                }
                file.Read(nullptr, extraBytes); // Extra bytes that may be there for whatever reason
            }
            else if (secHeader.magic == CHAR4_LE('S', 'T', 'R', 'D')) {
                stringSize = secHeader.size - 0x08;
                if (stringSize > 0x80000) // 500KiB
                    return -12; //file too big
                strings = (char*) malloc(stringSize);
                result = file.Read(strings, stringSize);
            }
            else if (secHeader.magic == CHAR4_LE('T', 'M', 'P', 'O')) {
                int extraBytes = secHeader.size - 0xC;
                u32 tempoAmount;
                result = file.Read(&tempoAmount, 4);
                if (result) return result;

                // TODO: restructure this entire code

                for (u32 i = 0; i < tempoAmount; i++) {
                    u32 id;
                    result = file.Read(&id, 4);
                    if (result) return result;
                    
                    u32 dataSize;
                    result = file.Read(&dataSize, 4);
                    if (result) return result;

                    // TODO: this stinks
                    u32 isStreamed_;
                    result = file.Read(&isStreamed_, 4);
                    if (result) return result;
                    bool isStreamed = isStreamed_ != 0;

                    Tempo* data = new Tempo[dataSize];
                    for (u32 i = 0; i < dataSize; i++) {
                        result = file.Read(&data[i].beats, 4);
                        if (result) return result;
                        result = file.Read(&data[i].time, 4);
                        if (result) return result;
                        result = file.Read(&data[i].flag8, 2);
                        if (result) return result;
                        result = file.Read(&data[i].flagA, 2);
                        if (result) return result;
                    }
                    
                    // recalculate tempo data for streamed tempos; this is done on game load for vanilla tempos
                    if (isStreamed && data->flag8 != 1) {
                        u32 time = data->time;
                        float beats = data->beats;
                        u32 time_added = time + 2000;
                        data->beats = (float)time_added * beats / (float)time;
                        data->time = time_added;
                    }

                    TempoTable* tempo = new TempoTable;

                    tempo->id1 = tempo->id2 = id;
                    tempo->isStreamed = (u8)isStreamed;
                    tempo->unkA = 32000; // sample rate?

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
    
        // Handle relocations
        // -------------------

        for (int i = 0; i < numPointers; i++) {
            Pointer pointer = pointers[i];
            if (pointer.pointerType == 0) {
                *(u32*)&tickflow[pointer.pointerPos] += (u32)(strings);
            } else if (pointer.pointerType == 1) {
                *(u32*)&tickflow[pointer.pointerPos] += (u32)(tickflow); 
            } else
                return -10; //unknown pointer type
        }
        start = (u32)tickflow + start;
        loaded = true;

        if (params.mod_loaded_msg)
            OSD::Notify("Tickflow loaded!");

        return 0;
    }

    // TODO: more descriptive name
    void BTKS::Unload() {
        loaded = false;
        delete[] tickflow;
        delete[] strings;
        
        for (auto tempo: tempos) {
            delete[] tempo.second->pos;
            delete tempo.second;
        }

        tempos.clear();
    }
}