#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"

using CTRPluginFramework::File;
using CTRPluginFramework::Utils;

namespace Megamix::Hooks {
    void doReadFile(CFileManager* self, FileInfo* fileInfo) {
        void* cache; // i don't feel like making another struct for this undocumented thing, so pointer arithmetic go brr

        // cached filesystem hell
        if (fileInfo->fileId >= 0 && (cache = Region::IsFileCachedFunc()(*(void**)Region::CacheFileManagerPos(), fileInfo->fileId, fileInfo), cache != 0)) {
            fileInfo->fileSize = *(u32*)(cache + 0x84);
            fileInfo->fileBuffer = new u8[fileInfo->fileSize];
            memcpy(fileInfo->fileBuffer, *(void**)(cache + 0x80), fileInfo->fileSize);
            return;
        }

        FileInputStream inputStream = {(void*)Region::FileInputStreamVtable(), {0, 0, 0}};
        wchar_t* buffer = new wchar_t[256];
        File file_ctrpf;
        Result result_ctrpf;

        auto game_result_checker = [] (Result result) {
            // taken straight from ghidra
            return (((result & 0x3fc00) == 0x4400) && (99 < (result & 0x3ff))) && ((result & 0x3ff) < 0xb4);
        };

        const char* LAYERED_LOCATION = "/spicerack/fs/%ls%ls";

        // sublocale - SD
        file_ctrpf = File();
        result_ctrpf = File::Open(file_ctrpf, Utils::Format(LAYERED_LOCATION, self->sublocale, fileInfo->filePath + 5), File::Mode::READ);

        // locale - SD
        if (result_ctrpf != 0) {
            file_ctrpf.Close();
            result_ctrpf = File::Open(file_ctrpf, Utils::Format(LAYERED_LOCATION, self->locale, fileInfo->filePath + 5), File::Mode::READ);
        }

        // global - SD
        if (result_ctrpf != 0) {
            file_ctrpf.Close();
            result_ctrpf = File::Open(file_ctrpf, Utils::Format(LAYERED_LOCATION, L"", fileInfo->filePath + 5), File::Mode::READ);
        }
        
        if (result_ctrpf == 0) {
            // TODO: read file here

            s64 size = file_ctrpf.GetSize();
            if (size < 0) {
                // could not read filesize
                svcBreak(USERBREAK_PANIC);
            }
            fileInfo->fileSize = size;

            void* fileBuffer = Region::OperatorNewFunc()(fileInfo->fileSize, fileInfo->mode, fileInfo->alignment);
            result_ctrpf = file_ctrpf.Read(fileBuffer, fileInfo->fileSize);
            if (result_ctrpf < 0) {
                // could not read file
                svcBreak(USERBREAK_PANIC);
            }

            file_ctrpf.Close();
        } else {
            file_ctrpf.Close();

            // sublocale - RomFS
            swprintf(buffer, 256, L"rom:/%ls%ls", self->sublocale, fileInfo->filePath + 5);
            Result result_game = Region::TryOpenFileFunc()(&inputStream.base, buffer, 1);


            // locale - RomFS
            if (game_result_checker(result_game)) {
                if ((u32)inputStream.base.ptr >> 1 != 0) {
                    if ((u32)inputStream.base.ptr & 1 == 1) {
                        // the reason i'm using svcBreak is because any kind of error handling in here is a mess and i don't think we're going to have any errors?

                        // pointer is not 2-aligned
                        svcBreak(USERBREAK_PANIC);
                    }
                    Region::CloseFileFunc()(inputStream.base.ptr);
                    inputStream.base.ptr = nullptr;
                }
            
                inputStream.base = {0, 0, 0};
                swprintf(buffer, 256, L"rom:/%ls%ls", self->locale, fileInfo->filePath + 5);
                result_game = Region::TryOpenFileFunc()(&inputStream.base, buffer, 1);
            }

            // global - RomFS
            if (game_result_checker(result_game)) {
                if ((u32)inputStream.base.ptr >> 1 != 0) {
                    if ((u32)inputStream.base.ptr & 1 == 1) {
                        // pointer is not 2-aligned
                        svcBreak(USERBREAK_PANIC);
                    }
                    Region::CloseFileFunc()(inputStream.base.ptr);
                    inputStream.base.ptr = nullptr;
                }
            
                inputStream.base = {0, 0, 0};
                result_game = Region::TryOpenFileFunc()(&inputStream.base, fileInfo->filePath, 1);
            }

            // read file (or set status accordingly if file couldn't be opened)
            if (game_result_checker(result_game)) {
                fileInfo->status = 0xb; 
            } else {
                s64 size;
                result_game = Region::TryGetSizeFunc()(&inputStream.base, &size);
                if (result_game < 0) {
                    // couldn't read filesize
                    svcBreak(USERBREAK_PANIC);
                }

                fileInfo->fileSize = size;
                fileInfo->fileBuffer = Region::OperatorNewFunc()(size, fileInfo->mode, fileInfo->alignment);
                result_game = Region::TryReadFunc()(&inputStream.base, (u32*)&size, fileInfo->fileBuffer, fileInfo->fileSize);
                if (result_game < 0) {
                    // couldn't read file
                    svcBreak(USERBREAK_PANIC);
                }
            }

            // close file if it was opened
            if (((s32)inputStream.base.ptr & 0xfffffffe) != 0) {
                if (((s32)inputStream.base.ptr & 1) == 1) {
                    // pointer is not 2-aligned
                    svcBreak(USERBREAK_PANIC);
                }

                Region::CloseFileFunc()(inputStream.base.ptr); // original game has an & 0xfffffffe here which is... superflous considering the million ptr checks
                inputStream.base.ptr = nullptr;

            }
        
            // original game has a second file closing here - not adding unless it seems to give any issues because like wtf
        }

        // cache file
        // i still refuse to make a struct for cache shit, even if this is documented
        u8 is_cache_enabled = *(u8*)(Region::CacheFileManagerPos() + 0xc);
        if ((fileInfo->fileId > -1) && is_cache_enabled != 0) { // hey champ shouldnt this also not happen when the file isn't loaded???
            Region::CacheFileFunc()((void*)Region::CacheFileManagerPos(), fileInfo->fileId, fileInfo->filePath, fileInfo->fileBuffer,
                fileInfo->fileSize, fileInfo->mode, fileInfo->alignment);
        }

        // and done!
    }
}