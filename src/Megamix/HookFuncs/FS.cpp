#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "Megamix.hpp"

using CTRPluginFramework::File;
using CTRPluginFramework::Utils;

namespace Megamix::Hooks {
    void doReadFile(CFileManager* self, FileInfo* fileInfo) {
        // TODO: triple check everything here works properly - ESPECIALLY the filenames, both for internal game functions and ctrpf functions

        void* cache; // i don't feel like making another struct for this undocumented thing, so pointer arithmetic go brr

        // cached filesystem hell
        if (fileInfo->fileId >= 0 && (cache = Region::IsFileCachedFunc()(*(void**)Region::CacheFileManagerPos(), fileInfo->fileId, fileInfo), cache != 0)) {
            fileInfo->fileSize = *(u32*)(cache + 0x84);
            fileInfo->fileBuffer = new u8[fileInfo->fileSize];
            memcpy(fileInfo->fileBuffer, *(void**)(cache + 0x80), fileInfo->fileSize);
            return;
        }

        FileInputStream inputStream = {(void*)Region::FileInputStreamVtable(), {0, 0, 0}};
        char16_t* buffer = new char16_t[256];
        File file_ctrpf;
        Result result_ctrpf;

        auto game_result_checker = [] (Result result) {
            // taken straight from ghidra
            return (((result & 0x3fc00) == 0x4400) && (99 < (result & 0x3ff))) && ((result & 0x3ff) < 0xb4);
        };

        const char* LAYERED_LOCATION = "/spicerack/fs/%ls%ls";

        wchar_t* strings_utf32[3] = {
            new wchar_t[9],
            new wchar_t[9],
            new wchar_t[0x80]
        };
        utf16_to_utf32((u32*)strings_utf32[0], (u16*)&self->sublocale, 9);
        utf16_to_utf32((u32*)strings_utf32[1], (u16*)&self->locale, 9);
        utf16_to_utf32((u32*)strings_utf32[2], (u16*)&fileInfo->filePath, 0x80);

        // sublocale - SD
        file_ctrpf = File();
        result_ctrpf = File::Open(file_ctrpf, Utils::Format(LAYERED_LOCATION, strings_utf32[0], strings_utf32[2] + 5), File::Mode::READ);

        // locale - SD
        if (result_ctrpf != 0) {
            file_ctrpf.Close();
            result_ctrpf = File::Open(file_ctrpf, Utils::Format(LAYERED_LOCATION, strings_utf32[1], strings_utf32[2] + 5), File::Mode::READ);
        }

        // global - SD
        if (result_ctrpf != 0) {
            file_ctrpf.Close();
            result_ctrpf = File::Open(file_ctrpf, Utils::Format(LAYERED_LOCATION, L"", strings_utf32[2] + 5), File::Mode::READ);
        }
        
        if (result_ctrpf == 0) {
            // TODO: read file here

            s64 size = file_ctrpf.GetSize();
            if (size < 0) {
                ERRF_ThrowResultWithMessage(size, "(m) Could not read file");
            }
            fileInfo->fileSize = size;

            void* fileBuffer = Region::OperatorNewFunc()(fileInfo->fileSize, fileInfo->mode, fileInfo->alignment);
            result_ctrpf = file_ctrpf.Read(fileBuffer, fileInfo->fileSize);
            if (result_ctrpf < 0) {
                ERRF_ThrowResultWithMessage(result_ctrpf, "(m) Could not read file");
            }

            file_ctrpf.Close();
        } else {
            file_ctrpf.Close();

            // sublocale - RomFS
            Region::SWPrintfFunc()(buffer, 256, u"rom:/%ls%ls", self->sublocale, fileInfo->filePath + 5);
            Result result_game = Region::TryOpenFileFunc()(&inputStream.base, buffer, 1);


            // locale - RomFS   
            if (game_result_checker(result_game)) {
                if ((u32)inputStream.base.ptr >> 1 != 0) {
                    if ((u32)inputStream.base.ptr & 1 == 1) {
                        ERRF_ThrowResultWithMessage(-1, "(sl) File pointer is not 2-aligned");
                    }
                    Region::CloseFileFunc()(inputStream.base.ptr);
                    inputStream.base.ptr = nullptr;
                }

                inputStream.base = {0, 0, 0};
                Region::SWPrintfFunc()(buffer, 256, u"rom:/%ls%ls", self->locale, fileInfo->filePath + 5);
                result_game = Region::TryOpenFileFunc()(&inputStream.base, buffer, 1);
            }

            // global - RomFS
            if (game_result_checker(result_game)) {
                if ((u32)inputStream.base.ptr >> 1 != 0) {
                    if ((u32)inputStream.base.ptr & 1 == 1) {
                        ERRF_ThrowResultWithMessage(-1, "(l) File pointer is not 2-aligned");
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
                    ERRF_ThrowResultWithMessage(result_game, "Could not read filesize");
                }

                fileInfo->fileSize = size;
                fileInfo->fileBuffer = Region::OperatorNewFunc()(size, fileInfo->mode, fileInfo->alignment);
                result_game = Region::TryReadFunc()(&inputStream.base, (u32*)&size, fileInfo->fileBuffer, fileInfo->fileSize);
                if (result_game < 0) {
                    ERRF_ThrowResultWithMessage(result_game, "Could not read file");
                }
            }

            // close file if it was opened
            if (((s32)inputStream.base.ptr & 0xfffffffe) != 0) {
                if (((s32)inputStream.base.ptr & 1) == 1) {
                    ERRF_ThrowResultWithMessage(-1, "(g) File pointer is not 2-aligned");
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