#ifndef RHMREGION_H
#define RHMREGION_H

#include <CTRPluginFramework.hpp>
#include <expected>
#include <vector>
#include <string>

#include <3ds/types.h>

#include "Megamix/Error.hpp"
#include "Megamix/Types.hpp"

extern u8 region;

namespace Megamix {
    struct GameInterface {
        u32 gameCode;
        u32 revision; // for potential future use?
        const char* regionName;

        // general code regions

        u32 textEnd;
        u32 rodataEnd;
        u32 dataEnd;
        u32 bssEnd;

        // game definitions

        GameDef* gameTable;
        GateGameDef* gateTable;

        // tickflow loading

        u32 tickflowHookPos;
        u32 gateHookPos;
        u32 gatePracHookPos;
        mutable std::vector<u32> ptrsToRetryRemix;

        // music tempo

        TempoTable* tempoTable;

        u32 strmTempoHookPos;
        u32 seqTempoHookPos;
        u32 allTempoHookPos;

        // museum row hax

        mutable std::vector<u32> ptrsToMuseumRowInfo; // what the fuck is this language
        mutable std::vector<u32> ptrsToMuseumRowColors;
        u32                      rowColorsInitHookPos;
        mutable std::vector<u32> museumRowsR1Cmps;
        mutable std::vector<u32> museumRowsR8Cmps;

        // some common globals / managers

        CSaveData** saveData;
        CInputManager** inputManager;
        CFileManager** fileManager;


        // placeholders for missing/unimplemented values

        static constexpr u32 NO_PTR = 0x404;         // specific region has no applicable func/data
        static constexpr u32 UNIMPLEMENTED = 0xdead; // specific region has applicable func/data but it's not yet implemented
    };

    // Rhythm Tengoku: The Best + (Japan) (0004000000155a00) (rev0)
    extern const GameInterface jpCode;
    // Rhythm Heaven Megamix (Americas) (000400000018a400) (rev0)
    extern const GameInterface usCode;
    // Rhythm Paradise Megamix (Europe) (000400000018a500) (rev0)
    extern const GameInterface euCode;
    // Rhythm Sesang: The Best + (Korea) (000400000018a600) (rev0)
    extern const GameInterface krCode;

    static const GameInterface* allRegions[4] = {&jpCode, &usCode, &euCode, &krCode};

    extern const GameInterface* pointers;

    std::expected<Void, u32> initGameInterface(u32 gameCode);

    namespace Game {
        inline u32 _textEnd () { return pointers->textEnd; }
        inline u32 _rodataEnd() { return pointers->rodataEnd; }
        inline u32 _dataEnd() { return pointers->dataEnd; }
        inline u32 _bssEnd() { return pointers->dataEnd; }

        inline const char* _regionName() { return pointers->regionName; }

        inline GameDef* gGameTable() { return pointers->gameTable; }
        inline GateGameDef* gGateTable() { return pointers->gateTable; }
        inline TempoTable* gTempoTable() { return pointers->tempoTable; }

        inline CSaveData* gSaveData() { return *pointers->saveData; }
        inline CInputManager* gInputManager() { return *pointers->inputManager; }
        inline CFileManager* gFileManager() { return *pointers->fileManager; }

        // for hooks: feel free to drop em here, or make a namespace named hHookGroupName if you feel it needs more context
        namespace Hooks {
            inline u32 tickflow() { return pointers->tickflowHookPos; }
            inline u32 gate() { return pointers->gateHookPos; }
            inline u32 gatePractice() { return pointers->gatePracHookPos; }

            inline u32 strmTempo() { return pointers->strmTempoHookPos; }
            inline u32 seqTempo() { return pointers->seqTempoHookPos; }
            inline u32 allTempo() { return pointers->allTempoHookPos; }
        }

        // for patches: feel free to drop em here, or make a namespace named pPatchName if you feel it needs more context
        namespace Patches {
            inline const std::vector<u32> ptrsToRetryRemix() { return pointers->ptrsToRetryRemix; }
        }

        // if there's any simple hooks (stubs for example) involved you can put em in the patch namespace
        namespace pMuseumRows {
            inline const std::vector<u32> ptrsToInfo() { return pointers->ptrsToMuseumRowInfo; }
            inline const std::vector<u32> ptrsToColors() { return pointers->ptrsToMuseumRowColors; }
            inline u32 colorInitHookPos() { return pointers->rowColorsInitHookPos; }
            inline const std::vector<u32> r1Cmps() { return pointers->museumRowsR1Cmps; }
            inline const std::vector<u32> r8Cmps() { return pointers->museumRowsR8Cmps; }
        }
    }
}


namespace Region {

    enum {
        JP = 0,
        US = 1,
        EU = 2,
        KR = 3,
        KR_CTR = 5, // in the SDK, KR is 5 instead of 3
        UNK
    };

    u8 FromCode(u32 code);

    u32 TickflowCommandsSwitch();
    u32 TickflowCommandsEnd();
    u32 TickflowAsyncSubLocation();

    u32 RegionFSHookFunc();
    u32 RegionOtherHookFunc();

    Megamix::CBlackBarManager** BlackbarLayout();

    typedef int (*SWPrintfSignature) (char16_t* buffer, size_t size, const char16_t* format, ...);
    typedef u32 (*SetTextBoxStringSignature) (Megamix::TextBox *, const char16_t *, u32);

    SWPrintfSignature SWPrintfFunc();
    SetTextBoxStringSignature SetTextBoxStringFunc();
}

#endif